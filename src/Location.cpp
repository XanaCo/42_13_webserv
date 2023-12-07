
#include "../inc/Location.hpp"

/*::: CONSTRUCTORS :::*/

Location::Location(std::string const &path) {

	this->_pathName = path;
	this->_pathRoot = "";
	this->_uploadDir = "";
	this->_index = "";
	this->_return = std::pair<int, std::string>(404, "/");
	this->_autoindex = false;
	this->_allowedMethods = 0;

	if (PRINT)
		std::cout << GREEN << "Constructor: Location created " << END_COLOR << std::endl;

	return ;
}

Location::Location(Location const &copy) {

	if (this != &copy) {
		this->_pathName = copy.getLPathName();
		this->_pathRoot = copy.getLRoot();
		this->_uploadDir = copy.getLUploadDir();
		this->_index = copy.getLIndex();
		this->_cgi = copy.getLCgi();
		this->_return = copy.getLReturn();
		this->_autoindex = copy.getLAutoindex();
		this->_allowedMethods = copy.getLAllowed();
	}

	if (PRINT)
		std::cout << GREEN << "Constructor: Location copy created " << END_COLOR << std::endl;

	return ;
}

/*::: DESTRUCTORS :::*/

Location::~Location() {

	if (PRINT)
		std::cout << RED << "Destructor: Location destroyed " << END_COLOR << std::endl;

	return ;
}

/*::: Operator Overloading :::*/

Location &Location::operator=(Location const &other) {

	if (this != &other) {
		this->_pathName = other.getLPathName();
		this->_pathRoot = other.getLRoot();
		this->_uploadDir = other.getLUploadDir();
		this->_index = other.getLIndex();
		this->_cgi = other.getLCgi();
		this->_return = other.getLReturn();
		this->_autoindex = other.getLAutoindex();
		this->_allowedMethods = other.getLAllowed();
	}

	if (PRINT)
		std::cout << GREEN << "Constructor: Location = created " << END_COLOR << std::endl;

	return *this;
}

std::ostream &operator<<(std::ostream &out, Location const &other) {
		
	out << BLUE << "LOCATION : "
			<< other.getLPathName()
			<< " | Root: "
			<< (other.getLRoot() != "" ? other.getLRoot() : "not specified")
			<< " | Autoindex: "
			<< (other.getLAutoindex() ? "YES" : "NO")
			<< " | Methods: "
			<< (other.getLAllowed()? "defined" : "not defined")
			<< " | Index: "
			<< (other.getLIndex()!= "" ? other.getLIndex() : "not defined")
			<< " | Upload: "
			<< (other.getLUploadDir() != "" ? "other.getLUploadDir" : "not defined")
			<< " | Return: "
			<< other.getLReturn().first
			<< " - "
			<< other.getLReturn().second
			// << " | CGI: "
			// << other.getLCgi()
			<< "\n" << END_COLOR;

	return out;
}

/*::: ACCESSORS :::*/

std::string Location::getLPathName() const {

	return this->_pathName;
}

std::string Location::getLRoot() const {

	return this->_pathRoot;
}

std::string Location::getLUploadDir() const {

	return this->_uploadDir;
}

std::string Location::getLIndex() const {

	return this->_index;
}

std::vector<std::string> Location::getLCgi() const {

	return this->_cgi;
}

std::pair<int, std::string> Location::getLReturn() const {

	return this->_return;
}

bool Location::getLAutoindex() const {

	return this->_autoindex;
}

char Location::getLAllowed() const {

	return this->_allowedMethods;
}

/*::: MEMBER FUNCTIONS :::*/

void Location::setLRoot(std::string Lroot) {

	//CHECK PATH VALID!!!
	this->_pathRoot = Lroot;
}

void Location::setLUploadDir(std::string Lupload) {

	//CHECK PATH VALID!!!
	this->_uploadDir = Lupload;
}

void Location::setLIndex(std::string Lindex) {

	this->_index = Lindex;
}

void Location::setLCgi(std::vector<std::string> Lcgi) {

	this->_cgi = Lcgi; ///
}

void Location::setLReturn(std::string Rerror, std::string Rpath) {

	int error = strToInt(Rerror);

	if (error < 1 || error > 900)
		throw ServerInfo::ServerInfoError("Invalid return error code in location");
	if (*(Rpath.begin()) != '/')
		throw ServerInfo::ServerInfoError("Invalid return path in location");
	//CHECK PATH VALID!!!
	this->_return = std::pair<int, std::string>(error, Rpath);
}

void Location::setLAutoindex(bool status) {

	this->_autoindex = status;
}

void Location::setLAllowed(char methods) {

	this->_allowedMethods = methods;
}
