#include <iostream>
#include <exception>
#include <vector>
#include <queue>
#include <string>
#include "../TextFileInput_decoder_utf8.h"
#include "../tiFile.h"
using namespace std;


void test_tiFile()
{
	try
	{
		std::clog<<"Opening file";
		tiFile<char16_t,TextFileInput_decoder_utf8 > inputFile_1("TestInputs/simple_utf-8_file.txt") ;
		std::clog<<std::endl;
		std::clog<<std::boolalpha<<"Is opened : "<<inputFile_1.is_open()<<std::endl;
		std::clog<<"Opened path:\""<<inputFile_1.get_path()<<"\""<<std::endl;

        std::clog<<"Peeking character at location : Line "<<inputFile_1.get_current_line()<<"   column: "<<inputFile_1.get_current_column()<<std::endl;
        auto peeked_char = inputFile_1.peek();
        if(peeked_char.has_value())
			std::clog<<" character = "<<(char)peeked_char.value();
		else
			std::clog<<" failed to obtain value (EOF?)";
		std::clog<<std::endl;

		std::clog<<"Peeking character at location : Line "<<inputFile_1.get_current_line()<<"   column: "<<inputFile_1.get_current_column()<<std::endl;
        peeked_char = inputFile_1.peek();
        if(peeked_char.has_value())
			std::clog<<" character = "<<(char)peeked_char.value();
		else
			std::clog<<" failed to obtain value (EOF?)";
		std::clog<<std::endl;





		std::clog<<"closing file..";
		inputFile_1.close();
		std::clog<<"."<<std::endl;
		std::clog<<std::boolalpha<<"Is opened : "<<inputFile_1.is_open()<<std::endl;
		std::clog<<"Opened path:\""<<inputFile_1.get_path()<<"\""<<std::endl;
        std::clog<<"Peeking character at location : Line "<<inputFile_1.get_current_line()<<"   column: "<<inputFile_1.get_current_column();

	}
	catch(std::exception & e)
	{
        std::cerr<<"[Exception]"<<e.what()<<std::endl;
	}
	catch(...)
	{
        std::cerr<<"unexpected exception"<<std::endl;
	}

}

int main()
{
	test_tiFile();

	return 0;
}
