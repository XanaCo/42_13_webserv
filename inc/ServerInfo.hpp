
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

	bool getIsAlive() const;
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

	bool						_alive; // needed?
	std::string					_serverName; // par default: "ServerXX" (XX nb serveurs?)
	//sockaddr_in
	in_port_t					_Port; // uint_16
	in_addr_t					_Host; // uint_32
	std::string					_root; // root path
	std::string					_index; // path to index.html
	int							_maxClientBody; // revoir type

	std::map<std::string, Location *>	_location; // revoir type
	std::vector<std::string>	_allowedMethods; // revoir type
	// definir redirection HTTP ??
	// bool listing ??
	// error pages??


};

std::ostream &operator<<(std::ostream &out, ServerInfo const &ServerInfo);

#endif
