
#pragma once

#ifndef CGI_HPP
# define CGI_HPP

# include "webserv.hpp"

class Request;
class Response;

class Cgi {

public:
	Cgi(Request &req, Response &res);
	Cgi(Cgi const &copy);
	~Cgi();

	Cgi	&operator=(Cgi const &other);

	std::map<std::string, std::string> getCGIEnv() const;


	void setCGIEnv(std::map<std::string, std::string> contentToAdd);

private:
	Cgi();

	int		_pipe[2];
	Request *_request;
	Response *_response;
	std::map<std::string, std::string> _envi;

};

#endif
