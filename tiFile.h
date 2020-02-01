#ifndef TIFILE_H_INCLUDED
#define TIFILE_H_INCLUDED

#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include <optional>
#include <stdexcept>
/* todo : concepts ! */

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
		}
		/** create file object and  attempt to open file*/
		tiFile(const char * path)
		{
			open(path);
    	}
		/** create file object and  attempt to open file*/
		tiFile(const std::string & path)
		{
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
			m_current_line=-1;
			m_current_column=-1;
		}
		/* ----------------------- positioning in file --------------------*/
		/** returns line number of next character read*/
		int64_t get_current_line() const
		{
			return m_current_line;
		}
		/** returns column number of next character read*/
		int64_t get_current_column() const
		{
            return m_current_column;
		}
		/* -------------------- reading operations ------------------------*/
		/** Reads next character from file(including end-of-line characters)
		 * \return if there is such character , returns unicode codepoint , if end of file was reached , or i/o error occurred, no value will be returned
		 * \exception If trying to read when no file is opened std::logic_error is thrown
		 * \exception If i/o error happens (failbit or badbit is set) nested std::ios_base::failure is thrown
		 */
		std::optional<CharType> get()
		{
			/*test if file is open*/
			if(false==is_open()) throw std::logic_error("Trying to get() in un-opened file");
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
            	/* advance location counters*/
			    // TODO

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
            	/*rewind*/
            	m_stream.seekg(starting_pos-m_stream.tellg(),std::ios_base::cur);
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


		/* ---------------------- misc ------------------------------------*/
		std::string get_path() const
		{
			if(msp_path==nullptr) return "";
			return *msp_path;
		}


	private:
		std::shared_ptr<std::string> msp_path;///< stores path of currently opened file (shared pointer , because red characters can also point to it)
		std::ifstream m_stream;///< stores file stream
		int64_t m_current_line;///< stores current line number(where next character is red) ; -1 if file is not opened
		int64_t m_current_column;///< stores current column number(where next character is red) ; -1 if file is not opened

};

#endif // TIFILE_H_INCLUDED
