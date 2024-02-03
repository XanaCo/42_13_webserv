#ifndef BASE_H
# define BASE_H

# include "webserv.hpp"

# define BASE "\033[1;34mBase\033[0m "

enum pollevents {
    pollout = 1,
    pollin = 2
};

class  Client;

class   Base{

    private :

        std::vector<ServerInfo> _servers;
        std::vector<Client *> _clients;
        std::vector<struct pollfd>  _pfds;
        int _sock_count;
        std::map<std::string, std::string> _mime_types;

    public :

        Base(void);
        Base(int argc, char **argv);
        Base(std::vector<ServerInfo> &  Servers);
        Base(const Base & rhs);
        ~Base(void);
        Base &  operator=(const Base & rhs);

        void    init_mime_types(void);

        // Add to structures 
        void    add_to_servers(char *port);
        void    add_to_clients(int socket, struct sockaddr_in* address, std::vector<ServerInfo> servers, int serv_sock);
        void    add_to_poll_in(int socket);
        void    add_to_poll_out(int socket);

        // Remove from structures 
        void    remove_from_servers(int socket);
        void    remove_from_clients(int socket);
        void    remove_from_poll(int socket);

        //Receive connections and change poll events
        bool    is_first_server(int i, std::string port);
        void    assign_socket_same_port(ServerInfo & curr, ServerInfo & same);
        ServerInfo &    get_first_server(const ServerInfo & curr);
        bool    set_servers_sockets(void);
        void    handle_new_connection(int serv_sock);
        void    change_poll_event(int socket, int event);
        void    receive_client_data(int client_sock); //Can be a client method
        bool    send_all(int s, const char *buf, int *len);

        //Return sockaddr_in or sockaddr_in->addr
        void*   get_in_addr(struct sockaddr *sa);
        void*   get_in_sockaddr(struct sockaddr *sa);

        //Identify the type of socket
        bool    is_a_server(int socket);
        bool    is_a_client(int socket);

        //Getters of objects ref in each struct
        Client &   get_cli_from_sock(int client_sock);
        ServerInfo &   get_serv_from_sock(int client_sock);
        struct pollfd   *get_poll_from_sock(int client_sock);
		std::map<std::string, std::string> &getMimeType(void);

        // Main loop after server creation
        void    start_servers(void);
        void    review_poll(void);

};

#endif
