
#pragma once

#ifndef FILEPARSER_HPP
# define FILEPARSER_HPP

# include "webserv.hpp"
# include "ServerInfo.hpp"

class ServerInfo;

class FileParser {

public:
	FileParser(std::string const &filePath);
	~FileParser();

	std::string const &getFilePath() const;
	std::vector<std::string> getRawFile() const;
	std::vector<std::string> getRawServer() const;
	std::vector<ServerInfo *> getAllServers() const;
	int getNServers() const;

	void cleanFile();
	std::string checkFileValid();
	void splitServers();
	// ServerInfo *stockServers(std::string const &rawServer)

	class FileParserError : public std::exception {
	
		public:
			FileParserError(std::string errorMsg) throw();
			~FileParserError() throw();

			virtual const char *what() const throw();
		
		private:
			std::string _errorMsg;
	};
	class SignalExit : public std::exception {};

private:
	FileParser();
	FileParser(FileParser const &copy);
	FileParser	&operator=(FileParser const &other);

	std::string const			_filePath;
	std::vector<std::string>	_rawFile;
	std::vector<std::string>	_rawServer;
	std::vector<ServerInfo *>	_allServers; // classified info
	int 						_nServers; // counts each time we stock serverline


};

std::ostream &operator<<(std::ostream &out, FileParser const &FileParser);

#endif
