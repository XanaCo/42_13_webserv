
#include "../inc/CGI.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Cgi::Cgi(Request &req, Response &res) {

	this->_envpMap;
	this->_request = &req;
	this->_response = &res;

	if (PRINT)
		std::cout << GREEN << "Constructor: Cgi created " << END_COLOR << std::endl;

	return ;
}

Cgi::Cgi(Cgi const &copy) {

	if (this != &copy) {
		this->_envpMap = copy.getCGIEnv();
	}

	if (PRINT)
		std::cout << GREEN << "Constructor: Cgi copy created " << END_COLOR << std::endl;

	return ;
}

Cgi::~Cgi() {

	if (PRINT)
		std::cout << RED << "Destructor: Cgi destroyed " << END_COLOR << std::endl;

	return ;
}

// ************************************************************************** //
//  OPERATORS
// ************************************************************************** //

Cgi &Cgi::operator=(Cgi const &other) {

	if (this != &other) {
		this->_envpMap = other.getCGIEnv();
	}

	if (PRINT)
		std::cout << GREEN << "Constructor: Cgi = created " << END_COLOR << std::endl;

	return *this;
}

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

std::map<std::string, std::string>  Cgi::getCGIEnv() const {

	return this->_envpMap;
}


// ************************************************************************** //
//	METHODS
// ************************************************************************** //

//setEnvironment(server, request)
//CGI env variables, set in map:
	//set = for all requests:
		//SERVER_SOFTWARE - "Webserv/1.0" Format: name/version
		//SERVER_NAME - server->_serverName
		//GATEWAY_INTERFACE - "CGI/1.1" Format: CGI/revision
	//request specific:
		//SERVER_PROTOCOL - "HTTP/1.1" Format: protocol/revision
		//SERVER_PORT - intToString(server->_Port).c_str()
		//REQUEST_METHOD - server->getMethods().c_str()
		//PATH_INFO - request->URIPathInfo (extra info about the script directory location)
		//PATH_TRANSLATED - request->URIPath (path to the script to use)
		//SCRIPT_NAME - virtual path of the script
		//QUERY_STRING - request->URIQuery (string apres character ? in URL)
		
		//REMOTE_HOST - request->_ClientHostName (hostname of the client. Can be "")
		//REMOTE_ADDR - request->_ClientAddress (IP address of the client)
		//AUTH_TYPE - "Basic"

		//CONTENT_TYPE - request->_contentType (MIME type of the data sent to the CGI script)
		//CONTENT_LENGTH -request->_contentLength (length in bytes of the request body)

		//a voir si besoin- HTTP_ACCEPT - The MIME types which the client will accept, as given by HTTP headers. Other protocols may need to get this information from elsewhere. Each item in this list should be separated by commas as per the HTTP spec. Format: type/subtype, type/subtype
		// a voir si besoin- HTTP_USER_AGENT - The browser the client is using to send the request. General format: software/version library/version.
	
	//extras
		//HTTP_COOKIE
		//UPLOAD_DIR

//copy envMap (std::map) into envChar (char**), to send to execve

//executeScript
	//pipe(_pipe)
	//response->setCGIpipe(_pipe[0])
	//response->setCGIpid(fork())  ->// response->_pid = fork()
	//if response->_pid == 0 we are in child process
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
