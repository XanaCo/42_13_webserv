
#include "../inc/CGI.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Cgi::Cgi(ServerInfo &server, Request &req, Response &resp) {

	this->_server = &server;
	this->_cgiLoc = server.getOneLocation("/CGI");
	this->_request = &req;
	this->_response = &resp;
	this->_typeScript = identifyFile(_request->getPath());
	this->_method = "";

	if (PRINT)
		std::cout << CGI << "🐥 constructor called" << std::endl;

	return ;
}

Cgi::Cgi(Cgi const &copy) {

	//copie profonde get all
	(void)copy;
	
	if (PRINT)
		std::cout << CGI << "🐥 copy constructor called" << std::endl;

	return ;
}

Cgi::~Cgi() {

	// if (this->_envpToExec)
	// 	freeCharTab(this->_envpToExec);

	if (PRINT)
		std::cout << CGI << "🗑️  destructor called" << std::endl;

	return ;
}

// ************************************************************************** //
//  OPERATORS
// ************************************************************************** //

Cgi &Cgi::operator=(Cgi const &other) {

	//copie profonde get all
	(void)other;

	if (PRINT)
		std::cout << CGI << "🐥 = constructor called" << std::endl;

	return *this;
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

int		Cgi::getTypeScript() const {return _typeScript;}
void	Cgi::setTypeScript(int typeScript) {_typeScript = typeScript;}

// ************************************************************************** //
//	METHODS
// ************************************************************************** //

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

	//client specific
		//HTTP_COOKIE

	this->_envpToExec = mapToCharTab(this->_envpMap);
	if (!this->_envpToExec)
		return -1;
	return 0;
}


void Cgi::executeScript() {

	if (pipe(this->_pipeOut) == -1)
	{
		// std::cerr << "PipeOut could not be created" << std::endl; //EFFACER
		_response->setReturnStatus(E_INTERNAL_SERVER);
		return;
	}

	if (pipe(this->_pipeIn) == -1)
	{
		close(_pipeOut[1]);
		close(_pipeOut[0]);
		// std::cerr << "PipeIn could not be created" << std::endl; //EFFACER
		_response->setReturnStatus(E_INTERNAL_SERVER);
		return;
	}

	// start timer _timestart
	_response->setCgiPid(fork());
	if (_response->getCgiPid() == -1)
	{
		close(_pipeOut[1]);
		close(_pipeOut[0]);
		close(_pipeIn[1]);
		close(_pipeIn[0]);
		// std::cerr << "Pid == -1" << std::endl; //EFFACER
		_response->setReturnStatus(E_INTERNAL_SERVER);
		return;
	}

	if (_response->getCgiPid() == 0) // CHILD
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
			//std::cerr << "Dup Failed" << std::endl; //EFFACER
			exit(E_INTERNAL_SERVER);
		}
		close(_pipeOut[1]);
		
		if (setEnvironment(this->getCGIServer(), this->getCGIRequest()) == -1)
		{
			//std::cerr << "Set ENV Failed" << std::endl; //EFFACER
			exit(E_INTERNAL_SERVER);
		}
		
		char *argvToExec[3];
		
		// REQUEST tells me if I need to execute php or python?
		// if (_request->getScriptType() == PY)
		// 	argvToExec[0] = const_cast<char *>("python3.10");
		// else if (_request->getScriptType() == PHP)
		// 	argvToExec[0] = const_cast<char *>("php-cgi");

		argvToExec[0] = const_cast<char *>("/bin/python3.10"); // TEST a effacer
		
			// REQUEST gives me the script path?
		argvToExec[1] = const_cast<char *>("./site/CGI/scriptCGI/py/sortingHat.py"); //+ _request->getScriptPath(); //SCRIPT TO EXECUTE//
		argvToExec[2] = NULL;

		//check if allsockets are closed

		execve(argvToExec[0], argvToExec, _envpToExec);
		freeCharTab(this->_envpToExec); // check
		exit(E_INTERNAL_SERVER);

	}
	_response->setCgiFdRessource(_pipeOut[0]); //copies fd out in parent response?
}

void Cgi::setArgvToExec(int type)
{

	if (type == PY)
		_argvToExec[0] = const_cast<char *>("python3.10");
	else if (type == PHP)
		_argvToExec[0] = const_cast<char *>("php-cgi");
	
	_argvToExec[1] = const_cast<char *>("site/CGI/scriptCGI/");// + _request->getScriptPath()); //SCRIPT TO EXECUTE
	_argvToExec[2] = NULL;
}
