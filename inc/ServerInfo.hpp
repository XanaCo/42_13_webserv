/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancolmen <ancolmen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:57:40 by ancolmen          #+#    #+#             */
/*   Updated: 2024/02/07 12:40:38 by ancolmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	ServerInfo					&operator=(ServerInfo const &other);

	int							getSocket() const;
	std::string					getServerName() const;
	struct sockaddr_in			getSockAddress() const;
	in_port_t					getPort() const;
	in_addr_t					getHost() const;
	std::string					getRoot() const;
	std::string					getIndex() const;
	unsigned int				getMaxClientBody() const;
	std::vector<Location>		getLocations() const;
	std::vector<std::string>	getErrorPages() const;
	std::string					getListen() const;
	int							getTimeout() const;
	Location					*getOneLocation(std::string Locname);
	int							getTypeCgi() const;

    bool						setListenSocket(std::string l_port);
    void						setSameListen(int same);
	void						setServerName(std::string name);
	void						setSockAddress();
	void						setPort(std::string port);
	void						setHost(std::string host);
	void						setRoot(std::string root);
	void						setIndex(std::string index);
	void						setMaxClientBody(std::string max);
	void						setListen(std::string port);
	void						setTimeout(std::string timeout);
	size_t						setErrorPages(std::vector<std::string> &serverTab, size_t pos);
	size_t						setLocations(std::vector<std::string> &serverTab, size_t pos);
	void						setTypeCgi(int typeCgi);

	std::string					getNameFileS(std::string path, Location const & loc) const;
	std::string					getNameFileC(std::string path, Location const & loc) const;

	void						checkAllInfos();

	bool						findCgiRessource(std::string path, std::string& newPath);
	int							findRessource(std::string path, std::string& newPath, int method) const;
	int							findRessource_post(std::string path, std::string& newPath, int method) const;
	int							findRessource_delete(std::string path, std::string& newPath, int method) const;

	class ServerInfoError : public std::exception {
	
		public:
			ServerInfoError(std::string errorMsg) throw();
			~ServerInfoError() throw();

			virtual const char *what() const throw();

		private:
			std::string _errorMsg;
	};

private:
	int							_listenSocket;
	std::string					_serverName;
	struct sockaddr_in			_sockAddress;
	in_port_t					_Port;
	in_addr_t					_Host;
	std::string					_Root;
	std::string					_index;
	unsigned int				_maxClientBody;
	std::vector<Location>		_locations;
	std::vector<std::string>	_errorPages;
	std::string					_listen;
	int							_timeout;
	int							_typeCgi;
};

std::ostream 					&operator<<(std::ostream &out, ServerInfo const &ServerInfo);

#endif
