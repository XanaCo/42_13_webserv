
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

	if (this->_envpToExec)
		freeCharTab(this->_envpToExec);

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
	// PABLO I NEED HELP HERE! :
	//Transformer ton int method de la requete dans le bon method en string
	this->_method = "BON METHOD";
	return ;
}

	// int getPipeOut() const;
	// int getPipeIn() const;

	// ServerInfo &getCGIServer() const;
	// Location &getCGILoc() const;
	// Request &getCGIRequest() const;
	// Response &getCGIResponse() const;

	// std::map<std::string, std::string> &getenvpMap() const;
	// std::string &getCGIMethod() const;
	// char **getCGIenvpToExec() const;
	// char **getCGIargvToExec() const;


// ************************************************************************** //
//	METHODS
// ************************************************************************** //

int Cgi::setEnvironment(ServerInfo &server, Request &req) {

	//general:
		this->_envpMap["SERVER_SOFTWARE"] = "Webserv/1.0";
		this->_envpMap["SERVER_NAME"] = server.getServerName();
		this->_envpMap["GATEWAY_INTERFACE"] = "CGI/1.1";

	//request specific:
		this->_envpMap["SERVER_PROTOCOL"] = "HTTP/1.1";
		this->_envpMap["SERVER_PORT"] = server.getListen();
		setMethod(req.getMethod());
		this->_envpMap["REQUEST_METHOD"] = this->_method;
		//PATH_INFO - request->URIPathInfo (extra info about the script directory location, others directories inside)
		this->_envpMap["PATH_INFO"] = "req.getPathInfo()";
		//PATH_TRANSLATED - request->getPath() (path to the script to use)
		this->_envpMap["PATH_TRANSLATED"] = req.getPath();
		//SCRIPT_NAME - virtual path of the script
		this->_envpMap["SCRIPT_NAME"] = this->_cgiLoc->getLPathName();
		//QUERY_STRING - request->URIQuery (string apres character ? in URL)
		this->_envpMap["QUERY_STRING"] = "";
		//REMOTE_HOST - request->_ClientHostName (hostname of the client. Can be "")
		this->_envpMap["REMOTE_HOST"] = req.getHost();
		//REMOTE_ADDR - request->_ClientAddress (IP address of the client)
		this->_envpMap["REMOTE_ADDR"] = "";
		
		this->_envpMap["AUTH_TYPE"] = "Basic";
		//CONTENT_TYPE - request->getContentType (MIME type of the data sent to the CGI script)
		this->_envpMap["CONTENT_TYPE"] = req.getContentType();
		//CONTENT_LENGTH -request->getContentLength (length in bytes of the request body)
		this->_envpMap["CONTENT_LENGTH"] = req.getContentLenght();

	//client specific
		//needed?- HTTP_ACCEPT - The MIME types which the client will accept, as given by HTTP headers. Other protocols may need to get this information from elsewhere. Each item in this list should be separated by commas as per the HTTP spec. Format: type/subtype, type/subtype
		//needed? - HTTP_ACCEPT_LANGUAGE - Accepted Response Languages Exemple : fr_CA, fr
		//needed?- HTTP_USER_AGENT - The browser the client is using to send the request. General format: software/version library/version. EX: Mozilla/5.0 (compatible; Konqueror/3; Linux)
		//HTTP_COOKIE
		//needed? HTTP_REFERER
	
	//extras
		//REDIRECT_STATUS - "200"
		//UPLOAD_DIR

//copy envMap (std::map) into envChar (char**), to send to execve
	this->_envpToExec = mapToCharTab(this->_envpMap);
	if (!this->_envpToExec)
		return 42;
	return 0;
}


void executeScript() {
	return;
}
	//pipe(_pipe)
	//response->setCGIpipe(_pipe[0])
	//response->setCgiPid(fork())  ->// response->_pid = fork()
	//if response->getCgiPid() == 0 we are in child process
		//dup2 for input
		//dup2 for output
		//setEnvi
		//setupCmdTab : cmd[3]; cmd[0] = python3 || php-cgi; cmd[1] = ./ + URIpath; cmd[2] = NULL;
		//setUpPathtoExec (php or py)
		//closeallsockets / fds
		//////il faut faire execve(const char *filename, char *const argv[], char *const envp[])-->
		//execve(pathToExec.c_str(), cmd, _envi)
			//if execve == -1 
			// free memory
			// throw error (close fd input of dup2 if method is POST)
	//else close pipe[1]
