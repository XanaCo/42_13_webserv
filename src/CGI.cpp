
#include "../inc/CGI.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Cgi::Cgi(ServerInfo &server, Request &req, Response &resp) {

	this->_server = &server;
	this->_cgiLoc = server.getOneLocation("/CGI");
	this->_request = &req;
	this->_response = &resp;

	if (PRINT)
		std::cout << CGI << "🐥 constructor called" << std::endl;

	return ;
}

Cgi::Cgi(Cgi const &copy) {

	//copie profonde get all
	(void)copy;
	
	if (PRINT)
		std::cout << CGI << "🐥 constructor called" << std::endl;

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
		std::cout << GREEN << "Constructor: Cgi = created " << END_COLOR << std::endl;

	return *this;
}

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

void Cgi::setMethod(int method) {

	if (!method)
		return;
	// HELP HERE!!! :
	// Transformer int method de la requete dans le bon method en string?
	this->_method = "BON METHOD";
	return ;
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

char **Cgi::getCGIargvToExec() const {

	return this->_argvToExec;
}


// ************************************************************************** //
//	METHODS
// ************************************************************************** //

int Cgi::setEnvironment(ServerInfo *server, Request *req) {

	//general:
		this->_envpMap["SERVER_SOFTWARE"] = "Webserv/1.0";
		this->_envpMap["SERVER_NAME"] = server->getServerName();
		this->_envpMap["GATEWAY_INTERFACE"] = "CGI/1.1";

	//request specific:
		this->_envpMap["SERVER_PROTOCOL"] = "HTTP/1.1";
		this->_envpMap["SERVER_PORT"] = server->getListen();
		setMethod(req->getMethod());
		this->_envpMap["REQUEST_METHOD"] = this->_method;
		//PATH_INFO - request->URIPathInfo (extra info about the script directory location, others directories inside)
		this->_envpMap["PATH_INFO"] = "req.getPathInfo()";
		//PATH_TRANSLATED - request->getPath() (path to the script to use)
		this->_envpMap["PATH_TRANSLATED"] = req->getPath();
		//SCRIPT_NAME - virtual path of the script
		this->_envpMap["SCRIPT_NAME"] = this->_cgiLoc->getLPathName();
		//QUERY_STRING - request->URIQuery (string apres character ? in URL)
		this->_envpMap["QUERY_STRING"] = "";
		//REMOTE_HOST - request->_ClientHostName (hostname of the client. Can be "")
		this->_envpMap["REMOTE_HOST"] = req->getHost();
		//REMOTE_ADDR - request->_ClientAddress (IP address of the client)
		this->_envpMap["REMOTE_ADDR"] = "";
		this->_envpMap["AUTH_TYPE"] = "Basic";
		//CONTENT_TYPE - request->getContentType (MIME type of the data sent to the CGI script)
		this->_envpMap["CONTENT_TYPE"] = req->getContentType();
		//CONTENT_LENGTH -request->getContentLength (length in bytes of the request body)
		this->_envpMap["CONTENT_LENGTH"] = req->getContentLenght();

	//client specific
		//needed?- HTTP_ACCEPT - The MIME types which the client will accept, as given by HTTP headers. Other protocols may need to get this information from elsewhere. Each item in this list should be separated by commas as per the HTTP spec. Format: type/subtype, type/subtype
		//needed? - HTTP_ACCEPT_LANGUAGE - Accepted Response Languages Exemple : fr_CA, fr
		//needed?- HTTP_USER_AGENT - The browser the client is using to send the request. General format: software/version library/version. EX: Mozilla/5.0 (compatible; Konqueror/3; Linux)
		//HTTP_COOKIE
		//needed? HTTP_REFERER
	
	//extras
		//REDIRECT_STATUS - "200"
		//UPLOAD_DIR

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
			std::cerr << "Set ENV Failed" << std::endl; //EFFACER
			exit(E_INTERNAL_SERVER);
		}
		
		//setArgvToExec(_request->getScriptType()); // REQUEST tells me if I need to execute php or python?
		setArgvToExec(PY); //TESTER
		
		//closeallfds sockets
		//clean logs?

		// if (execve(_argvToExec[0], _argvToExec, _envpToExec) == -1)
		// {
		// 	std::cerr << "Execve Failed" << std::endl; //EFFACER
		// 	//REVISER SI THROW ERROR OU RETURN (WAITPID?)
		// 	/// _response->setReturnStatus(E_INTERNAL_SERVER);
		// 	exit(1);
		// }
		execve(_argvToExec[0], _argvToExec, _envpToExec);
		// std::cerr << "Execve Failed" << std::endl; //EFFACER
		// std::cout << "Test" << std::endl;
		exit(E_INTERNAL_SERVER);

	}
	_response->setCgiFdRessource(_pipeOut[0]); //copies fd out in parent response?
	
}

void Cgi::setArgvToExec(int type) {

	if (type == PY)
		_argvToExec[0] = const_cast<char *>("/bin/python3.10");
	else if (type == PHP)
		_argvToExec[0] = const_cast<char *>("/usr/bin/php-cgi");
	
	_argvToExec[1] = const_cast<char *>("site/CGI/scriptCGI/");// + _request->getScriptPath()); //SCRIPT TO EXECUTE
	_argvToExec[2] = NULL;

}
