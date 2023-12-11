
#include "../inc/CGI.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Cgi::Cgi(Request &req, Response &res) {

	this->_envi;
	this->_request = &req;
	this->_response = &res;

	if (PRINT)
		std::cout << GREEN << "Constructor: Cgi created " << END_COLOR << std::endl;

	return ;
}

Cgi::Cgi(Cgi const &copy) {

	if (this != &copy) {
		this->_envi = copy.getCGIEnv();
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
		this->_envi = other.getCGIEnv();
	}

	if (PRINT)
		std::cout << GREEN << "Constructor: Cgi = created " << END_COLOR << std::endl;

	return *this;
}

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

std::map<std::string, std::string>  Cgi::getCGIEnv() const {

	return this->_envi;
}


// ************************************************************************** //
//	METHODS
// ************************************************************************** //


//setEnvi
	//set all vars perso au serv


//execScript
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
			//if execve == -1 throw error (close fd input of dup2 if method is POST)
	//else close pipe[1]
