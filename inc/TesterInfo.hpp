
#pragma once

#ifndef TESTERINFO_HPP
# define TESTERINFO_HPP

# include "webserv.hpp"

class TesterInfo {

public:
	TesterInfo();
	~TesterInfo();

private:

	bool						_alive;
	std::string					_serverName;
	//struct sockaddr_in			_sockAddress;
	in_port_t					_Port;
	in_addr_t					_Host;
	std::string					_Root;
	std::string					_index;
	int							_maxClientBody;
	//std::vector<Location *>		_location;
	//std::map<int, std::string>	_errorPages;
	int							_listen;
	char						_allowedMethods;

};

std::ostream &operator<<(std::ostream &out, TesterInfo const &TesterInfo);

#endif
