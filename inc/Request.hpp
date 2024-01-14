
#pragma once

#include "webserv.hpp"

#define REQUEST "\033[1;37mRequest\033[0m"


class Request
{
    public:
        Request();
        Request(const Request& obj);
        ~Request();

        Request &operator=(Request const &obj);
        
        //          parsing
        void        resetValues();
        bool        fillMethod(std::string& line);
        bool        fillHeader(std::string& request);
        bool        fillBody(std::string&  body);

        //          checkup
        int         checkHttpVersion();

        //          set
        void        setMethod(int method);
        void        setPath(std::string path);
        void        setVersion(std::string version);
        void        setHost(std::string host);
        void        setPort(std::string port);
        void        setUserAgent(std::string userAgent);
        void        setContentType(std::string contentType);
        void        setContentLength(int contentLenght);
        void        setCookies(std::vector<std::string> cookies);
        void        setConnection(std::string connection);
        void        setBody(std::string body);
        void        setReturnStatus(int returnStatus);
    
        //                          get
        int                         getMethod(void) const;
        std::string                 getPath(void) const;
        std::string                 getVersion(void) const;
        std::string                 getHost(void) const;
        std::string                 getPort(void) const;
        std::string                 getUserAgent(void) const;
        std::string                 getContentType(void) const;
        int                         getContentLenght(void) const;
        std::vector<std::string>    getCookies(void) const;
        std::string                 getConnection(void) const;
        std::string                 getBody(void) const;
        int                         getReturnStatus(void) const;

    private:

        int                         _method;
        std::string                 _path;
        std::string                 _version;
        std::string                 _host;          // www.example.com
        std::string                 _port;
        std::string                 _userAgent;     // Mozilla/5.0 (...)... (navigateur du client)
        std::string                 _contentType;
        int                         _contentLenght;
        std::vector<std::string>    _cookies;
        std::string                 _connection;
        std::string                 _body;
        int                         _returnStatus;

        // ServerInfo*                 _server;
        // bool                        _headerCompleted;
        // bool                        _bodyCompleted;


};
std::ostream& operator<<(std::ostream& os, Request& obj);

//      request utils
void	removeBackSlashR(std::string& str);
void    closeFile(int* fd);


