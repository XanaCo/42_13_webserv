/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancolmen <ancolmen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 12:31:44 by ancolmen          #+#    #+#             */
/*   Updated: 2024/02/07 12:54:51 by ancolmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"

# define RESPONSE "\033[1;37mResponse\033[0m"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

class Response
{
	public:
		Response();
		Response(const Response &obj);
		~Response();

		Response	&operator=(const Response &obj);

		void		resetValues();

		int			readRessource(int fd);
		void		deleteRessource(std::string path);
		bool		postRessource(int fd, std::string content);

		bool		craftAutoIndex(std::string path);
		void		setMimeType(std::string path, Base *_base);
		std::string	extract_extension(std::string path);

		void		setContent(std::string content);
		void		setPort(uint16_t port);
		void		setContentType(std::string contentType);
		void		setContentLength(int contentLength);
		void		setCgiPid(pid_t cgiPid);
		void		setCgiFd(int cgiFd);
		void		setCgiOutput(std::string cgiOutput);
		void		setCgiBytesWritten(long cgiBytesWritten);
		void		setCgiFdRessource(int cgiFdRessource);
		void		setStatusCode(std::string statusCode);
		void		setProtocol(std::string protocol);
		void		setLocation(std::string protocol);
		void		setCookies(std::string cookies);

		pid_t		getCgiPid() const;
		int			getCgiFd() const;
		std::string	getCgiOutput() const;
		long		getCgiBytesWritten() const;
		int			getCgiFdRessource() const;
		std::string	getContent() const;
		uint16_t	getPort() const;
		int			getContentLength() const;
		std::string	getContentType() const;
		std::string	getStatusCode() const;
		std::string	getProtocol() const;
		std::string	getLocation() const;
		std::string	getCookies() const;

	private:
		uint16_t	_port;
		int			_contentLength;
		pid_t		_cgiPid;
		int			_cgiFd;
		std::string	_cgiOutput;
		long		_cgiBytesWritten;
		int			_cgiFdRessource;

		std::string	_statusCode;
		std::string	_protocol;
		std::string	_contentType;
		std::string	_content;
		std::string	_location;
		std::string	_cookies;
};

std::ostream		&operator<<(std::ostream &os, const Response &obj);

#endif
