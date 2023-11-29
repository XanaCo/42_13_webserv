
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

	// getFilePath
	// getNServers
	// getServerMap

	class FileParserError : public std::exception {
	
		public:
			// FileParserError();
			// virtual ~FileParserError();
			const char *what() const throw();
	};

private:
	FileParser();
	FileParser(FileParser const &copy);
	FileParser	&operator=(FileParser const &other);

	std::string const			_filePath;
	int 						_nServers; // counts each time we launch
	std::map<int, ServerInfo *>	_Server;


};

std::ostream &operator<<(std::ostream &out, FileParser const &FileParser);

#endif
