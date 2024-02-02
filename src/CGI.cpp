
#include "../inc/CGI.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Cgi::Cgi(ServerInfo &server, Request &req, Response &resp) {

	this->_server = &server;
	this->_cgiLoc = server.getOneLocation("/CGI");
	this->_request = &req;
	this->_response = &resp;
	this->_typeScript = server.getTypeCgi();
	this->_method = "";

	if (PRINT)
		std::cout << CGI << "🐥 constructor called" << std::endl;

}

Cgi::~Cgi() {

	// if (this->_envpToExec)
	// 	freeCharTab(this->_envpToExec);

	if (PRINT)
		std::cout << CGI << "🗑️  destructor called" << std::endl;

}

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

int Cgi::setMethod(int method) {

	if (method == GET)
		this->_method = "GET";
	else if (method == POST)
		this->_method = "POST";
	else
		return 1;

	return 0;
}

int Cgi::setEnvironment(ServerInfo *server, Request *req) {

	//general:
		this->_envpMap["SERVER_SOFTWARE"] = "Webserv/1.0";
		this->_envpMap["SERVER_NAME"] = server->getServerName();
		this->_envpMap["GATEWAY_INTERFACE"] = "CGI/1.0";

	//request specific:
		this->_envpMap["SERVER_PROTOCOL"] = "HTTP/1.1";
		this->_envpMap["SERVER_PORT"] = server->getListen();
		if (setMethod(req->getMethod()))
			return -1;		
		this->_envpMap["REQUEST_METHOD"] = this->_method;
			//PATH_INFO - request->URIPathInfo (extra info about the script directory location, others directories inside)
		this->_envpMap["PATH_INFO"] = "";
			//PATH_TRANSLATED - request->getPath() (path to the script to use) //reviser
		this->_envpMap["PATH_TRANSLATED"] = this->_cgiLoc->getLPathName();
			//SCRIPT_NAME - virtual path of the script //reviser
		this->_envpMap["SCRIPT_NAME"] = req->getPath();
		this->_envpMap["QUERY_STRING"] = req->getArgs();
		this->_envpMap["REMOTE_HOST"] = req->getHost();
			//REMOTE_ADDR - request->_ClientAddress (IP address of the client)
		this->_envpMap["REMOTE_ADDR"] = "";
		// debat pour garder cette variable juste en dessous
		// this->_envpMap["AUTH_TYPE"] = "Basic";
		this->_envpMap["CONTENT_TYPE"] = req->getContentType();
		this->_envpMap["CONTENT_LENGTH"] = intToStr(req->getContentLength());
		this->_envpMap["CONTENT_BODY"] = req->getBody();
		this->_envpMap["REQUEST_METHOD"] = this->_method;
		
		this->_envpMap["REDIRECT_STATUS"] = "1";

	//client specific
		//HTTP_COOKIE

	this->_envpToExec = mapToCharTab(this->_envpMap);
	if (!this->_envpToExec)
		return -1;
	return 0;
}

int Cgi::getPipeOut() {

	return this->_pipeOut[2];
}

int Cgi::getPipeIn() {

	return this->_pipeIn[2];
}

ServerInfo *Cgi::getCGIServer() const {

	return this->_server;
}

Location *Cgi::getCGILoc() const {

	return this->_cgiLoc;
}

Request *Cgi::getCGIRequest() {

	return this->_request;
}

Response *Cgi::getCGIResponse() {

	return this->_response;
}

std::map<std::string, std::string> &Cgi::getenvpMap() {

	return this->_envpMap;
}

std::string &Cgi::getCGIMethod() {

	return this->_method;
}

char **Cgi::getCGIenvpToExec() const {

	return this->_envpToExec;
}

int		Cgi::getTypeScript() const {
	
	return _typeScript;
}

// ************************************************************************** //
//	METHODS
// ************************************************************************** //

bool Cgi::executeScript() {

	int wstatus;
	signal(SIGALRM, &timeoutHandler);

	if (pipe(this->_pipeOut) == -1)
	{
		_request->setReturnStatus(E_INTERNAL_SERVER);
		return false;
	}

	if (pipe(this->_pipeIn) == -1)
	{
		close(_pipeOut[1]);
		close(_pipeOut[0]);
		_request->setReturnStatus(E_INTERNAL_SERVER);
		return false;
	}

	_response->setCgiPid(fork());
	if (_response->getCgiPid() == -1)
	{
		close(_pipeOut[1]);
		close(_pipeOut[0]);
		close(_pipeIn[1]);
		close(_pipeIn[0]);
		_request->setReturnStatus(E_INTERNAL_SERVER);
		return false;
	}

	if (_response->getCgiPid() == 0)
	{
		close(_pipeIn[1]);
		close(_pipeOut[0]);
		if (dup2(_pipeIn[0], 0) == -1)
		{
			close(_pipeIn[0]);
			close(_pipeOut[1]);
			exit(E_INTERNAL_SERVER);
		}
		close(_pipeIn[0]);
		if (dup2(_pipeOut[1], 1) == -1)
		{
			close(_pipeOut[1]);
			exit(E_INTERNAL_SERVER);
		}
		close(_pipeOut[1]);
		
		if (setEnvironment(this->getCGIServer(), this->getCGIRequest()) == -1)
			exit(E_INTERNAL_SERVER);
		
		char *argvToExec[3];
		std::string tmp = _request->getPath();

		if (_typeScript == PY)
			argvToExec[0] = const_cast<char *>("/bin/python3.10");
		else if (_typeScript  == PHP)
			argvToExec[0] = const_cast<char *>("/bin/php");
		argvToExec[1] = const_cast<char *>(tmp.c_str());
		argvToExec[2] = NULL;

		alarm(1);
		execve(argvToExec[0], argvToExec, _envpToExec);
		freeCharTab(this->_envpToExec);
		exit(E_INTERNAL_SERVER);
	}

	alarm(0);
	waitpid(-1, &wstatus, 0);
	if (wstatus == SIGALRM)
	{
		_request->setReturnStatus(E_INTERNAL_SERVER); 
		return false;
	}

	_response->setCgiFdRessource(_pipeOut[0]);
	return true;
}
