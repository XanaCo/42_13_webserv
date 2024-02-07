/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancolmen <ancolmen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:25:02 by ancolmen          #+#    #+#             */
/*   Updated: 2024/02/07 11:27:11 by ancolmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
AUTHORISED FUNCTIONS
All that respects C++98.
execve, dup, dup2, pipe, strerror, gai_strerror,
errno, dup, dup2, fork, socketpair htons, htonl,
ntohs, ntohl, select, poll, epoll (epoll_create,
epoll_ctl, epoll_wait), kqueue (kqueue, kevent),
socket, accept, listen, send, recv, chdir bind,
connect, getaddrinfo, freeaddrinfo, setsockopt,
getsockname, getprotobyname, fcntl, close, read,
write, waitpid, kill, signal, access, stat,
opendir, readdir and closedir.

ARGUMENTS
Configuration file
*/

#include "../inc/webserv.hpp"

int	main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << RED << "Webserv Error : Too many arguments" << END_COLOR << std::endl;
		return (EXIT_FAILURE); 
	}
	try 
	{
		signal(SIGINT, signalHandler);
		signal(SIGQUIT, signalHandler);
		signal(SIGPIPE, signalHandler);
		
		std::string filePath = (argc == 2? argv[1] : "configFiles/default.conf" );

		FileParser infos(filePath);
		infos.parseFile();
		
		std::vector<ServerInfo> Servers = infos.getAllServers();

		Base start(Servers);
		start.start_servers();
 	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
