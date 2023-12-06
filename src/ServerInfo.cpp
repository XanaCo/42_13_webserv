
#include "../inc/ServerInfo.hpp"

/*::: CONSTRUCTORS :::*/

ServerInfo::ServerInfo() {

	this->_serverName = "Unknown";
	this->_Port = 0;
	this->_Host = 0;
	this->_Root = "";
	this->_index = "";
	this->_maxClientBody = 0;
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
			<< "\n";

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

void ServerInfo::setSockAddress() {

	this->_sockAddress.sin_port = _Port;
	this->_sockAddress.sin_addr.s_addr = _Host;
	this->_sockAddress.sin_family = AF_INET;
}

void ServerInfo::setPort(std::string port) {

	int res = strToInt(port);

	if (res < 1 || res > 65535)
		throw ServerInfoError("Invald port value");
	
	this->_Port = htons((u_int16_t)res);
}

void ServerInfo::setHost(std::string host) {

	struct sockaddr_in tmp;

	if (!host.compare("localhost"))
		host = "127.0.0.1";
	if (!inet_pton(AF_INET, host.c_str(), &tmp.sin_addr))
		throw ServerInfoError("Wrong IPv4 address format");

	this->_Host = inet_addr(host.c_str());
}

void ServerInfo::setRoot(std::string root) {




	this->_Root = root;
}

void ServerInfo::setIndex(std::string index) {




	this->_index = index;
}

void ServerInfo::setMaxClientBody(std::string max) {

	this->_maxClientBody = strToInt(max); // attention unsigned int
}

size_t ServerInfo::setLocations(std::vector<std::string> &serverTab, size_t pos) {

	size_t it;
	Location locati(serverTab[pos]);
	// bool cgi = false;

	for (it = pos + 1; it < serverTab.size(); it++)
	{
		if (!serverTab[it].compare("location"))
			break;
		else if (!serverTab[it].compare("root"))
		{
			it++;
			std::cout << "L Root : "<< serverTab[it] << std::endl;
			///check root first, then stock
			//if (it < serverTab.size() && semiColonEnding(serverTab[it]))
				//locati.setRoot();
		}
		else if (!serverTab[it].compare("allow_methods"))
		{
			//////3 args or !semicolon check errors
			it++;

			for (size_t p = it; p < serverTab.size(); p++)
			{
				std::cout << "L Methods : "<< serverTab[p] << std::endl;
				///check method first, then stock
				this->_allowedMethods += *(serverTab[p]).c_str();
				if (semiColonEnding(serverTab[p]))
				{
					it = p;
					break;
				}
			}
		}
		else if (!serverTab[it].compare("autoindex"))
		{
			it++;
			std::cout << "L Autoindex : "<< serverTab[it] << std::endl;
			//it < serverTab.size() && semiColonEnding(serverTab[it])
		}
		else if (!serverTab[it].compare("index"))
		{
			it++;
			std::cout << "L index : "<< serverTab[it] << std::endl;
			//it < serverTab.size() && semiColonEnding(serverTab[it])
		}
		else if (!serverTab[it].compare("return"))
		{
			///x2 ARGS
			it++;
			std::cout << "L return : "<< serverTab[it] << std::endl;
			// it < serverTab.size() && semiColonEnding(serverTab[it])
		}
		else if (!serverTab[it].compare("CGI"))
		{
			//PLUSIEURS ARGS while !semicolon
			it++;
			std::cout << "L CGI : "<< serverTab[it] << std::endl;
			//it < serverTab.size() && semiColonEnding(serverTab[it])
		}
		else if (!serverTab[it].compare("upload_dir"))
		{
			it++;
			std::cout << "L upload: "<< serverTab[it] << std::endl;
			//it < serverTab.size() && semiColonEnding(serverTab[it])
		}
		else
		{
			std::cout << "error here: "<< serverTab[it] << std::endl;
			throw ServerInfoError("Unexpected directive in configuration file");
		}
	}
	return it;
}

size_t ServerInfo::setErrorPages(std::vector<std::string> &serverTab, size_t pos) {

	size_t it;

	for (it = pos; it < serverTab.size(); it++)
	{
		std::cout << serverTab[it] << std::endl;
		if (semiColonEnding(serverTab[it]))
		{
			break;
		}
	}
	return it;

	// this->_errorPages = // create pair;
}

void ServerInfo::setListen(std::string port) {

	this->_listen = strToInt(port);
}

void ServerInfo::setTimeout(std::string timeout) {

	int res = strToInt(timeout);

	if (res < 1 || res > 120)
		throw ServerInfoError("Invalid timeout value");

	this->_timeout = res;
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
