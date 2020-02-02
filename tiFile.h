#ifndef TIFILE_H_INCLUDED
#define TIFILE_H_INCLUDED

#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <optional>
#include <stdexcept>
#include <codecvt>
#include <map>
/* todo : concepts ! */


template <typename CharType>
class tiFileCharacter
{
		std::shared_ptr<std::string> m_source_file;
		int64_t m_line;
		int64_t m_column;
		CharType m_character;
	public:
		tiFileCharacter():m_line(-1),m_column(-1),m_source_file(nullptr){}
		tiFileCharacter(const CharType & in_character ,std::shared_ptr<std::string> in_path_str, const int64_t & in_line , const int64_t &in_column):m_character(in_character),m_source_file(in_path_str),m_line(in_line),m_column(in_column){}
		tiFileCharacter(const tiFileCharacter & other)
		{
			m_source_file=other.m_source_file;
			m_line=other.m_line;
			m_column=other.m_column;
			m_character=other.m_character;
		}

		tiFileCharacter& operator=(const tiFileCharacter & other)
		{
			m_source_file=other.m_source_file;
			m_line=other.m_line;
			m_column=other.m_column;
			m_character=other.m_character;
			return *this;
		}

		std::string source_file() const
		{
			if(nullptr==m_source_file) return "";
			return *m_source_file;
		}
		int64_t source_line() const
		{
			return m_line;
		}
		int64_t source_column() const
		{
			return m_column;
		}
		operator CharType()
		{
			return m_character;
		}
		std::string to_utf8_str() const
		{
			std::string utf8str;
			char32_t val= m_character;
			if(val<=0x007F)
			{
				utf8str+=(char) val;
			}
			else if (val<=0x7FF)
			{
				char byte1 = 0b11000000 | (0b00011111 & (val>>6));
				char byte2 = 0b10000000 | (0b00111111 & val);
				utf8str+=byte1;
				utf8str+=byte2;
			}
			else if (val<=0xFFFF)
			{
				char byte1 = 0b11100000 | (0b00001111 & (val>>12));
				char byte2 = 0b10000000 | (0b00111111 & (val>>6));
				char byte3 = 0b10000000 | (0b00111111 & (val));
				utf8str+=byte1;
				utf8str+=byte2;
				utf8str+=byte3;
			}
			else if (val<=0x10FFFF)
			{
				char byte1 = 0b11110000 | (0b00000111 & (val>>18));
				char byte2 = 0b10000000 | (0b00111111 & (val>>12));
				char byte3 = 0b10000000 | (0b00111111 & (val>>6));
				char byte4 = 0b10000000 | (0b00111111 & (val));
				utf8str+=byte1;
				utf8str+=byte2;
				utf8str+=byte3;
				utf8str+=byte4;
			}
			return utf8str;
		}


};

/** \class tiFile - short for Text Input File - provides reading access to encoded text files such as UTF-8
 *
 *   \warning - do not pass copies of this object, as it will open and parse text file once again
 */
template <typename CharType,template<typename> typename DecoderType,int64_t line_index_base = 1, int64_t column_index_base=line_index_base>
class tiFile
{
	public:



		/*-------------------- constructors ------------------------------*/
		/** construct file object, but do not initialize it*/
		tiFile()
		{
			msp_path=nullptr;
			m_eof_met=false;
			m_current_line=invalid_line;
			m_current_column=invalid_column;
		}
		/** create file object and  attempt to open file*/
		tiFile(const char * path)
		{
			m_eof_met=false;
			open(path);
    	}
		/** create file object and  attempt to open file*/
		tiFile(const std::string & path)
		{
			m_eof_met=false;
			open(path);
		}
		/*--------------------- destructor ---------------------------------*/
		/** close opened file and destroy file object*/
		virtual ~tiFile()
		{
			m_stream.close();
			msp_path=nullptr;
		}
		/*-------------------open / close ----------------------------------*/
		/** open file */
		void open(const char* path)
		{
            open(std::string(path));
		}
		/** open file */
		void open(const std::string & path)
		{
            close();
			msp_path=std::make_shared<std::string>(path);
			m_stream.open(path);
			m_current_line=line_index_base;
			m_current_column=column_index_base;
			//m_stream.exceptions(std::ios_base::failbit|std::ios_base::badbit);
			if(false==m_stream.is_open())
			{
				msp_path=nullptr;
				m_current_line=invalid_line;
				m_current_column=invalid_column;
			}
			else
			{
				m_eof_met=false;
				m_line_start_pos[line_index_base]=m_stream.tellg();
			}

		}
		/** test if file is opened*/
		bool is_open() const
		{
            return m_stream.is_open();
		}
		/** close opened file*/
		void close()
		{
			m_stream.close();
			msp_path=nullptr;
			m_current_line=invalid_line;
			m_current_column=invalid_column;
			m_line_start_pos.clear();
			m_line_length.clear();
			m_eof_met=false;
		}
		/* ----------------------- positioning in file --------------------*/
		/** returns line number of next character read
		 * \return line number of next get() action , or tiFile<CharType,DecoderType>::invalid_line if no file is opened
		 */
		int64_t get_current_line() const
		{
			return m_current_line;
		}
		/** returns column number of next character read
		 * \return column number of next get() action , or tiFile<CharType,DecoderType>::invalid_column if no file is opened
		 */
		int64_t get_current_column() const
		{
            return m_current_column;
		}
		/** go to line provided*/
		void seek_to_line(int64_t lineNo)
		{
			// check if file is open
			if(!is_open()) throw std::logic_error("cannot seek if file is not opened");
			// check if line number is valid
			if(lineNo<line_index_base) throw std::out_of_range("cannot seek to line with number less than starting index");
			// check if we know where this line starts
			if(m_line_start_pos.count(lineNo)>0)
			{
				m_stream.seekg(m_line_start_pos[lineNo]);
				m_current_line=lineNo;
				m_current_column=column_index_base;
				return;
			}
			// check if we can know how many lines are within this file
			if(m_eof_met)
			{
				throw std::out_of_range("file does not appear to have that much lines");
			}
			// seek to closest known position
			auto closestKnown=m_line_start_pos.lower_bound (lineNo);
            seek_to_line(closestKnown->first);
            // read until we reach desired position or  end of file
            while(!m_eof_met)
			{
				get_raw();
				if(m_current_line == lineNo) return; // we should be there
			}
			throw std::out_of_range("file does not appear to have that much lines");



		}
		/* -------------------- reading operations ------------------------*/
		/** Reads next character from file(including end-of-line characters) , returns in tiFileCharacter<T> form
		 * \return if there is such character , returns unicode codepoint , if end of file was reached , or i/o error occurred, no value will be returned
		 * \exception If trying to read when no file is opened std::logic_error is thrown
		 * \exception If i/o error happens (failbit or badbit is set) nested std::ios_base::failure is thrown
		 */
		std::optional<tiFileCharacter<CharType> > get()
		{
			auto char_line = get_current_line();
			auto char_column = get_current_line();
			auto ch = get_raw();
			if(false==ch.has_value()) return {};
            return tiFileCharacter<CharType> (ch.value(),msp_path,char_line,char_column);
		}
		/** Reads next character from file(including end-of-line characters)
		 * \return if there is such character , returns unicode codepoint , if end of file was reached , or i/o error occurred, no value will be returned
		 * \exception If trying to read when no file is opened std::logic_error is thrown
		 * \exception If i/o error happens (failbit or badbit is set) nested std::ios_base::failure is thrown
		 */
		std::optional<CharType> get_raw()
		{
			/*test if file is open*/
			if(false==is_open()) throw std::logic_error("Trying to get() when no file is opened");
			/*test if end-of-file flag is already set, if so, nothing to read here*/
			if(m_stream.eof())	return {};
            /*store current position , because we might read more than 1 bytes from file (due to encoding) - thus ifstream::peek() will not work */
            auto starting_pos = m_stream.tellg();
            /*clear status flags*/
            m_stream.clear();
            /* read first character */
            try
            {
            	/*read first byte*/
				auto firstbyte=m_stream.get();
				if( m_stream.eof() )
				{
					/*ok, we have reached end of file , and character has not begun yet*/
					return {};
				}
				if(m_stream.fail())
				{
					/*some i/o error occurred (other than EOF) */
					throw std::ios_base::failure("i/o error");
				}
            	/*create decoder object*/
            	DecoderType<CharType> char_dec(firstbyte);
            	/*complete reading of character*/
            	while(false==char_dec.is_complete())
				{
					auto next_byte=m_stream.get();
					if(m_stream.fail())
					{
						throw std::ios_base::failure("could not complete reading character due to i/o error");
					}
					char_dec.append(next_byte);
				}
            	/* test if this is last byte of file*/
				bool last_byte_of_file=false;
				m_stream.clear();
				m_stream.peek();
				if(m_stream.eof())
				{
					last_byte_of_file=true;
					m_eof_met=true;
				}
				m_stream.clear();

            	/* advance location counters*/
			    bool eol_detected=false;
			    if(is_EOL(char_dec.get()))
				{
					if(CR==char_dec.get())
					{
                        auto next_char = peek();
                        if(LF==next_char.value_or(CR))
						{
							/*we will break line after we encounter LF*/
						}
						else
						{
							eol_detected=true;
						}
					}
					else
					{
						eol_detected=true;
					}
				}
				if(eol_detected)
				{
					m_line_length[m_current_line]=m_current_column-column_index_base+1;
					m_current_column=column_index_base;
					m_current_line++;
					if(!last_byte_of_file)
					{
						m_line_start_pos[m_current_line]=m_stream.tellg();
					}
				}
				else
				{
					m_current_column++;
				}
				/* return codepoint*/
				return char_dec.get();
            }
            catch(...)
            {
            	/*try to rewind*/
                m_stream.seekg(starting_pos-m_stream.tellg(),std::ios_base::cur);
            	/*throw nested exception*/
            	std::throw_with_nested(std::ios_base::failure("could not read character"));
            }

		}
		/** Returns next character from file without advancing file position
		 * \return if there is such character , returns unicode codepoint , if end of file was reached , or i/o error occurred, no value will be returned
		 * \exception If trying to peek when no file is opened std::logic_error is thrown
		 * \exception If i/o error happens (failbit or badbit is set) nested std::ios_base::failure is thrown
		 */
		std::optional<CharType> peek()
		{
			/*test if file is open*/
			if(false==is_open()) throw std::logic_error("Trying to peek in un-opened file");
			/*test if end-of-file flag is already set, if so, nothing to read here*/
			if(m_stream.eof())	return {};
            /*store current position , because we might read more than 1 bytes from file (due to encoding) - thus ifstream::peek() will not work */
            auto starting_pos = m_stream.tellg();
            auto m_starting_line = m_current_line;
            auto m_starting_column = m_current_column;
            /*clear status flags*/
            m_stream.clear();
            /* read first character */
            try
            {
            	auto answ=get_raw();
            	/*rewind*/
            	m_stream.seekg(starting_pos-m_stream.tellg(),std::ios_base::cur);
            	m_current_line=m_starting_line;
            	m_current_column=m_starting_column;
				/* return codepoint*/
				return answ;
            }
            catch(...)
            {
            	/*try to rewind*/
                m_stream.seekg(starting_pos-m_stream.tellg(),std::ios_base::cur);
            	m_current_line=m_starting_line;
            	m_current_column=m_starting_column;
            	/*throw nested exception*/
            	std::throw_with_nested(std::ios_base::failure("could not read character"));
            }

		}


		/** get line , stored in std::basic_string<tiFileCharacter>
		 * if end of line character is encountered, it is red from stream , but discarded and not written to string
		 */
		std::optional<std::vector<tiFileCharacter<CharType> > > getline()
		{
			std::vector<tiFileCharacter<CharType> > answ;
			bool any_char_found=false;
            while (auto current_char = get() )
			{
				any_char_found=true;
				if((CharType)is_EOL(current_char.value()))
				{
					if(CR==current_char.value())
					{
						if(LF==peek()) get();
					}
					break;
				}
				answ.push_back(current_char.value());

			}
			if(any_char_found)return answ;
			return {};
		}
		/* ---------------------- misc ------------------------------------*/
		/** returns path of opened file*/
		std::string get_path() const
		{
			if(msp_path==nullptr) return "";
			return *msp_path;
		}
		/** returns true if character provided is considered as End-Of-Line*/
		static bool is_EOL(const char32_t & unicode_codepoint)
		{
			switch(unicode_codepoint)
			{
				case LF:
				case VT:
				case FF:
				case CR:
				case NEL:
				case LS:
				case PS:
					return true;
				default:
					return false;
			}

		}

		/** get number of lines in file
		 * \warning this requires reading whole file before. if it hasn't already done that , it will be performed
		 */
		 int64_t total_lines()
		 {
		 	if(m_eof_met) return m_line_start_pos.rbegin()->first-line_index_base;
		 	auto current_pos_f = m_stream.tellg();
		 	auto curr_line = m_current_line;
		 	auto curr_col=m_current_column;
		 	while(!m_eof_met) get_raw();
		 	m_stream.seekg(current_pos_f);
		 	m_current_line=curr_line;
		 	m_current_column=curr_line;
			if(m_eof_met) return m_line_start_pos.rbegin()->first-line_index_base;
		 	else return -1;

		 }

	private:
		std::shared_ptr<std::string> msp_path;///< stores path of currently opened file (shared pointer , because red characters can also point to it)
		std::ifstream m_stream;///< stores file stream
		int64_t m_current_line;///< stores current line number(where next character is red) ; -1 if file is not opened
		int64_t m_current_column;///< stores current column number(where next character is red) ; -1 if file is not opened
		std::map<int64_t ,std::streampos> m_line_start_pos; ///< stores start positions of lines
		std::map<int64_t, int64_t> m_line_length;///< stores how many characters are within line
		bool m_eof_met;///< true if end of file has been reached at least once
	public:
		constexpr static char32_t LF = 0x000A;
		constexpr static char32_t VT = 0x000B;
		constexpr static char32_t FF = 0x000C;
		constexpr static char32_t CR = 0x000D;
		constexpr static char32_t NEL = 0x0085;
		constexpr static char32_t LS = 0x2028;
		constexpr static char32_t PS = 0x2029;
		constexpr static int64_t invalid_line=std::numeric_limits<int64_t>::min()/*line_index_base-1*/;
		constexpr static int64_t invalid_column=std::numeric_limits<int64_t>::min()/*column_index_base-1*/;
		static_assert(line_index_base>std::numeric_limits<int64_t>::min(),"linde index cannot start at minimum value of int64_t ");
		static_assert(column_index_base>std::numeric_limits<int64_t>::min(),"column index cannot start at minimum value of int64_t ");


};
template <typename CharType>
std::ostream& operator<<(std::ostream& os, const tiFileCharacter<CharType> & dt)
{
	os<<dt.to_utf8_str();
    return os;
}

template <typename CharType>
std::ostream& operator<<(std::ostream& os, const std::vector<tiFileCharacter<CharType> > & dt)
{
	for(auto c:dt)
	os<<c.to_utf8_str();
    return os;
}


#endif // TIFILE_H_INCLUDED
