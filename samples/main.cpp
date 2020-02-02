#include <iostream>
#include <exception>
#include <vector>
#include <queue>
#include <string>
#include <optional>
#include "../TextFileInput_decoder_utf8.h"
#include "../tiFile.h"
#include <chrono>
#include <cstdio>
#include <sstream>
using namespace std;

std::string exception_to_string(const std::exception& e, int level =  0)
{
	std::string result(level,' ');
    result+= e.what() ;
    result+='\n';
    try {
        std::rethrow_if_nested(e);
    } catch(const std::exception& e) {
        result+=exception_to_string(e, level+1);
    } catch(...) {}
    return result;
}
template <typename T>
std::string _to_string(const T & input)
{
	std::ostringstream oss;
	oss<<input;
	return oss.str();
}

struct testResult
{
	bool failed;
	std::string error_messages;
	std::string test_title;
	std::string exception_string;
	std::chrono::duration<double> elapsed_seconds;
};

std::vector<testResult> test_tiFile_constructors()
{
	std::vector<testResult> results;
	testResult currentResult;
	auto start_time=std::chrono::system_clock::now();


	currentResult.failed=true;
	currentResult.test_title="constructor tiFile::tiFile()";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		tiFile<char,TextFileInput_decoder_utf8> testable ;
		if(testable.is_open())
		{
			currentResult.failed=true;
			currentResult.error_messages="after constructing with default constructor, is_open should return false";
		}
		else currentResult.failed=false;

	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);







	currentResult.failed=true;
	currentResult.test_title="constructor tiFile::tiFile(const char* ) with existing file \"simple_ascii_file.txt\"";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		tiFile<char,TextFileInput_decoder_utf8> testable("TestInputs/simple_ascii_file.txt") ;
		if(false==testable.is_open())
		{
			currentResult.failed=true;
			currentResult.error_messages="did not open \"simple_ascii_file.txt\"";
		}
		else currentResult.failed=false;

	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);







	std::string tmp_path_0001=std::tmpnam(nullptr);
	currentResult.failed=true;
	currentResult.test_title="constructor tiFile::tiFile(const char* ) with non-existing file \"";
	currentResult.test_title+=tmp_path_0001;
	currentResult.test_title+="\"";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		tiFile<char,TextFileInput_decoder_utf8> testable(tmp_path_0001.c_str()) ;
		if(true==testable.is_open())
		{
			currentResult.failed=true;
			currentResult.error_messages="should have failed opening non-existing file, but didn't";
		}
		else currentResult.failed=false;

	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);







	currentResult.failed=true;
	currentResult.test_title="constructor tiFile::tiFile(const string& ) with existing file \"simple_ascii_file.txt\"";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		tiFile<char,TextFileInput_decoder_utf8> testable(std::string("TestInputs/simple_ascii_file.txt")) ;
		if(false==testable.is_open())
		{
			currentResult.failed=true;
			currentResult.error_messages="did not open \"simple_ascii_file.txt\"";
		}
		else currentResult.failed=false;

	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);







	std::string tmp_path_0002=std::tmpnam(nullptr);
	currentResult.failed=true;
	currentResult.test_title="constructor tiFile::tiFile(const std::string& ) with non-existing file \"";
	currentResult.test_title+=tmp_path_0002;
	currentResult.test_title+="\"";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		tiFile<char,TextFileInput_decoder_utf8> testable(tmp_path_0002) ;
		if(true==testable.is_open())
		{
			currentResult.failed=true;
			currentResult.error_messages="should have failed opening non-existing file, but didn't";
		}
		else currentResult.failed=false;

	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);




	return results;
}

std::vector<testResult> test_tiFile_open_close()
{
	std::vector<testResult> results;
	testResult currentResult;
	auto start_time=std::chrono::system_clock::now();


	currentResult.failed=true;
	currentResult.test_title="test tiFile::open() when no file is opened";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		do
		{
			tiFile<char,TextFileInput_decoder_utf8> testable ;
			if(testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="could not create test conditions";
				break;
			}
			testable.open("TestInputs/simple_ascii_file.txt");
			if(false==testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="failed to open \"TestInputs/simple_ascii_file.txt\"";
				break;
			}
			currentResult.failed=false;
		}while(0);
	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);




    currentResult.failed=true;
	currentResult.test_title="test tiFile::open() when another file is already opened";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		do
		{
			tiFile<char,TextFileInput_decoder_utf8> testable("TestInputs/E.coli") ;
			if(false==testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="could not create test conditions";
				break;
			}
			testable.open("TestInputs/simple_ascii_file.txt");
			if(false==testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="failed to open \"TestInputs/simple_ascii_file.txt\"";
				break;
			}
			currentResult.failed=false;
		}while(0);
	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);




    currentResult.failed=true;
	currentResult.test_title="test tiFile::close() when file is already opened";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		do
		{
			tiFile<char,TextFileInput_decoder_utf8> testable("TestInputs/E.coli") ;
			if(false==testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="could not create test conditions";
				break;
			}
			testable.close();
			if(testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="failed to close()";
				break;
			}
			currentResult.failed=false;
		}while(0);
	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);





    currentResult.failed=true;
	currentResult.test_title="test tiFile::close() when file is not open";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		do
		{
			tiFile<char,TextFileInput_decoder_utf8> testable ;
			if(testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="could not create test conditions";
				break;
			}
			testable.close();
			if(testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="failed to close()";
				break;
			}
			currentResult.failed=false;
		}while(0);
	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);

	return results;
}


std::vector<testResult> test_tiFile_positioning()
{
	std::vector<testResult> results;
	testResult currentResult;
	auto start_time=std::chrono::system_clock::now();

    //--------------------------------------------------------------- get current line -------------------------------------------------------------------------------------------

	currentResult.failed=true;
	currentResult.test_title="test tiFile::get_current_line() when no file is opened";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		do
		{
			tiFile<char,TextFileInput_decoder_utf8> testable ;
			if(testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="could not create test conditions";
				break;
			}
			if(tiFile<char,TextFileInput_decoder_utf8>::invalid_line!=testable.get_current_line())
			{
				currentResult.failed=true;
				currentResult.error_messages="get_current_line should return tiFile::invalid_line when no file is opened;\n Returned:"+_to_string(testable.get_current_line());
				currentResult.error_messages+="\n(tiFile::invalid_line ="+_to_string(tiFile<char,TextFileInput_decoder_utf8>::invalid_line)+")";
				break;
			}
			currentResult.failed=false;
		}while(0);
	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);



	currentResult.failed=true;
	currentResult.test_title="test tiFile::get_current_line() when no file is closed";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		do
		{
			tiFile<char,TextFileInput_decoder_utf8> testable ("TestInputs/E.coli");
			if(false==testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="could not create test conditions";
				break;
			}
			testable.close();
			if(testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="could not create test conditions";
				break;
			}
			if(tiFile<char,TextFileInput_decoder_utf8>::invalid_line!=testable.get_current_line())
			{
				currentResult.failed=true;
				currentResult.error_messages="get_current_line should return tiFile::invalid_line when no file is opened\n Returned:"+_to_string(testable.get_current_line());
				currentResult.error_messages+="\n(tiFile::invalid_line ="+_to_string(tiFile<char,TextFileInput_decoder_utf8>::invalid_line)+")";

				break;
			}
			currentResult.failed=false;
		}while(0);
	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);



	currentResult.failed=true;
	currentResult.test_title="test tiFile::get_current_line() when file is opened";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		do
		{
			tiFile<char,TextFileInput_decoder_utf8> testable ("TestInputs/E.coli");
			if(false==testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="could not create test conditions";
				break;
			}
			if(1!=testable.get_current_line())
			{
				currentResult.failed=true;
				currentResult.error_messages="get_current_line should return 1 when no file is opened and no read/seek has been performed\n Returned:"+_to_string(testable.get_current_line());
				currentResult.error_messages+="\n(tiFile::invalid_line ="+_to_string(tiFile<char,TextFileInput_decoder_utf8>::invalid_line)+")";

				break;
			}
			currentResult.failed=false;
		}while(0);
	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);



//--------------------------------------------------------------- get current column ------------------------------------------------------------------------------------
	currentResult.failed=true;
	currentResult.test_title="test tiFile::get_current_column() when no file is opened";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		do
		{
			tiFile<char,TextFileInput_decoder_utf8> testable ;
			if(testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="could not create test conditions";
				break;
			}
			if(tiFile<char,TextFileInput_decoder_utf8>::invalid_column!=testable.get_current_column())
			{
				currentResult.failed=true;
				currentResult.error_messages="get_current_column should return tiFile::invalid_column when no file is opened;\n Returned:"+_to_string(testable.get_current_column());
				currentResult.error_messages+="\n(tiFile::invalid_column ="+_to_string(tiFile<char,TextFileInput_decoder_utf8>::invalid_column)+")";
				break;
			}
			currentResult.failed=false;
		}while(0);
	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);



	currentResult.failed=true;
	currentResult.test_title="test tiFile::get_current_column() when file is opened and closed";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		do
		{
			tiFile<char,TextFileInput_decoder_utf8> testable ("TestInputs/E.coli");
			if(false==testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="could not create test conditions";
				break;
			}
			testable.close();
			if(testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="could not create test conditions";
				break;
			}
			if(tiFile<char,TextFileInput_decoder_utf8>::invalid_line!=testable.get_current_column())
			{
				currentResult.failed=true;
				currentResult.error_messages="get_current_column should return tiFile::invalid_column when no file is opened\n Returned:"+_to_string(testable.get_current_column());
				currentResult.error_messages+="\n(tiFile::invalid_column ="+_to_string(tiFile<char,TextFileInput_decoder_utf8>::invalid_column)+")";

				break;
			}
			currentResult.failed=false;
		}while(0);
	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);



	currentResult.failed=true;
	currentResult.test_title="test tiFile::get_current_column() when file is opened";
	currentResult.error_messages="";
	currentResult.exception_string="";
	start_time=std::chrono::system_clock::now();
	try
	{
		do
		{
			tiFile<char,TextFileInput_decoder_utf8> testable ("TestInputs/E.coli");
			if(false==testable.is_open())
			{
				currentResult.failed=true;
				currentResult.error_messages="could not create test conditions";
				break;
			}
			if(1!=testable.get_current_line())
			{
				currentResult.failed=true;
				currentResult.error_messages="get_current_column should return 1 when no file is opened and no read/seek has been performed\n Returned:"+_to_string(testable.get_current_column());
				currentResult.error_messages+="\n(tiFile::invalid_column ="+_to_string(tiFile<char,TextFileInput_decoder_utf8>::invalid_line)+")";

				break;
			}
			currentResult.failed=false;
		}while(0);
	}
	catch(std::exception &e)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
		currentResult.exception_string=exception_to_string(e);
	}
	catch(...)
	{
		currentResult.failed=true;
		currentResult.error_messages="did not expect exception";
	}
	currentResult.elapsed_seconds=std::chrono::system_clock::now()-start_time;
	results.push_back(currentResult);











	return results;
}



void test_tiFile(std::ostream &out,bool html=false);
void test_tiFile(std::ostream &out,bool html)
{
	std::vector<testResult> results=test_tiFile_constructors();
	std::vector<testResult> results_partial;

	results_partial=test_tiFile_open_close();
	results.insert(results.end(),results_partial.begin(),results_partial.end());

	results_partial=test_tiFile_positioning();
	results.insert(results.end(),results_partial.begin(),results_partial.end());




	for(testResult & result : results)
	{
		if(html)
		{
			out<<"<button type=\"button\" class=\"collapsible\">";
			out<<"<table width=\"100%\"><tr>";
			out<<"<td>"<<result.test_title<<"</td>";
			out<<"<td width=\"50 px\">";
			if(result.failed)
				out<<"<span class=\"failed_badge\">&nbsp;&nbsp;failed&nbsp;&nbsp;</span>";
			else
				out<<"<span class=\"passed_badge\">&nbsp;&nbsp;passed&nbsp;&nbsp;</span>";
			out<<"</td><td width=\"90 px\">";
			out<<result.elapsed_seconds.count()<<" s";
			out<<"</td></tr></table>";
			out<<"</button>";
			out<<"<div class=\"content\">";
			out<<"<table>";
			out<<"<tr><th>reason</th><td><pre>"<<result.error_messages<<"</pre></td></tr>";
			out<<"<tr><th>exceptions caught</th><td><pre>"<<result.exception_string<<"</pre></td></tr>";
			out<<"</table>";
			out<<"</div>";
		}
		else
		{
			out<<" "<<result.test_title<<" : ";
			if(result.failed)
				out <<"failed";
			else
				out<<"passed";
			out<<std::endl;
		}
	}


}
void add_html_beginning(std::ostream &out)
{
	out<<"<!DOCTYPE html>"<<std::endl;
	out<<"<html>"<<std::endl;
	out<<"<head>"<<std::endl;
	out<<"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"<<std::endl;
	out<<"<style>"<<std::endl;
	out<<".collapsible{background-color: #777;color: white;cursor: pointer;padding: 18px;width: 100%;border: none;text-align: left;outline: none;font-size: 15px;}"<<std::endl;
	out<<".active, .collapsible:hover {background-color: #555;}"<<std::endl;
	out<<".content {padding: 0 18px; display: none; overflow: hidden; background-color: #f1f1f1;}"<<std::endl;
	out<<".passed_badge {border-radius:5px;  background-color: #20a12f; color:white;}"<<std::endl;
	out<<".failed_badge {border-radius:5px;  background-color: #e33500; color:white;}"<<std::endl;
	out<<"</style>"<<std::endl;
	out<<"</head>"<<std::endl;
	out<<"<body>"<<std::endl;
	out<<"<h1> Test Report </h1>"<<std::endl;
}
void add_html_ending(std::ostream &out)
{
	out<<"<script>"<<std::endl;
	out<<"var coll = document.getElementsByClassName(\"collapsible\");"<<std::endl;
	out<<"var i;"<<std::endl;
	out<<"for (i = 0; i < coll.length; i++) {\
	      coll[i].addEventListener(\"click\", function() {\
		  this.classList.toggle(\"active\");\
		  var content = this.nextElementSibling;\
		  if (content.style.display === \"block\") {\
		  content.style.display = \"none\";\
		  } else {\
		  content.style.display = \"block\";\
		 }});}"<<std::endl;
	out<<"</script>"<<std::endl;
	out<<"</body>"<<std::endl;
}
int main()
{
	std::ofstream report("test_report.html");
	add_html_beginning(report);
	test_tiFile(report,true);
	add_html_ending(report);

	return 0;
}
