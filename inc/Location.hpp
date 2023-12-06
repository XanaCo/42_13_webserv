
#pragma once

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "webserv.hpp"

class Location {

public:
	Location(std::string const &block);
	~Location();

	std::string getLPathName() const;
	std::string getLRoot() const;
	std::string getLUploadDir() const;
	std::string getLIndex() const;
	std::vector<std::string> getLCgi() const;
	std::vector<std::string> getLReturn() const;
	bool getLAutoindex() const;
	char getLAllowed() const;

	void setLPathName(std::string Lname);
	void setLRoot(std::string Lroot);
	void setLUploadDir(std::string Lupload);
	void setLIndex(std::string Lindex);
	void setLCgi(std::vector<std::string> Lcgi);
	void setLReturn(std::vector<std::string> Lreturn);
	// void setLAutoindex();
	// void setLAllowed();

private:
	Location();
	Location(Location const &copy);
	Location	&operator=(Location const &other);

	std::string _pathName;
	std::string _pathRoot;
	std::string _uploadDir;
	std::string _index;
	std::vector<std::string> _cgi;
	std::vector<std::string> _return;
	bool _autoindex;
	char _allowedMethods;

};

#endif
