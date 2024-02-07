/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Base.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancolmen <ancolmen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 12:04:17 by ancolmen          #+#    #+#             */
/*   Updated: 2024/02/07 12:41:05 by ancolmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef BASE_H
# define BASE_H

# include "webserv.hpp"

# define BASE "\033[1;34mBase\033[0m "

enum pollevents {

	pollout = 1,
	pollin = 2

};

class Client;

class Base {

	private :
		std::vector<ServerInfo>				_servers;
		std::vector<Client *>				_clients;
		std::vector<struct pollfd>			_pfds;
		int									_sock_count;
		std::map<std::string, std::string>	_mime_types;

	public :
		Base();
		Base(int argc, char **argv);
		Base(std::vector<ServerInfo> &Servers);
		Base(const Base &rhs);
		~Base();

		Base &operator=(const Base &rhs);

		void								init_mime_types();

		void								add_to_servers(char *port);
		void								add_to_clients(int socket, struct sockaddr_in* address, std::vector<ServerInfo> servers, int serv_sock);
		void								add_to_poll_in(int socket);
		void								add_to_poll_out(int socket);

		void								remove_from_servers(int socket);
		void								remove_from_clients(int socket);
		void								remove_from_poll(int socket);

		bool								is_first_server(int i, std::string port);
		void								assign_socket_same_port(ServerInfo &curr, ServerInfo &same);
		ServerInfo							&get_first_server(const ServerInfo &curr);
		void								handle_new_connection(int serv_sock);
		void								change_poll_event(int socket, int event);
		void								receive_client_data(int client_sock);
		bool								send_all(int s, const char *buf, int *len);

		bool								set_servers_sockets();
		void								*get_in_addr(struct sockaddr *sa);
		void								*get_in_sockaddr(struct sockaddr *sa);
		Client								&get_cli_from_sock(int client_sock);
		ServerInfo							&get_serv_from_sock(int client_sock);
		struct pollfd						*get_poll_from_sock(int client_sock);
		std::map<std::string, std::string>	&getMimeType();

		bool								is_a_server(int socket);
		bool								is_a_client(int socket);

		void								start_servers();
		void								review_poll();
		
		bool								client_is_timed_out(Client &client);

};

#endif
