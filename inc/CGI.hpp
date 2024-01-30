
#pragma once

#ifndef CGI_HPP
# define CGI_HPP

# include "webserv.hpp"

# define CGI	"\033[1;35mCGI\033[0m "

class Request;
class Response;
class Serverinfo;

class Cgi {

public:
	Cgi(ServerInfo &server, Request &req, Response &resp);
	~Cgi();

	// Cgi(Cgi const &copy);
	// Cgi	&operator=(Cgi const &other);
	int setMethod(int method);
	int setEnvironment(ServerInfo *server, Request *req);
	int getPipeOut();
	int getPipeIn();
	ServerInfo *getCGIServer() const;
	Location *getCGILoc() const;
	Request *getCGIRequest();
	Response *getCGIResponse();
	std::map<std::string, std::string> &getenvpMap();
	std::string &getCGIMethod();
	char **getCGIenvpToExec() const;
	//std::map<std::string, std::string> getCGIEnv() const;
	int getTypeScript() const;

	void executeScript();

private:

	int _pipeIn[2];
	int _pipeOut[2];
	
	ServerInfo *_server;
	Location *_cgiLoc;
	Request *_request;
	Response *_response;

	std::map<std::string, std::string> _envpMap;
	std::string _method;
	
	char **_argvToExec;
	char **_envpToExec;
	int _typeScript;
	//int _timestart;

};

#endif
