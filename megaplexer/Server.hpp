#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <fcntl.h>
# include <errno.h>
# include <netdb.h>
# include <sys/socket.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <sys/types.h>
# include <dirent.h>
# include <poll.h>
# include <sys/epoll.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <unistd.h>
# include <cstdio>
# include <vector>

class   Server{

    public :

    Server();
    Server(std::string l_port);
    Server(const Server & rhs);
    ~Server();

    Server & operator=(const Server & rhs);

    int listen_socket;
    //int   port;
    std::string   l_port;
    //struct sockaddr_in address; 
    //socklen_t   addrlen;

    bool set_listen_socket(std::string l_port);

    private :

};

std::ostream &  operator<<(std::ostream & o, Server const & rhs);

#endif

