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
	(void)argv;
	if (argc > 2)
	{
		std::cerr << "Webserv Error : Too many arguments" << std::endl;
		return (EXIT_FAILURE); 
	}
	try 
	{
		// signals first thing
		// (argc == 1 valid)
		// check argv[1] here (if it does not exist create it?)
		//FileParser config(argv[1]); // parsing check here, number of servers set
		TesterInfo config; // <- Test a toutes les infos par default!
		//launch SERVER(FileParser.getServers()) // -> multiplexer()?
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
