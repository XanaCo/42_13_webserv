
#pragma once

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "webserv.hpp"

class Location {

public:
	Location(std::string const &block);
	Location(Location const &copy);
	~Location();

	Location	&operator=(Location const &other);

	std::string getLPathName() const;
	std::string getLRoot() const;
	std::string getLUploadDir() const;
	std::string getLIndex() const;
	std::vector<std::string> getLCgi() const;
	std::pair<int, std::string> getLReturn() const;
	bool getLAutoindex() const;
	char getLAllowed() const;

	void setLRoot(std::string Lroot);
	void setLUploadDir(std::string Lupload);
	void setLIndex(std::string Lindex);
	void setLCgi(std::vector<std::string> Lcgi);
	void setLReturn(std::string Rerror, std::string Rpath);
	void setLAutoindex(bool status);
	void setLAllowed(char methods);

private:
	Location();

	std::string _pathName;
	std::string _pathRoot;
	std::string _uploadDir;
	std::string _index;
	std::vector<std::string> _cgi;
	std::pair<int, std::string> _return;
	bool _autoindex;
	char _allowedMethods;

};

std::ostream &operator<<(std::ostream &out, Location const &other);

#endif
