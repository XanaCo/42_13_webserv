/*
AUTHORISED FUNCTIONS
All that respects C++ 98.
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
		std::cerr << "Webserv Error : Too many arguments" << std::endl;
		return (EXIT_FAILURE); 
	}
	try 
	{
		signal(SIGINT, signalHandler); // Clean exit, no leaks
		signal(SIGQUIT, signalHandler); // Clean exit, no leaks
		signal(SIGPIPE, signalHandler); // Ignore
		
		std::string filePath = (argc == 2? argv[1] : "configFiles/default.conf" );

		FileParser infos(filePath);
		infos.parseFile();
		
		std::vector<ServerInfo> Servers = infos.getAllServers(); //classified Servers
		//printServersInfo(Servers);

		Base    start(Servers);
        start.start_servers();
 	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
