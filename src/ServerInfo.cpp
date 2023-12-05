
#include "../inc/ServerInfo.hpp"

/*::: CONSTRUCTORS :::*/

ServerInfo::ServerInfo() {

	this->_serverName = "";
	this->_Port = 0;
	this->_Host = 0;
	this->_Root = "";
	this->_index = "";
	//this->_sockAddress;
	this->_maxClientBody = 0;
	//this->_locations;
	//this->_errorPages;
	this->_listen = 0;
	this->_timeout = 0;
	this->_allowedMethods = 0;

	if (PRINT)
		std::cout << GREEN << "Default Server was created | Port: " << this->_Port << " | Host: " << this->_Host << END_COLOR << std::endl;

	return ;
}

ServerInfo::ServerInfo(ServerInfo const &copy) {

	if (this != &copy) {
		this->_serverName = copy.getServerName();
		this->_Port = copy.getPort();
		this->_Host = copy.getHost();
		this->_Root = copy.getRoot();
		this->_index = copy.getIndex();
		this->_sockAddress = copy.getSockAddress();
		this->_maxClientBody = copy.getMaxClientBody();
		this->_locations = copy.getLocations();
		this->_errorPages = copy.getErrorPages();
		this->_listen = copy.getListen();
		this->_timeout = copy.getTimeout();
		this->_allowedMethods = copy.getAllowed();
	}

	if (PRINT)
		std::cout << GREEN << "Server by Copy was created | Port: " << this->_Port << " | Host: " << this->_Host << END_COLOR << std::endl;

	return ;
}


/*::: DESTRUCTORS :::*/

ServerInfo::~ServerInfo() {

	if (PRINT)
		std::cout << RED << this->_serverName << " was destroyed" << END_COLOR << std::endl;

	return ;
}

/*::: Operator Overloading ::: */

ServerInfo	&ServerInfo::operator=(ServerInfo const &other) {

	if (this != &other) {
		this->_serverName = other.getServerName();
		this->_Port = other.getPort();
		this->_Host = other.getHost();
		this->_Root = other.getRoot();
		this->_index = other.getIndex();
		this->_sockAddress = other.getSockAddress();
		this->_maxClientBody = other.getMaxClientBody();
		this->_locations = other.getLocations();
		this->_errorPages = other.getErrorPages();
		this->_listen = other.getListen();
		this->_timeout = other.getTimeout();
		this->_allowedMethods = other.getAllowed();
	}

	if (PRINT)
		std::cout << GREEN << "Server by = was created | Port: " << this->_Port << " | Host: " << this->_Host << END_COLOR << std::endl;

	return *this;
}

std::ostream &operator<<(std::ostream &out, ServerInfo const &other) {
		
	out << other.getServerName()
			<< " | Port: "
			<< other.getPort()
			<< " | Host: "
			<< other.getHost()
			<< " | Root: "
			<< other.getRoot()
			<< " | Index: "
			<< other.getIndex()
			<< " | MaxBody: "
			<< other.getMaxClientBody()
			<< " | Listen: "
			<< other.getListen()
			<< " | Timeout: "
			<< other.getTimeout()
			<< " | Allowed: "
			<< other.getAllowed()
			<< ".\n";

	return out;
}

/*::: ACCESSORS :::*/

std::string ServerInfo::getServerName() const {

	return this->_serverName;
}

struct sockaddr_in ServerInfo::getSockAddress() const {

	return this->_sockAddress;
}

in_port_t ServerInfo::getPort() const {
	
	return this->_Port;
}

in_addr_t ServerInfo::getHost() const {
	
	return this->_Host;
}

std::string ServerInfo::getRoot() const {

	return this->_Root;
}

std::string ServerInfo::getIndex() const {

	return this->_index;
}

unsigned int ServerInfo::getMaxClientBody() const {

	return this->_maxClientBody;
}

std::vector<Location *> ServerInfo::getLocations() const {

	return this->_locations;
}

std::map<int, std::string> ServerInfo::getErrorPages() const {

	return this->_errorPages;
}

int ServerInfo::getListen() const {

	return this->_listen;
}

int ServerInfo::getTimeout() const {

	return this->_timeout;
}

char ServerInfo::getAllowed() const {

	return this->_allowedMethods;
}

/*::: SETERS :::*/

void ServerInfo::setServerName(std::string name) {

	this->_serverName = name;
}

void ServerInfo::setSockAddress(struct sockaddr_in sockAd) {

	this->_sockAddress = sockAd; // attention  copie profonde
}

void ServerInfo::setPort(std::string port) {

	this->_Port = strToInt(port); // attention  in_port_t
}

void ServerInfo::setHost(std::string host) {

	this->_Host = strToInt(host); // attention  in_addr_t
}

void ServerInfo::setRoot(std::string name) {

	this->_Root = name;
}

void ServerInfo::setIndex(std::string name) {

	this->_index = name;
}

void ServerInfo::setMaxClientBody(std::string max) {

	this->_maxClientBody = strToInt(max); // attention unsigned int
}

void ServerInfo::setLocations(std::vector<Location *> loc) {

	this->_locations = loc;  // copie profonde?
}

void ServerInfo::setErrorPages(std::map<int, std::string> ePages) {

	this->_errorPages = ePages;  // copie profonde?
}

void ServerInfo::setListen(std::string port) {

	this->_listen = strToInt(port);
}

void ServerInfo::setTimeout(std::string timeout) {

	this->_timeout = strToInt(timeout);
}

void ServerInfo::setAllowed(char methods) {

	this->_allowedMethods = methods;
}

/*::: EXCEPTIONS :::*/

ServerInfo::ServerInfoError::ServerInfoError(std::string errorMsg) throw() : _errorMsg("Webserv Error : " + errorMsg) {}

ServerInfo::ServerInfoError::~ServerInfoError() throw() {}

const char *ServerInfo::ServerInfoError::what() const throw() {

	return (_errorMsg.c_str());
}
