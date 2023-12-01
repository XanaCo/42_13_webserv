
#include "../inc/FileParser.hpp"

/*::: CONSTRUCTORS :::*/

FileParser::FileParser(std::string const &filePath) : _filePath(filePath) {

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

void FileParser::cleanFile() {

	std::string allContent;
	
	allContent = checkFileValid();
	eraseComments(allContent);
	this->_rawFile = cSplitLine(allContent, " \f\t\n\r\v");

	if (this->_rawFile.empty())
		throw FileParserError("File is empty");


}

std::string FileParser::checkFileValid() {

	struct stat fileStat;
	std::ifstream fileStream;
	std::stringstream allContent;
	std::string res;

	if (_filePath.empty() || !_filePath.length())
		throw FileParserError("Invalid path");
	if (stat(_filePath.c_str(), &fileStat) == -1)
		throw FileParserError("Permission denied or Bad address");
	if (!(fileStat.st_mode & S_IFREG))
		throw FileParserError("Invalid file");
	if (access(_filePath.c_str(), R_OK) == -1)
		throw FileParserError("Not reading rights");
	if (_filePath.find(".conf", _filePath.size() - 5) == std::string::npos) // .conf accepted
		throw FileParserError("Wrong configuration file format");
	
	fileStream.open(_filePath.c_str());
	if (fileStream.fail())
		throw FileParserError("Failed to open file");
	
	allContent << fileStream.rdbuf();
	fileStream.close();

	return allContent.str();
}

void FileParser::splitServers() {

	size_t it;
	int checkServ = 0;

	if (_rawFile[0].compare("server") != 0)
			throw FileParserError("Missing server in configuration file");

	for (it = 0; it != _rawFile.size(); it++)
	{
		if (!_rawFile[it].compare("server"))
			checkServ++;
	}

	for (it = 0; it != _rawFile.size(); it++)
	{
		if (!_rawFile[it].compare("server"))
		{
			if (!_rawFile[1 + it].compare("{"))
				it = serverEnd(it);
			else
				throw FileParserError("Wrong keyword configuration");
		}
	}
	// std::cout << _nServers << std::endl;
	// std::cout << checkServ << std::endl;
	// if (_nServers != checkServ)
	// 	throw FileParserError("Wrong number of servers");
	printStringVector(_rawServer);
	return;
}

size_t FileParser::serverEnd(size_t pos) {
	
	std::string res;
	bool closed = false;
	bool location = false;
	size_t it;

	for (it = pos + 2; it != _rawFile.size() - 1; it++)
	{
		if (!_rawFile[it].compare("server") && !_rawFile[it + 1].compare("{") && _nServers > 0)  // we check for next keyword server
			return (it);
		else if (!_rawFile[it].compare("{") && !location) // we open location
			location = true;
		else if (!_rawFile[it].compare("}") && location) // we close location
			location = false;
		else if (!_rawFile[it].compare("}") && !closed) // we close all
			closed = true;
		else if (!_rawFile[it].compare("server") && _rawFile[it + 1].compare("{")) // check double server name
			throw FileParserError("Wrong keyword configuration");
		else if ((!_rawFile[it].compare("{") && location) || (!_rawFile[it].compare("}") && closed))
			throw FileParserError("wrong brackets configuration");
		else
			res += _rawFile[it] + " ";
	}
	this->_nServers++;
	this->_rawServer.push_back(res);
	return it;
}

// Check server config when all is stocked, just in case

/*::: EXCEPTIONS :::*/

FileParser::FileParserError::FileParserError(std::string errorMsg) throw() : _errorMsg("Webserv Error : " + errorMsg) {}

FileParser::FileParserError::~FileParserError() throw() {}

const char *FileParser::FileParserError::what() const throw() {

	return (_errorMsg.c_str());
}
