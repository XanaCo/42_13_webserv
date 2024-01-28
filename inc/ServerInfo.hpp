
#pragma once

#ifndef SERVERINFO_HPP
# define SERVERINFO_HPP

# include "webserv.hpp"

# define SERVER_INFO "\033[1;38mServerInfo\033[0m"

class Location;

class ServerInfo {

public:
	ServerInfo();
	ServerInfo(ServerInfo const &copy);
	~ServerInfo();

	ServerInfo	&operator=(ServerInfo const &other);

    int getSocket() const;
	std::string getServerName() const;
	struct sockaddr_in getSockAddress() const;
	in_port_t getPort() const;
	in_addr_t getHost() const;
	std::string getRoot() const;
	std::string getIndex() const;
	unsigned int getMaxClientBody() const;
	std::vector<Location> getLocations() const;
	std::vector<std::string> getErrorPages() const;
	std::string getListen() const;
	int getTimeout() const;
	char getAllowed() const;
	Location *getOneLocation(std::string Locname);

    bool setListenSocket(std::string l_port);
    void setSameListen(int same);
	void setServerName(std::string name);
	void setSockAddress();
	void setPort(std::string port);
	void setHost(std::string host);
	void setRoot(std::string root);
	void setIndex(std::string index);
	void setMaxClientBody(std::string max);
	void setListen(std::string port);
	void setTimeout(std::string timeout);
	size_t setErrorPages(std::vector<std::string> &serverTab, size_t pos);
	size_t setLocations(std::vector<std::string> &serverTab, size_t pos);

	std::string	getNameFileS(std::string path) const;

	void checkAllInfos();

	bool	findCgiRessource(std::string path, std::string& newPath) const;
	bool	findRessource(std::string path, std::string& newPath) const;

	class ServerInfoError : public std::exception {
	
		public:
			ServerInfoError(std::string errorMsg) throw();
			~ServerInfoError() throw();

			virtual const char *what() const throw();

		private:
			std::string _errorMsg;
	};

private:

	int							_listenSocket; // Socket/fd listening to new connections
	std::string					_serverName; // HOST name
	struct sockaddr_in			_sockAddress;
	in_port_t					_Port; // uint_16
	in_addr_t					_Host; // uint_32
	std::string					_Root; // root path
	std::string					_index; // path to index.html
	unsigned int				_maxClientBody;
	std::vector<Location>		_locations; // Location INSTANCES with classified info
	std::vector<std::string>	_errorPages; // 500 - path to error
	std::string					_listen; // PORT in string type
	int							_timeout;

};

std::ostream &operator<<(std::ostream &out, ServerInfo const &ServerInfo);

#endif
