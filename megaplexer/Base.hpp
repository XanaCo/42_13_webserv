#ifndef BASE_H
# define BASE_H

#include "Client.hpp"
#include "Server.hpp"

class   Base{

    public :
         

        std::vector<Server> servers;
        std::vector<Client> clients;
        std::vector<struct pollfd>  pfds;
        int sock_count;

        Base(void);
        Base(int argc, char **argv);
        Base(const Base & rhs);
        ~Base(void);
        Base &  operator=(const Base & rhs);

        void    add_to_servers(char *port);
        void    add_to_poll_in(int socket);
        void    add_to_poll_out(int socket);

};

#endif
