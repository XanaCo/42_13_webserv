
#pragma once

#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP

# include "webserv.hpp"
# include "Location.hpp"

class Location;

class ServerInfo {

public:
	ServerInfo();
	~ServerInfo();

	std::string getServerName() const;
	in_port_t getPort() const;
	in_addr_t getHost() const;
	//getRoot
	//getIndex
	//getMax

	void setServerName(std::string name) const;
	void setPort(in_port_t port) const;
	void setHost(in_addr_t host) const;

	class ServerInfoError : public std::exception {
	
		public:
			// ServerInfoError();
			// virtual ~ServerInfoError();
			const char *what() const throw();
	};

private:
	ServerInfo(ServerInfo const &copy);
	ServerInfo	&operator=(ServerInfo const &other);

	std::string					_serverName;
	struct sockaddr_in			_sockAddress;
	in_port_t					_Port; // uint_16
	in_addr_t					_Host; // uint_32
	std::string					_Root; // root path
	std::string					_index; // path to index.html
	unsigned int				_maxClientBody;
	std::vector<Location *>		_location; // class location
	std::map<int, std::string>	_errorPages; // 500 - path to error
	int							_listen;
	char						_allowedMethods;

};

std::ostream &operator<<(std::ostream &out, ServerInfo const &ServerInfo);

#endif
