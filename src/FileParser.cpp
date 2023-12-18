
#include "../inc/FileParser.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

FileParser::FileParser(std::string const &filePath) : _filePath(filePath) {

	this->_nServers = 0;

	if (PRINT)
		std::cout << FILEPARSER << "🐥 constructor called" << std::endl;

	return ;
}

FileParser::~FileParser() {

	if (PRINT)
		std::cout << FILEPARSER << "🗑️  destructor called" << std::endl;

	return ;
}

// ************************************************************************** //
//  OPERATORS
// ************************************************************************** //

std::ostream &operator<<(std::ostream &out, FileParser const &other) {

	out << "FilePath : "
			<< other.getFilePath()
			<< "\n\nRAWFile : "
			<< returnStringVector(other.getRawFile())
			<< "\n\nRAWServer : "
			<< returnStringVector(other.getRawServer())
			<< "\n\nnServers : "
			<< other.getNServers()
			<< "\n";

	return out;
}

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

std::string const &FileParser::getFilePath() const {

	return this->_filePath;
}

std::vector<std::string> FileParser::getRawFile() const {

	return this->_rawFile;
}

std::vector<std::string> FileParser::getRawServer() const {

	return this->_rawServer;
}

std::vector<ServerInfo> FileParser::getAllServers() const {

	return this->_allServers;
}

int FileParser::getNServers() const {

	return this->_nServers;
}

// ************************************************************************** //
//	METHODS
// ************************************************************************** //

void FileParser::parseFile() {

	std::string allContent;
	
	allContent = checkFileValid();
	eraseComments(allContent);
	this->_rawFile = cSplitLine(allContent, " \f\t\n\r\v");

	if (this->_rawFile.empty())
		throw FileParserError("File is empty");

	splitServers();
	stockServerInfo();

	std::cout << GREEN << " Server Configuration OK" << END_COLOR << std::endl;

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
	if (_filePath.find(".conf", _filePath.size() - 5) == std::string::npos)
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
	size_t end = 0;
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
			it = end;
			if (it + 1 == _rawFile.size())
				throw FileParserError("Wrong Server configuration");
			else if (!_rawFile[it + 1].compare("{"))
			{
				this->_nServers++;
				end = serverEnd(it);
			}
			else
				throw FileParserError("Wrong keyword configuration");
		}
	}
	if (_nServers != checkServ)
		throw FileParserError("Wrong number of servers");
	return;
}

size_t FileParser::serverEnd(size_t pos) {
	
	std::string res;
	bool closed = false;
	bool location = false;
	size_t it;

	for (it = pos + 2; it != _rawFile.size(); it++)
	{
		if (!_rawFile[it].compare("server"))
		{
			this->_rawServer.push_back(res);
			return (it);
		}
		else if (!_rawFile[it].compare("{") && !location)
			location = true;
		else if (!_rawFile[it].compare("}") && location)
			location = false;
		else if (!_rawFile[it].compare("}") && !closed)
			closed = true;
		else if (!_rawFile[it].compare("server") && _rawFile[it + 1].compare("{"))
			throw FileParserError("Wrong keyword configuration");
		else if ((!_rawFile[it].compare("{") && location) || (!_rawFile[it].compare("}") && closed))
			throw FileParserError("Wrong brackets configuration");
		else
			res += _rawFile[it] + " ";
	}
	if (res.empty() || location || !closed)
		throw FileParserError("Wrong Server configuration");
	this->_rawServer.push_back(res);
	return it;
}

ServerInfo	FileParser::stockInfos(std::vector<std::string> serverTab) {
	
	ServerInfo newServer;
	bool location = false;
	bool error_pages = false;

	for (size_t it = 0; it < serverTab.size(); it++)
	{
		if (!serverTab[it].compare("server_name") && !location && !error_pages)
		{
			it++;
			if(it < serverTab.size() && semiColonEnding(serverTab[it]))
			{
				if (newServer.getServerName() != "" && newServer.getServerName() != "Unknown")
					throw FileParserError("Server must have only one server_name directive");
				newServer.setServerName(serverTab[it]);
			}
			else
				throw FileParserError("server_name configuration error");
		}
		else if (!serverTab[it].compare("host") && !location && !error_pages)
		{
			it++;
			if(it < serverTab.size() && semiColonEnding(serverTab[it]))
			{
				if (newServer.getHost())
					throw FileParserError("Server must have only one host directive");
				newServer.setHost(serverTab[it]);
			}
			else
				throw FileParserError("host configuration error");
		}
		else if (!serverTab[it].compare("listen") && !location && !error_pages)
		{
			it++;
			if(it < serverTab.size() && semiColonEnding(serverTab[it]))
			{
				if (newServer.getListen() != "")
					throw FileParserError("Server must have only one listen directive");
				newServer.setPort(serverTab[it]);
			}
			else
				throw FileParserError("listen configuration error");
		}
		else if (!serverTab[it].compare("root") && !location && !error_pages)
		{
			it++;
			if(it < serverTab.size() && semiColonEnding(serverTab[it]))
			{
				if (newServer.getRoot() != "")
					throw FileParserError("Server must have only one root directive");
				newServer.setRoot(serverTab[it]);
			}
			else
				throw FileParserError("root configuration error");
		}
		else if (!serverTab[it].compare("index") && !location && !error_pages)
		{
			it++;
			if(it < serverTab.size() && semiColonEnding(serverTab[it]))
			{
				if (newServer.getIndex() != "")
					throw FileParserError("Server must have only one index directive");
				newServer.setIndex(serverTab[it]);
			}
			else
				throw FileParserError("index configuration error");
		}
		else if (!serverTab[it].compare("client_max_body_size") && !location && !error_pages)
		{
			it++;
			if(it < serverTab.size() && semiColonEnding(serverTab[it]))
			{
				if (newServer.getMaxClientBody())
					throw FileParserError("Server must have only one client_max_body_size directive");
				newServer.setMaxClientBody(serverTab[it]);
			}
			else
				throw FileParserError("client_max_size configuration error");
		}
		else if (!serverTab[it].compare("timeout") && !location && !error_pages)
		{
			it++;
			if(it < serverTab.size() && semiColonEnding(serverTab[it]))
			{
				if (newServer.getTimeout())
					throw FileParserError("Server must have only one timeout directive");
				newServer.setTimeout(serverTab[it]);
			}
			else
				throw FileParserError("timeout configuration error");
		}
		else if (!serverTab[it].compare("error_page") && !location)
		{
			it++;
			error_pages = true;

			if(it < serverTab.size() && !semiColonEnding(serverTab[it]))
			{
				it = newServer.setErrorPages(serverTab, it);
				error_pages = false;
			}
			else
				throw FileParserError("error_page configuration error");
		}
		else if (!serverTab[it].compare("location") && !error_pages)
		{
			it++;
			location = true;
			if(it < serverTab.size() && *(serverTab[it].begin()) == '/')
			{
				it = newServer.setLocations(serverTab, it) - 1;
				location = false;
			}
			else
				throw FileParserError("location configuration error");
		}
		else
			throw FileParserError("Unexpected directive in configuration file");
	}
	newServer.checkAllInfos();
	return newServer;
}

void	FileParser::stockServerInfo() {

	std::vector<std::string> serverSplit;
	int it;

	for (it = 0; it < _nServers; it++)
	{
		serverSplit = cSplitLine(_rawServer[it], " ");
		this->_allServers.push_back(stockInfos(serverSplit));
	}
}

// ************************************************************************** //
//	EXCEPTIONS
// ************************************************************************** //

FileParser::FileParserError::FileParserError(std::string errorMsg) throw() : _errorMsg("Webserv Error : " + errorMsg) {}

FileParser::FileParserError::~FileParserError() throw() {}

const char *FileParser::FileParserError::what() const throw() {

	return (_errorMsg.c_str());
}
