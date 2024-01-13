#pragma once

#include "webserv.hpp"

#define CLIENT "\033[1;31mClient\033[0m "

class ServerInfo;
class Request;
class Response;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

typedef enum e_status_c
{
	WANT_TO_RECIVE_REQ,
	RECIVING_REQ_HEADER,
    RECIVED_REQ_HEADER,
	RECIVING_REQ_BODY,
	REQ_RECIVED,
	WAITING_FOR_RES,
	RES_READY_TO_BE_SENT,
	SENDING_RES_HEADER,
	SENDING_RES_BODY,
	UPLOADING_FILE,     // en cas d'upload d'un file -> via cgi / fork ou filestream fd monitor
	RES_SENT, 
	ERROR_WHILE_SENDING
}	t_status_c;

typedef void (Client::*methodFunction)(ServerInfo&) const;

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

        ServerInfo*             _server;
        int                     _fdRessource

    public :

        Client(void);
        Client(int socket, struct sockaddr_in *r_address);
        Client(const Client & rhs);
        ~Client(void);
        Client &   operator=(const Client & rhs);

        std::string         display_status(void) const;

        //              Function to receive data from a client
        bool            alloc_req_resp(void);
        bool            receive_data(void);
        bool            checkHttpVersion(void);
        void            run(std::vector<ServerInfo> serverList);
        bool            found_header_end(size_t *found) const;
        std::string     curated_header(size_t end);
        void            receive_header_data(char *buffer, int nbytes);
        void            receive_body_data(char *buffer, int nbytes);



        //                  routine request / response
        void                getRes();
        void                postRes();
        void                deleteRes();
        ServerInfo&         findServer(std::vector<ServerInfo> serverList);

        static const std::vector<void (Client::*)(ServerInfo&)>& methodFunctions()
        {
            static std::vector<void (Client::*)(ServerInfo&)> methods;
            if (methods.empty())
            {
                methods.push_back(&Client::getRes);
                methods.push_back(&Client::postRes);
                methods.push_back(&Client::deleteRes);
            }
            return methods;
        }
        



        //                  get
        int                 get_socket(void) const;
        struct sockaddr_in  get_addr_struct(void) const;
        std::string         get_received(void) const;
        int                 get_status(void) const;
        int                 get_bytes_received(void) const;
        Response*           getResponse(void);
        int                 getFdRessource(void) const;
        ServerInfo*         getServer(void) const;  // utiliser un pointeur ?

        //                  set
        void                set_socket(int sock);
        void                set_addr_struct(struct sockaddr_in addr);
        void                set_received(std::string buf);
        void                set_status(int status);
        void                set_bytes_received(int nbytes);
        void                setFdRessource(int fd);
        void                setServer(ServerInfo* server);

        // void        setReturnStatus(Request request);
        // Request     getReturnStatus(void) const;
};
std::ostream &  operator<<(std::ostream & o, Client const & rhs); 
