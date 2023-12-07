
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
        this->_listen_socket = other.get_socket();
        this->_l_port = other.get_l_port();
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
	}

	if (PRINT)
		std::cout << GREEN << "Server by = was created | Port: " << this->_Port << " | Host: " << this->_Host << END_COLOR << std::endl;

	return *this;
}

std::ostream &operator<<(std::ostream &out, ServerInfo const &other) {
		
	
	out << WHITE << "SERVER: "
			<< other.getServerName()
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
			<< "\n" << END_COLOR;

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

std::vector<Location> ServerInfo::getLocations() const {

	return this->_locations;
}

std::vector<std::string> ServerInfo::getErrorPages() const {

	return this->_errorPages;
}

int ServerInfo::getListen() const {

	return this->_listen;
}

int ServerInfo::getTimeout() const {

	return this->_timeout;
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

	//CHECK VALID PATH
	this->_Root = root;
}

void ServerInfo::setIndex(std::string index) {

	//CHECK VALID PATH
	this->_index = index;
}

void ServerInfo::setMaxClientBody(std::string max) {

	this->_maxClientBody = strToInt(max); // attention unsigned int
}

size_t ServerInfo::setLocations(std::vector<std::string> &serverTab, size_t pos) {

	Location locati(serverTab[pos]);
	size_t it;

	for (it = pos + 1; it < serverTab.size(); it++)
	{
		if (!serverTab[it].compare("location"))
			break;
		else if (!serverTab[it].compare("root"))
		{
			it++;
			if (it < serverTab.size() && semiColonEnding(serverTab[it]))
				locati.setLRoot(serverTab[it]);
			else
				throw ServerInfoError("Invalid root declaration in location");
		}
		else if (!serverTab[it].compare("allow_methods"))
		{
			char methods = 0;
			it++;
			for (size_t p = it; p < serverTab.size(); p++)
			{
				if (!serverTab[p].compare("GET"))
				{	
					if (!(!(methods & GET) && (methods += GET)))
						throw ServerInfoError("The weird test of Pablo failed");
				}
				else if (!serverTab[p].compare("POST"))
				{
					if (!(!(methods & POST) && (methods += POST)))
						throw ServerInfoError("The weird test of Pablo failed");
				}
				else if (!serverTab[p].compare("DELETE"))
				{
					if (!(!(methods & DELETE) && (methods += DELETE)))
						throw ServerInfoError("The weird test of Pablo failed");
				}
				else if (!serverTab[p].compare("GET;") && semiColonEnding(serverTab[p]))
				{
					if (!(!(methods & GET) && (methods += GET)))
						throw ServerInfoError("The weird test of Pablo failed");
					it = p;
					break;
				}
				else if (!serverTab[p].compare("POST;") && semiColonEnding(serverTab[p]))
				{
					if (!(!(methods & POST) && (methods += POST)))
						throw ServerInfoError("The weird test of Pablo failed");
					it = p;
					break;
				}
				else if (!serverTab[p].compare("DELETE;") && semiColonEnding(serverTab[p]))
				{
					if (!(!(methods & DELETE) && (methods += DELETE)))
						throw ServerInfoError("The weird test of Pablo failed");
					it = p;
					break;
				}
				else
					throw ServerInfoError("Invalid method declaration in location");
			}
			locati.setLAllowed(methods);
		}
		else if (!serverTab[it].compare("autoindex"))
		{
			it++;
			if (it < serverTab.size() && semiColonEnding(serverTab[it]))
			{
				if (locati.getLAutoindex() != false)
					throw ServerInfoError("Location must have only one autoindex directive");
				if (!serverTab[it].compare("on"))
					locati.setLAutoindex(true);
				else if (!serverTab[it].compare("off"))
					locati.setLAutoindex(false);
				else
					throw ServerInfoError("Invalid autoindex declaration in location");
			}
			else
				throw ServerInfoError("Invalid autoindex declaration in location");
		}
		else if (!serverTab[it].compare("index"))
		{
			it++;
			if (it < serverTab.size() && semiColonEnding(serverTab[it]))
			{
				if (locati.getLIndex() != "")
					throw ServerInfoError("Location must have only one index directive");
				locati.setLIndex(serverTab[it]);
			}
			else
				throw ServerInfoError("Invalid index declaration in location");
		}
		else if (!serverTab[it].compare("return"))
		{
			it += 2;
			if (it < serverTab.size() && semiColonEnding(serverTab[it]))
			{
				if (locati.getLReturn() != std::pair<int, std::string>(404, "/"))
					throw ServerInfoError("Location must have only one return directive");
				locati.setLReturn(serverTab[it - 1], serverTab[it]);
			}
			else
				throw ServerInfoError("Invalid return declaration in location");
		}
		else if (!serverTab[it].compare("CGI")) ///TODO
		{
			it++;
			for (size_t p = it; p < serverTab.size(); p++)
			{
				// if (*(serverTab[it].begin()) == '.py' || *(serverTab[it].begin()) == '.php')
				// 	first
				// else if (*(serverTab[it].begin()) == '/')
				// 	deuxieme
				std::cout << "L CGI : "<< serverTab[p] << std::endl;
				if (semiColonEnding(serverTab[p]))
				{
					break;
				}

				it = p + 1;
			}
		}
		else if (!serverTab[it].compare("upload_dir"))
		{
			it++;
			if (it < serverTab.size() && semiColonEnding(serverTab[it]))
				locati.setLUploadDir(serverTab[it]);
			else
				throw ServerInfoError("Invalid upload_dir declaration in location");
		}
		else
			throw ServerInfoError("Unexpected directive in configuration file");
	}
	this->_locations.push_back(locati);
	return it;
}

size_t ServerInfo::setErrorPages(std::vector<std::string> &serverTab, size_t pos) {

	size_t it;

	for (it = pos; it < serverTab.size(); it++)
	{
		std::cout << serverTab[it] << std::endl;
		this->_errorPages.push_back(serverTab[it]);
		if (semiColonEnding(serverTab[it]))
		{
			this->_errorPages.push_back(serverTab[it]);
			break;
		}
	}

	///CHECK if error are good
	return it;
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

/*::: EXCEPTIONS :::*/

ServerInfo::ServerInfoError::ServerInfoError(std::string errorMsg) throw() : _errorMsg("Webserv Error : " + errorMsg) {}

ServerInfo::ServerInfoError::~ServerInfoError() throw() {}

const char *ServerInfo::ServerInfoError::what() const throw() {

	return (_errorMsg.c_str());
}
