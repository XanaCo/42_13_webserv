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
        void    add_to_clients(int socket, struct sockaddr_in* address);
        void    add_to_poll_in(int socket);
        void    add_to_poll_out(int socket);
        void    start_servers(void);
        void    review_poll(void);
        void    check_read_operations(void);
        void    check_send_operations(void);
        bool    is_a_server(int socket);
        void    handle_new_connection(int serv_sock);
        void    receive_client_data(int client_sock);
        void*   get_in_addr(struct sockaddr *sa);
        void*   get_in_sockaddr(struct sockaddr *sa);
        Client &   get_cli_from_sock(int client_sock);

};

#endif
