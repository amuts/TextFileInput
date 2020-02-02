#ifndef TEXTFILEINPUT_DECODER_BASE_H_INCLUDED
#define TEXTFILEINPUT_DECODER_BASE_H_INCLUDED

#include <cstdint>
#include <string>
#include <type_traits>
//#include <concepts>
/**
 * this class provides utility to create decoder between encoding within file and character type that stores unicode codepoint
 * direct use of this class will result in exception thrown , use decoders derived from it
 */

template < class T >
concept integral = std::is_integral_v<T>;

template <integral CodepointCharT>
class TextFileInput_decoder_base
{
	public:
		TextFileInput_decoder_base()=delete;
		/** construct object and start decoding by loading first byte
		 * \exception can throw exception to inform that given first byte is invalid std::invalid_argument
		 */
		TextFileInput_decoder_base(const uint8_t & firstbyte){}
		/** Check if decoder has received all bytes of text file
		 * \return true if no more bytes have to be appended , false otherwise
		 */
		virtual bool is_complete()const
		{
			std::logic_error(std::string(__PRETTY_FUNCTION__)+": use of decoder base is illegal , use proper decoder"); /*maybe different exception type?*/
			return true;
		}
		/** append next byte
		 * \throw if trying to append invalid byte , std::invalid_argument will be thrown
		 * \throw if trying to append to complete character , std::logic_error will be thrown
		 */
		 virtual void append(const uint8_t &nextbyte)
		 {
			throw std::logic_error(std::string(__PRETTY_FUNCTION__)+": use of decoder base is illegal , use proper decoder");/*maybe different exception type ?*/
		 }
		 /** get unicode codepoint
		  * \return returns codepoint in template type
		  * \throw if character is not complete std::logic_error is thrown
		  * \throw if codepoint cannot be represented in chosen character type, std::overflow_error
		  */
		 virtual CodepointCharT get() const
		 {
            throw std::logic_error(std::string(__PRETTY_FUNCTION__)+": use of decoder base is illegal , use proper decoder");/*maybe different exception type ?*/
		 }
};



#endif // TEXTFILEINPUT_DEENCODER_BASE_H_INCLUDED
