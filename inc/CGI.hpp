
#pragma once

#ifndef CGI_HPP
# define CGI_HPP

# include "webserv.hpp"

class Request;
class Response;
class Serverinfo;

class Cgi {

public:
	Cgi(ServerInfo &server, Request &req, Response &resp);
	Cgi(Cgi const &copy);
	~Cgi();

	Cgi	&operator=(Cgi const &other);

	std::map<std::string, std::string> getCGIEnv() const;

	void setRequest(Request &req);
	void setResponse(Request &resp);
	void setMethod(int method);
	int setEnvironment(ServerInfo &server, Request &req);

	int getPipeOut() const;
	int getPipeIn() const;

	ServerInfo &getCGIServer() const;
	Location &getCGILoc() const;
	Request &getCGIRequest() const;
	Response &getCGIResponse() const;

	std::map<std::string, std::string> &getenvpMap() const;
	std::string &getCGIMethod() const;
	char **getCGIenvpToExec() const;
	char **getCGIargvToExec() const;

	class CGIexception : public std::exception {

		public :
			virtual const char *what() const throw();

	};


private:

	int		_pipeIn[2];
	int		_pipeOut[2];
	
	ServerInfo *_server;
	Location *_cgiLoc;
	Request *_request;
	Response *_response;

	std::map<std::string, std::string> _envpMap;
	std::string _method;
	char **_envpToExec;
	char **_argvToExec;

};

#endif
