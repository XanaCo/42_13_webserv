
#include "../inc/FileParser.hpp"

/*::: CONSTRUCTORS :::*/

FileParser::FileParser(std::string const &filePath) : _filePath(filePath) {

	/// set params 

	if (PRINT)
		std::cout << GREEN << "Constructor: FileParser created " << END_COLOR << std::endl;

	return ;
}

/*::: DESTRUCTORS :::*/

FileParser::~FileParser() {

	if (PRINT)
		std::cout << RED << "Destructor: FileParser destroyed " << END_COLOR << std::endl;

	return ;
}

/*::: Operator Overloading ::: */

std::ostream &operator<<(std::ostream &out, FileParser const &other) {
		
	out << "all FileParserInfos here to debug";

	return out;
}

/*::: ACCESSORS :::*/

// all geters here

/*::: MEMBER FUNCTIONS :::*/

// other functions here

/*::: EXCEPTIONS :::*/

const char *FileParser::FileParserError::what(std::string msg) const throw() {

	std::string error = "Webserv Error : " + msg;

	return error.c_str();
}
