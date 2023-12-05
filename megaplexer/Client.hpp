#ifndef CLIENT_HPP
# define CLIENT_HPP

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

// # include <Request.hpp>
// # include <Response.hpp>

# define BUFFER_SIZE 1024

class   Client{

    public :

        int new_socket;

        struct  sockaddr_in address;
        std::string received;

        Client(void);
        Client(int socket, struct sockaddr_in *r_address);
        Client(const Client & rhs);
        ~Client(void);
        Client &   operator=(const Client & rhs);

        // void        setReturnStatus(Request request);
        // Request     getReturnStatus(void) const;


    private :

        // Request     request;
        // Response    response;
};

std::ostream &  operator<<(std::ostream & o, Client const & rhs);  

#endif
