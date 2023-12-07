
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
	int getTimeout() const;
	char getAllowed() const;
    int get_socket(void) const;
    std::string get_l_port(void) const;

    bool set_listen_socket(std::string l_port);
	void setServerName(std::string name);
	void setSockAddress();
	void setPort(std::string port);
	void setHost(std::string host);
	void setRoot(std::string root);
	void setIndex(std::string index);
	void setMaxClientBody(std::string max);
	void setListen(std::string port);
	void setTimeout(std::string timeout);
	void setAllowed(char methods);
	size_t setErrorPages(std::vector<std::string> &serverTab, size_t pos);
	size_t setLocations(std::vector<std::string> &serverTab, size_t pos);

	class ServerInfoError : public std::exception {
	
		public:
			ServerInfoError(std::string errorMsg) throw();
			~ServerInfoError() throw();

			virtual const char *what() const throw();

		private:
			std::string _errorMsg;
	};

private:

    int                         _listen_socket;
    std::string                 _l_port;
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
	int							_timeout;
	char						_allowedMethods;

};

std::ostream &operator<<(std::ostream &out, ServerInfo const &ServerInfo);

#endif
