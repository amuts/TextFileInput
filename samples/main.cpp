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
		tiFile<char16_t,TextFileInput_decoder_utf8 > inputFile_1("TestInputs/bible.txt") ;
		std::clog<<std::endl;
		std::clog<<std::boolalpha<<"Is opened : "<<inputFile_1.is_open()<<std::endl;
		std::clog<<"Opened path:\""<<inputFile_1.get_path()<<"\""<<std::endl;
		std::clog<<"Lines: "<<inputFile_1.total_lines()<<std::endl;


		inputFile_1.seek_to_line(30362);
		std::clog<< inputFile_1.getline().value();



		while(auto currentChar=inputFile_1.get())
		{
          //  std::clog<<currentChar.value();
		}





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
