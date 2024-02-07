/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancolmen <ancolmen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 12:06:55 by ancolmen          #+#    #+#             */
/*   Updated: 2024/02/07 12:38:19 by ancolmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

#define CLIENT "\033[1;31mClient\033[0m "

class ServerInfo;
class Request;
class Response;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef enum e_chunk_c {

	CHUNK_SIZE,
	CHUNK_DATA,

}	t_chunk_c;

typedef enum e_status_c {

	WANT_TO_RECEIVE_REQ,
	RECEIVING_REQ_HEADER,
	RECEIVED_REQ_HEADER,
	RECEIVING_REQ_BODY,
	REQ_RECEIVED,
	WAITING_FOR_RES,
	RES_READY_TO_BE_SENT,
	SENDING_RES_HEADER,
	SENDING_RES_BODY,
	UPLOADING_FILE,
	RES_SENT, 
	ERROR_WHILE_SENDING

}	t_status_c;

class Base;

class Client {

	private :
		int						_new_socket;
		struct sockaddr_in		_address;
		std::string				_received;
		Request					*_request;
		Response				*_response;
		int						_client_status;
		std::string				_header;
		std::string				_body;
		std::string				_to_send;
		std::string				_chunk_pool;
		int						_bytes_received;
		int						_header_bytes;
		int						_body_bytes;
		int						_max_body_size;
		bool					_req_end;
		int						_bytes_to_send;
		int						_bytes_sent;
		bool					_first_chunk;
		int						_timeout;
		int						_chunk_index_type;
		unsigned long			_timestamp;
		std::vector<ServerInfo>	_servers;
		Base					*_base;
		ServerInfo				*_server;
		int						_serv_sock;
		int						_fdRessource;

	public :
		Client();
		Client(int socket, struct sockaddr_in *r_address, std::vector<ServerInfo> _servers, Base *base, int serv_sock);
		Client(const Client &rhs);
		~Client();
		
		Client &operator=(const Client &rhs);

		std::string								display_status() const;

		std::string								generate_directory_listing(std::string &dir_path);

		bool									alloc_req_resp();
		bool									receive_data();
		bool									checkHttpVersion();
		void									run(std::vector<ServerInfo> serverList);
		bool									found_header_end(size_t *found) const;
		std::string								curated_header(size_t end);
		void									receive_header_data(char *buffer, int nbytes);
		void									receive_body_data(char *buffer, int nbytes);
		void									receive_chunked_body_data(char *buffer, int nbytes);

		bool									send_all(int s, const char *buf, int *len);
		bool									send_partial(int socket);
		void									reset_client();
		std::string								make_temp_header();
		bool									send_data();

		bool									parseCgiExit();
		void									openErrorPage();
		bool									getRes();
		bool									postRes();
		bool									deleteRes();
		bool									executeMethod();
		void									routine(int bytes);
		void									findServer();

		static std::vector<bool (Client::*)()>	&methodFunctions();
		
		int										get_socket() const;
		struct sockaddr_in						get_addr_struct() const;
		std::string								get_received() const;
		int										get_status() const;
		int										get_bytes_received() const;
		bool									get_req_end() const;
		Request									*getRequest() const;
		Response								*getResponse() const;
		int										getFdRessource() const;
		int										get_timeout() const;
		unsigned long							get_timestamp() const;
		ServerInfo								*getServer() const;

		void									set_socket(int sock);
		void									set_addr_struct(struct sockaddr_in addr);
		void									set_received(std::string buf);
		void									set_status(int status);
		void									set_bytes_received(int nbytes);
		void									set_req_end(bool end);
		void									set_timeout(int timeout);
		void									set_timestamp(unsigned long timestamp);
		void									setFdRessource(int fd);
		void									setServer(ServerInfo *server);

};

std::ostream &operator<<(std::ostream &o, Client const &rhs);
