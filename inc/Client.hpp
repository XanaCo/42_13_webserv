#pragma once

#include "webserv.hpp"

#define CLIENT "\033[1;31mClient\033[0m "

class ServerInfo;
class Request;
class Response;

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024 
# endif

typedef enum e_status_c
{
	WANT_TO_RECEIVE_REQ,
	RECEIVING_REQ_HEADER,
    RECEIVED_REQ_HEADER,
	RECEIVING_REQ_BODY,
	REQ_RECEIVED,
	WAITING_FOR_RES,
	RES_READY_TO_BE_SENT,
	SENDING_RES_HEADER,
	SENDING_RES_BODY,
	UPLOADING_FILE,     // en cas d'upload d'un file -> via cgi / fork ou filestream fd monitor
	RES_SENT, 
	ERROR_WHILE_SENDING
}	t_status_c;

// typedef void (Client::*methodFunction)(ServerInfo&) const;

class   Base;

class   Client{

    private :

        int                         _new_socket;
        struct sockaddr_in          _address;
        std::string                 _received;
        Request                     *_request;
        Response                    *_response;
        int                         _client_status;
        std::string                 _header;
        std::string                 _body;
        int                         _bytes_received;
        int                         _header_bytes;
        int                         _body_bytes;
        int                         _max_body_size;
        std::vector<ServerInfo>     _servers;
        Base *                      _base;

        ServerInfo*                 _server;
        int                         _fdRessource;

    public :

        Client(void);
        Client(int socket, struct sockaddr_in *r_address, std::vector<ServerInfo> _servers, Base   *base);
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

        bool            send_all(int s, const char *buf, int *len);                                            
        void            reset_client(void);
        std::string     make_temp_header(void);
        bool            send_data(void);

        //                  routine request / response
        bool                getRes();
        bool                postRes();
        bool                deleteRes();
        bool                executeMethod();
        void                routine();
        ServerInfo*         findServer();

        static std::vector<bool (Client::*)()>& methodFunctions() {
            static std::vector<bool (Client::*)()> methods;
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
        Request*            getRequest(void) const;
        Response*           getResponse(void) const;
        int                 getFdRessource(void) const;
        ServerInfo*         getServer(void) const;

        //                  set
        void                set_socket(int sock);
        void                set_addr_struct(struct sockaddr_in addr);
        void                set_received(std::string buf);
        void                set_status(int status);
        void                set_bytes_received(int nbytes);
        void                setFdRessource(int fd);
        void                setServer(ServerInfo* server);
};
std::ostream &  operator<<(std::ostream & o, Client const & rhs); 
