/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancolmen <ancolmen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 12:29:43 by ancolmen          #+#    #+#             */
/*   Updated: 2024/02/07 12:44:33 by ancolmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

#define REQUEST "\033[1;37mRequest\033[0m"

class Request
{
	public:
		Request();
		Request(const Request &obj);
		~Request();

		Request						&operator=(Request const &obj);

		void						resetValues();
		void						fillArgs(std::string str);
		bool						fillMethod(std::string &line);
		bool						fillHeader(std::string &request);
		bool						fillBody(std::string &body);

		int							checkHttpVersion();
		std::string					display_method();

		void						setMethod(int method);
		void						setPath(std::string path);
		void						setVersion(std::string version);
		void						setHost(std::string host);
		void						setPort(std::string port);
		void						setUserAgent(std::string userAgent);
		void						setContentType(std::string contentType);
		void						setContentLength(int contentLength);
		void						setCookies(std::vector<std::string> cookies);
		void						setConnection(std::string connection);
		void						setBody(std::string body);
		void						setReturnStatus(int returnStatus);
		void						setArgs(std::string args);
		void						setTransfertEncoding(std::string args);
		void						setChunkTransf(bool value);
		void						setKeepAlive(bool value);
		void						updateCookie();

		int							getMethod() const;
		std::string					getPath() const;
		std::string					getVersion() const;
		std::string					getHost() const;
		std::string					getPort() const;
		std::string					getUserAgent() const;
		std::string					getContentType() const;
		int							getContentLength() const;
		std::vector<std::string>	getCookies() const;
		std::string					getConnection() const;
		std::string					getBody() const;
		int							getReturnStatus() const;
		std::string					getArgs() const;
		std::string					getTransfertEncoding() const;
		bool						getChunkTransf() const;
		bool						getKeepAlive() const;

	private:
		int							_method;
		std::string					_path;
		std::string					_version;
		std::string					_host;
		std::string					_port;
		std::string					_userAgent;
		std::string					_contentType;
		int							_contentLength;
		std::vector<std::string>	_cookies;
		std::string					_connection;
		bool						_keepAlive;
		std::string					_body;
		int							_returnStatus;
		std::string					_args;
		std::string					_transfertEncoding;
		bool						_chunkTransf;

};

std::ostream						&operator<<(std::ostream &os, Request &obj);

void								removeBackSlashR(std::string &str);
void								closeFile(int *fd);


