#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

class ServerInfo;
class Request;
class Response;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif
# define READ_READY 0
# define HEADER_READING 1
# define BODY_READING 2
# define REQUEST_RECEIVED 3
# define RESPONSE_BUILT 4
# define RESPONSE_SENDING 5
# define RESPONSE_SENT 6

class   Client{

    private :

        int _new_socket;
        struct sockaddr_in _address;
        std::string _received;
        Request     *_request;
        Response   *_response;
        int _client_status;

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
        int get_status(void) const;
        std::string display_status(void) const;
        void    set_socket(int sock);
        void    set_addr_struct(struct sockaddr_in addr);
        void    set_received(std::string buf);
        void    set_status(int status);
        // void        setReturnStatus(Request request);
        // Request     getReturnStatus(void) const;

        // Function to receive data from a client
        bool    receive_data(void);
        void    run(std::string strRequest, std::vector<ServerInfo> serverList);

};

std::ostream &  operator<<(std::ostream & o, Client const & rhs);  

#endif
