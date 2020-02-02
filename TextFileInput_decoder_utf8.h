#ifndef TEXTFILEINPUT_DECODER_UTF8_H_INCLUDED
#define TEXTFILEINPUT_DECODER_UTF8_H_INCLUDED

#include <cstdint>
#include <string>
#include <limits>
#include "TextFileInput_decoder_base.h"
/**
 * this class provides utility to create decoder between encoding within file and character type that stores unicode codepoint
 * direct use of this class will result in exception thrown , use decoders derived from it
 */

template <integral CodepointCharT>
class TextFileInput_decoder_utf8:public TextFileInput_decoder_base<CodepointCharT>
{
	protected:
        uint_fast32_t m_codepoint;
		int m_bytesleft;
	public:
		TextFileInput_decoder_utf8()=delete;
		/** construct object and start decoding by loading first byte
		 * \exception can throw exception to inform that given first byte is invalid std::invalid_argument
		 */
		TextFileInput_decoder_utf8(const uint8_t & firstbyte):TextFileInput_decoder_base<CodepointCharT>(firstbyte)
		{
			// test if this is valid start byte - starts with 0xxxxxxx ; 110xxxxx; 1110xxxx ; 11110xxx
            if( (firstbyte&0x80) ==0)
			{
				// this is single byte character
                m_bytesleft=0;
                m_codepoint=firstbyte;
			}
			else if( (firstbyte&0xE0) == 0xC0)
			{
				// this is two byte character
				m_bytesleft=1;
				m_codepoint=firstbyte&0x1F;
			}
			else if ((firstbyte&0xF0) == 0xE0)
			{
				// this is three byte character
				m_bytesleft=2;
				m_codepoint=firstbyte&0x0F;
			}
			else if ((firstbyte&0xF8) == 0xF0)
			{
				m_bytesleft=3;
				m_codepoint=firstbyte&0x07;
			}
			else
			{
				// invalid byte
				throw std::invalid_argument("Invalid byte as UTF-8 character start");
			}

		}
		/** Check if decoder has received all bytes of text file
		 * \return true if no more bytes have to be appended , false otherwise
		 */
		virtual bool is_complete()const
		{
			return m_bytesleft==0;
		}
		/** append next byte
		 * \throw if trying to append invalid byte , std::invalid_argument will be thrown
		 * \throw if trying to append to complete character , std::logic_error will be thrown
		 */
		 virtual void append(const uint8_t &nextbyte)
		 {
		 	// check if we are not already done
		 	if(m_bytesleft<0)
				throw std::logic_error("Attempting to add too many bytes for UTF-8 character");
            // check if this byte starts with  10xxxxxx
            if((nextbyte & 0xC0) != 0x80)
				throw std::invalid_argument("non-first byte of character in UTF-8 encoding must conform to 10xxxxxx , it didn't");
			// make space for next 6 bits
			m_codepoint<<=6;
			// append 6 bits from this byte
			m_codepoint |= nextbyte & 0x3F;
            // update bytes left counter
            m_bytesleft--;
		 }
		 /** get unicode codepoint
		  * \return returns codepoint in template type
		  * \throw if character is not complete std::logic_error is thrown
		  * \throw if codepoint cannot be represented in chosen character type, std::overflow_error
		  */
		 virtual CodepointCharT get() const
		 {
		 	// test if we can fit this codepoint within target
		 	if(std::numeric_limits<CodepointCharT>::max() < m_codepoint)
                throw std::overflow_error("Cannot fit codepoint in target character type(overflow)");
			/*if(std::numeric_limits<CodepointCharT>::min() > m_codepoint)
				throw std::underflow_error("Cannot fit codepoint in target character type(underflow)");
			*/
			// return value
			return m_codepoint;
		 }
};



#endif // TEXTFILEINPUT_DECODER_UTF8_H_INCLUDED
