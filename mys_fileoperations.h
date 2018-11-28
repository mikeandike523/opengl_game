#pragma once
#include "string"
#include <fstream>
#include <sstream>
namespace mys_fileoperations {
	const char* mysgeoshaderfilepath = "C:\\Users\\Michael Sohnen\\source\\repos\\FreeglutOpenglGame1\\FreeglutOpenglGame1\\mysgeoshader.txt";
	//https://stackoverflow.com/a/19922123/5166365
	std::string readfileintostdstring(const char* filepath) {
		std::ifstream inFile;
		inFile.open(filepath);//open the input file

		std::stringstream strStream;
		strStream << inFile.rdbuf();//read the file
		return strStream.str();//str holds the content of the file
	
	}
}
