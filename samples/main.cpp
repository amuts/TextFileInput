#include <iostream>
#include <exception>
#include <vector>
#include <queue>
#include <string>
#include "../TextFileInput_decoder_utf8.h"
using namespace std;

void test_utf8_decoder()
{
	struct test_sample_struct
	{
		std::string test_title;
 		std::queue<uint8_t> byte_sequence;
 		uint32_t expected_result;
        test_sample_struct(const char* in_title,std::vector<uint8_t> in_byte_seq, uint32_t in_expected_result)
        {
        	test_title=in_title;
        	for(auto byte:in_byte_seq)
				byte_sequence.push(byte);
			expected_result=in_expected_result;
        }
	};
	std::vector<test_sample_struct>  testSamples = {
										test_sample_struct("Testing character 'A' ", {0x41},65),
										test_sample_struct("Testing last single byte character ", {0x7F},127),
										test_sample_struct("Testing first two byte character ", {0xC2,0x80},128),
										test_sample_struct("Testing last two byte character ", {0xDF,0xBF},2047),
										test_sample_struct("Testing first three byte character ", {0xE0,0xA0,0x80},2048),
										test_sample_struct("Testing last three byte character ", {0xEF,0xBF,0xBF},65535),
										test_sample_struct("Testing first four byte character ", {0xF0,0x90,0x80,0x80},65536)
									};



    cout << "Testing utf-8 decoder" << endl;

	for(test_sample_struct test:testSamples)
	{
        std::cout<<"  "<<test.test_title<<" : ";
        try
        {
        	TextFileInput_decoder_utf8<char32_t> testable(test.byte_sequence.front());
        	test.byte_sequence.pop();
        	while(!testable.is_complete())
			{
                if(test.byte_sequence.empty())
				{
					throw "expects more bytes than should";
				}
				testable.append(test.byte_sequence.front());
				test.byte_sequence.pop();
			}
			if(!test.byte_sequence.empty())
			{
                throw "did not use all bytes";
			}
			if(test.expected_result != testable.get())
			{
                throw "expectation not met";
			}
			std::cout<<"passed";




        }
        catch(const char* msg)
        {
        	std::cout<<" Failed ("<<msg<<")";
        }
        catch(std::exception &e)
		{
			std::cout<<" Failed (exception)"<<std::endl<<"      "<<e.what();
		}
		catch(...)
		{
			std::cout<<" Failed (unknown exception)";
		}
		std::cout<<std::endl;
	}

}

int main()
{

	test_utf8_decoder();

	return 0;
}
