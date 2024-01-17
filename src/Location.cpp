
#include "../inc/Location.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Location::Location(std::string const &path) {

	this->_pathName = path;
	this->_pathRoot = "";
	this->_uploadDir = "";
	this->_index = "";
	this->_return = std::pair<int, std::string>(404, "/");
	this->_autoindex = false;
	this->_allowedMethods = 0;

	if (PRINT)
		std::cout << LOCATION << "🐥 constructor called" << std::endl;

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
		std::cout << LOCATION << "🐥 constructor called" << std::endl;

	return ;
}

Location::~Location() {

	if (PRINT)
		std::cout << LOCATION << "🗑️  destructor called" << std::endl;

	return ;
}

// ************************************************************************** //
//  OPERATORS
// ************************************************************************** //

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
			<< (other.getLUploadDir() != "" ? other.getLUploadDir() : "not defined")
			<< " | Return: "
			<< other.getLReturn().first
			<< " - "
			<< other.getLReturn().second
			<< "\n" << END_COLOR;

	return out;
}

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

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

void Location::setLRoot(std::string Lroot) {

	if (!checkPathExists(Lroot))
		throw ServerInfo::ServerInfoError("Invalid path in root location");

	this->_pathRoot = Lroot;
}

void Location::setLUploadDir(std::string Lupload) {

	if (!checkPathExists(Lupload))
		throw ServerInfo::ServerInfoError("Invalid path in upload_dir location");
	this->_uploadDir = Lupload;
}

void Location::setLIndex(std::string Lindex) {

	this->_index = Lindex;
}

void Location::setLCgi(std::string Lcgi) {

	this->_cgi.push_back(Lcgi);
}

void Location::setLReturn(std::string Rerror, std::string Rpath) {

	int error = strToInt(Rerror);

	if (error < 100 || error > 504)
		throw ServerInfo::ServerInfoError("Invalid return error code in location");
	if (*(Rpath.begin()) != '/')
		throw ServerInfo::ServerInfoError("Invalid return path in location");
	if (!checkPathExists(Rpath))
		throw ServerInfo::ServerInfoError("Invalid return path in location");

	this->_return = std::pair<int, std::string>(error, Rpath);
}

void Location::setLAutoindex(bool status) {

	this->_autoindex = status;
}

void Location::setLAllowed(char methods) {

	this->_allowedMethods = methods;
}

// ************************************************************************** //
//	METHODS
// ************************************************************************** //

void Location::checkLocationInfo() {

	if (this->_pathName == "")
		throw ServerInfo::ServerInfoError("Location should have a name directive");
	if (this->_pathRoot == "")
		this->setLRoot("site/");
	if (this->_uploadDir == "")
		this->setLUploadDir("site/testFiles/fileFolder");
	if (this->_pathName == "/CGI")
	{
		for (size_t it = 0; it < this->getLCgi().size(); it++)
		{
			if (it % 2 == 0)
			{
				if (this->getLCgi()[it].compare(".py") && this->getLCgi()[it].compare(".php"))
					throw ServerInfo::ServerInfoError("Wrong CGI script format");
			}
			else
			{
				if (!checkFileExists(this->getLCgi()[it]))
					throw ServerInfo::ServerInfoError("Wrong CGI path_to_file format");
			}
		}
	}
	if (this->_index != "")
	{
		std::string pathToIndex = this->_pathRoot + this->_index;
		if (!checkFileExists(pathToIndex))
			throw ServerInfo::ServerInfoError("Invalid index path_to_file format");
	}
	if (this->_return.first < 100 || this->_return.first > 504)
		throw ServerInfo::ServerInfoError("Wrong location code error format");
	if (!this->_return.second.empty() && this->_return.second.compare("/"))
	{
		if (!checkPathExists(this->_return.second))
			throw ServerInfo::ServerInfoError("Wrong location return path format");
	}
}
