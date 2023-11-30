
#include "../inc/FileParser.hpp"

/*::: CONSTRUCTORS :::*/

FileParser::FileParser(std::string const &filePath) : _filePath(filePath) {

	// this->_rawFile;
	// this->_rawServer;
	// this->_allServers;
	this->_nServers = 0;

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
	
	(void)other;
	out << "all FileParserInfos here to debug";

	return out;
}

/*::: ACCESSORS :::*/

std::string const &FileParser::getFilePath() const {

	return this->_filePath;
}

std::vector<std::string> FileParser::getRawFile() const {

	return this->_rawFile;
}

std::vector<std::string> FileParser::getRawServer() const {

	return this->_rawServer;
}

std::vector<ServerInfo *> FileParser::getAllServers() const {

	return this->_allServers;
}

int FileParser::getNServers() const {

	return this->_nServers;
}

/*::: MEMBER FUNCTIONS :::*/

void FileParser::cleanFile(std::string const &path) {

	std::string allContent;
	
	allContent = checkFileValid(path);
	eraseComments(allContent);

	this->_rawFile = cSplitLine(allContent, " \f\t\n\r\v");

}

std::string FileParser::checkFileValid(std::string const &path) {

	struct stat fileStat;
	std::ifstream fileStream;
	std::stringstream allContent;

	if (path.empty() || !path.length())
		throw FileParserError("Invalid Path");
	if (stat(path.c_str(), &fileStat) == -1)
		throw FileParserError("Permission denied or Bad address");
	if (!(fileStat.st_mode & S_IFREG))
		throw FileParserError("Invalid File");
	if (access(path.c_str(), R_OK) == -1)
		throw FileParserError("Not reading rights");
	
	fileStream.open(path.c_str());
	if (fileStream.fail())
		throw FileParserError("Failed to open File");
	
	allContent << fileStream.rdbuf();
	fileStream.close();

	return allContent.str();
}

void FileParser::splitServers(std::vector<std::string> &rawServer) {

	(void)rawServer;
	// // split blocks
	// Search for "server" + "{" (start)
	// Search for "}" + "\0" | "server" (end)
	// Must ignore { } inside
	// pushback each line, stock inside rawServer <vector>

	return;
}

/*::: EXCEPTIONS :::*/

FileParser::FileParserError::FileParserError(std::string errorMsg) throw() : _errorMsg("Webserv Error : " + errorMsg) {}

FileParser::FileParserError::~FileParserError() throw() {}

const char *FileParser::FileParserError::what() const throw() {

	return (_errorMsg.c_str());
}
