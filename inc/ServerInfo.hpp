
#pragma once

#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP

# include "webserv.hpp"
# include "Location.hpp"

class Location;

class ServerInfo {

public:
	ServerInfo();
	ServerInfo(ServerInfo const &copy);
	~ServerInfo();

	ServerInfo	&operator=(ServerInfo const &other);

	std::string getServerName() const;
	struct sockaddr_in getSockAddress() const;
	in_port_t getPort() const;
	in_addr_t getHost() const;
	std::string getRoot() const;
	std::string getIndex() const;
	unsigned int getMaxClientBody() const;
	std::vector<Location *> getLocations() const;
	std::map<int, std::string> getErrorPages() const;
	int getListen() const;
	char getAllowed() const;

	void setServerName(std::string name);
	void setSockAddress(struct sockaddr_in sockAd);
	void setPort(in_port_t port);
	void setHost(in_addr_t host);
	void setRoot(std::string name);
	void setIndex(std::string name);
	void setMaxClientBody(unsigned int max);
	void setLocations(std::vector<Location *> loc);
	void setErrorPages(std::map<int, std::string> ePages);
	void setListen(int port) ;
	void setAllowed(char methods);

	class ServerInfoError : public std::exception {
	
		public:
			ServerInfoError(std::string errorMsg) throw();
			~ServerInfoError() throw();

			virtual const char *what() const throw();

		private:
			std::string _errorMsg;
	};

private:

	std::string					_serverName;
	struct sockaddr_in			_sockAddress;
	in_port_t					_Port; // uint_16
	in_addr_t					_Host; // uint_32
	std::string					_Root; // root path
	std::string					_index; // path to index.html
	unsigned int				_maxClientBody;
	std::vector<Location *>		_locations; // class Location with infos
	std::map<int, std::string>	_errorPages; // 500 - path to error
	int							_listen;
	char						_allowedMethods;

};

std::ostream &operator<<(std::ostream &out, ServerInfo const &ServerInfo);

#endif
