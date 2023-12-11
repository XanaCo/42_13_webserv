
#include "../inc/ServerInfo.hpp"

/*::: CONSTRUCTORS :::*/

ServerInfo::ServerInfo() {

    this->_listenSocket = 0;
	this->_serverName = "Unknown";
	this->_Port = 0;
	this->_Host = 0;
	this->_Root = "";
	this->_index = "";
	this->_maxClientBody = 0;
	this->_listen = "";
	this->_timeout = 0;

	if (PRINT)
		std::cout << GREEN << "Default Server was created | Port: " << this->_Port << " | Host: " << this->_Host << END_COLOR << std::endl;

	return ;
}

ServerInfo::ServerInfo(ServerInfo const &copy) {

	if (this != &copy) {
		this->_listenSocket = copy.getSocket();
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
        this->_listenSocket = other.getSocket();
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
        	<< " | Socket: "
			<< other.getSocket()
			<< " | SockAD: "
			<< other.getSockAddress().sin_family
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

int ServerInfo::getSocket() const{

    return this->_listenSocket;
}

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

std::string ServerInfo::getListen() const {

	return this->_listen; 
}

int ServerInfo::getTimeout() const {

	return this->_timeout;
}

/*::: SETERS :::*/


bool    ServerInfo::setListenSocket(std::string l_port){

    int listener;
    int ret_addr;
    int test = 1;
    struct  addrinfo    hints, *ai, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((ret_addr = getaddrinfo(NULL, l_port.c_str(), &hints, &ai)) != 0)
        return (std::cerr << "selectserver : " << gai_strerror(ret_addr) << std::endl, false);
    for (p = ai; p != NULL; p = p->ai_next)
    {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0)
            continue;
        fcntl(listener, F_SETFL, O_NONBLOCK);
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &test, sizeof(int));
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
        {
            close(listener);
            continue;
        }
        break ;
    }
    freeaddrinfo(ai);
    if (!p || listen(listener, 10) == -1)
        return (false);
    return (this->_listenSocket = listener, true);
}

void    ServerInfo::setSameListen(int same){

    this->_listenSocket = same;
}


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
		throw ServerInfoError("Invalid port value");
	
	this->_Port = htons((u_int16_t)res);
	setListen(port);
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

	this->_maxClientBody = strToInt(max);
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
						throw ServerInfoError("GET method is already added");
				}
				else if (!serverTab[p].compare("POST"))
				{
					if (!(!(methods & POST) && (methods += POST)))
						throw ServerInfoError("POST method is already added");
				}
				else if (!serverTab[p].compare("DELETE"))
				{
					if (!(!(methods & DELETE) && (methods += DELETE)))
						throw ServerInfoError("DELETE method is already added");
				}
				else if (!serverTab[p].compare("GET;") && semiColonEnding(serverTab[p]))
				{
					if (!(!(methods & GET) && (methods += GET)))
						throw ServerInfoError("GET method is already added");
					it = p;
					break;
				}
				else if (!serverTab[p].compare("POST;") && semiColonEnding(serverTab[p]))
				{
					if (!(!(methods & POST) && (methods += POST)))
						throw ServerInfoError("POST method is already added");
					it = p;
					break;
				}
				else if (!serverTab[p].compare("DELETE;") && semiColonEnding(serverTab[p]))
				{
					if (!(!(methods & DELETE) && (methods += DELETE)))
						throw ServerInfoError("DELETE method is already added");
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
		else if (!serverTab[it].compare("CGI"))
		{
			it++;
			size_t p;

			for (p = it; p < serverTab.size(); p++)
			{
				if (semiColonEnding(serverTab[p]))
					break;
				locati.setLCgi(serverTab[p]);
			}
			locati.setLCgi(serverTab[p]);
			it = p;
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
		if (semiColonEnding(serverTab[it]))
			break;
		this->_errorPages.push_back(serverTab[it]);
	}
	this->_errorPages.push_back(serverTab[it]);

	return it;
}

void ServerInfo::setListen(std::string port) {

	this->_listen = port;
}

void ServerInfo::setTimeout(std::string timeout) {

	int res = strToInt(timeout);

	if (res < 1 || res > 120)
		throw ServerInfoError("Invalid timeout value");

	this->_timeout = res;
}

void ServerInfo::checkAllInfos() {

	if (this->_Host == 0)
		throw ServerInfoError("Server should have a root directive");
	if (this->_Root == "")
		throw ServerInfoError("Server should have a root directive");
	if (this->_listen == "")
		throw ServerInfoError("Server should have a listen directive");
	if (this->_index == "")
		throw ServerInfoError("Server should have an index directive");
	if (this->_index != "")
	{
		std::string pathToIndex = this->_Root + "/" + this->_index;
		if (!checkFileExists(pathToIndex))
					throw ServerInfo::ServerInfoError("Invalid index path_to_file format");
	}
	if (this->_maxClientBody == 0)
		throw ServerInfoError("Server should have a client_max_size_body directive");
	if (!this->getLocations().empty())
	{
		for (size_t it = 0; it < this->getLocations().size(); it++)
			this->getLocations()[it].checkLocationInfo();
	}
	if (!this->getErrorPages().empty())
	{
		for (size_t it = 0; it < this->getErrorPages().size(); it++)
		{
			if (it % 2 == 0)
			{
				if (isdigit(strToInt(this->getErrorPages()[it])))
					throw ServerInfo::ServerInfoError("Invalid error page code format");
			}
			else
			{
				std::string pathToError = "site/" + this->getErrorPages()[it];
				if (!checkFileExists(pathToError))
					throw ServerInfo::ServerInfoError("Invalid error page path_to_file format");
			}
		}
	}
	this->setSockAddress();
}

/*::: EXCEPTIONS :::*/

ServerInfo::ServerInfoError::ServerInfoError(std::string errorMsg) throw() : _errorMsg("Webserv Error : " + errorMsg) {}

ServerInfo::ServerInfoError::~ServerInfoError() throw() {}

const char *ServerInfo::ServerInfoError::what() const throw() {

	return (_errorMsg.c_str());
}
