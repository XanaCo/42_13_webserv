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

    private :

        int _new_socket;
        struct sockaddr_in _address;
        std::string _received;
        // Request     request;
        // Response    response;

    public :

        Client(void);
        Client(int socket, struct sockaddr_in *r_address);
        Client(const Client & rhs);
        ~Client(void);
        Client &   operator=(const Client & rhs);

        // Getter / Setters
        int get_socket(void) const;
        struct sockaddr_in get_addr_struct(void) const;
        std::string get_received(void) const;
        void    set_socket(int sock);
        void    set_addr_struct(struct sockaddr_in addr);
        void    set_received(std::string buf);
        // void        setReturnStatus(Request request);
        // Request     getReturnStatus(void) const;

        // Function to receive data from a client
        bool    receive_data(void);

};

std::ostream &  operator<<(std::ostream & o, Client const & rhs);  

#endif
