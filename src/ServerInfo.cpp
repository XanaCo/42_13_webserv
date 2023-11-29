
#include "../inc/ServerInfo.hpp"

/*::: CONSTRUCTORS :::*/

ServerInfo::ServerInfo() : _serverName("Default") {

	this->_alive = true;
	this->_Port = 0;
	this->_Host = 0;
	this->_Root = "www";
	/// set other params default

	if (PRINT)
		std::cout << GREEN << this->_serverName << " was created | Port: " << this->_Port << " | Host: " << this->_Host << END_COLOR << std::endl;

	return ;
}

/*::: DESTRUCTORS :::*/

ServerInfo::~ServerInfo() {

	if (PRINT)
		std::cout << RED << this->_serverName << " was destroyed" << END_COLOR << std::endl;

	return ;
}

/*::: Operator Overloading ::: */

std::ostream &operator<<(std::ostream &out, ServerInfo const &other) {

	std::string answer;

	(other.getIsAlive())? answer = "YES" : answer = "NO";
		
	out << other.getServerName()
			<< " | Port: "
			<< other.getPort()
			<< " | Host: "
			<< other.getHost()
			<< " | Alive: "
			<< answer
			<< ".";

	return out;
}

/*::: ACCESSORS :::*/

std::string ServerInfo::getServerName() const {

	return this->_serverName;
}

in_port_t ServerInfo::getPort() const {
	
	return this->_Port;
}

in_addr_t ServerInfo::getHost() const {
	
	return this->_Host;
}

bool ServerInfo::getIsAlive() const {
	
	return this->_alive;
}

/*::: MEMBER FUNCTIONS :::*/



/*::: EXCEPTIONS :::*/

const char *ServerInfo::ServerInfoError::what() const throw() {

	return YELLOW "Webserv Error : ServerInfo" END_COLOR;
}
