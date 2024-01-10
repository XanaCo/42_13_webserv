#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "webserv.hpp"

# define CLIENT "\033[1;31mClient\033[0m "

class ServerInfo;
class Request;
class Response;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024 
# endif

# define WANT_TO_RECEIVE_REQ 0
# define RECEIVING_REQ_HEADER 1
# define RECEIVING_REQ_BODY 2
# define REQ_RECEIVED 3
# define WAITING_FOR_RES 4
# define RES_READY_TO_BE_SENT 5
# define SENDING_RES_HEADER 6
# define SENDING_RES_BODY 7
# define UPLOADING_FILE 8
# define RES_SENT 9
# define ERROR_WHILE_SENDING 10


class   Base;

class   Client{

    private :

        int                     _new_socket;
        struct sockaddr_in      _address;
        std::string             _received;
        Request                 *_request;
        Response                *_response;
        int                     _client_status;
        std::string             _header;
        std::string             _body;
        int                     _bytes_received;
        int                     _header_bytes;
        int                     _body_bytes;
        std::vector<ServerInfo>     _servers;
        Base *                  _base;

    public :

        Client(void);
        Client(int socket, struct sockaddr_in *r_address, std::vector<ServerInfo> _servers, Base   *base);
        Client(const Client & rhs);
        ~Client(void);
        Client &   operator=(const Client & rhs);

        // Getter / Setters
        int get_socket(void) const;
        struct sockaddr_in get_addr_struct(void) const;
        std::string get_received(void) const;
        int get_status(void) const;
        int get_bytes_received(void) const;
        Request*   getRequest(void);
        Response*   getResponse(void);

        std::string display_status(void) const;
        void    set_socket(int sock);
        void    set_addr_struct(struct sockaddr_in addr);
        void    set_received(std::string buf);
        void    set_status(int status);
        void    set_bytes_received(int nbytes);
        // void        setReturnStatus(Request request);
        // Request     getReturnStatus(void) const;

        // Function to receive data from a client
        bool            alloc_req_resp(void);
        bool            receive_data(void);
        bool            checkHttpVersion(void);
        void            run(std::vector<ServerInfo> serverList);
        bool            found_header_end(size_t *found) const;
        std::string     curated_header(size_t end);
        void            receive_header_data(char *buffer, int nbytes);
        void            receive_body_data(char *buffer, int nbytes);

        bool    send_all(int s, const char *buf, int *len);                                            
        bool    send_data(void);

};

std::ostream &  operator<<(std::ostream & o, Client const & rhs);  

#endif
