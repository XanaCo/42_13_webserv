#ifndef BASE_H
# define BASE_H

#include "Client.hpp"
#include "Server.hpp"

enum pollevents {
    pollout = 1,
    pollin = 2
};

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
        void    remove_from_servers(int socket);
        void    remove_from_clients(int socket);
        void    remove_from_poll(int socket);
        void    start_servers(void);
        void    change_poll_event(int socket, int event);
        void    review_poll(void);
        bool    is_a_server(int socket);
        bool    is_a_client(int socket);
        void    handle_new_connection(int serv_sock);
        void    receive_client_data(int client_sock); //Can be a client method
        void*   get_in_addr(struct sockaddr *sa);
        void*   get_in_sockaddr(struct sockaddr *sa);
        Client &   get_cli_from_sock(int client_sock);//Can also be a client method (unsure)
        struct pollfd   *get_poll_from_sock(int client_sock);

};

#endif
