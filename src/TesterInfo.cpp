
#include "../inc/TesterInfo.hpp"

/*::: CONSTRUCTORS :::*/

TesterInfo::TesterInfo() {

	this->_serverName = "Tester";
	this->_alive = true;
	this->_Port = 8080;
	this->_Host = 0;
	this->_Root = "/";
	this->_index = "index.html";
	this->_maxClientBody = 3000000;
	this->_allowedMethods += GET + POST + DELETE;
	//this->_sockAddress;  //a remplir
	this->_listen = 5;
	//this->_location; //a remplir
	//this->_errorPages; //a remplir

	if (PRINT)
		std::cout << GREEN << this->_serverName << " was created | Port: " << this->_Port << " | Host: " << this->_Host << END_COLOR << std::endl;

	return ;
}

/*::: DESTRUCTORS :::*/

TesterInfo::~TesterInfo() {

	if (PRINT)
		std::cout << RED << this->_serverName << " was destroyed" << END_COLOR << std::endl;

	return ;
}

