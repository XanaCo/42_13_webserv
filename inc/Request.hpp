
#pragma once

#ifndef REQUEST_HPP
# define REQUEST_HPP

# define REQUEST "\033[1;37mRequest\033[0m"

# include "webserv.hpp"

class   ServerInfo;

class Request
{
    public:
        Request();
        Request(const Request& obj);
        ~Request();

        Request &operator=(Request const &obj);

        bool        isCompleted(void) const;
        void        findHost(std::vector<ServerInfo>& servers, ServerInfo &server);
        void        resetValues(void);
        bool        fillContent(std::string request);

        void        setMethod(int method);
        void        setPath(std::string path);
        void        setHost(std::string host);
        void        setPort(std::string port);
        void        setUserAgent(std::string userAgent);
        void        setContentType(std::string contentType);
        void        setContentLength(int contentLenght);
        void        setCookies(std::vector<std::string> cookies);
        void        setConnection(std::string connection);
        void        setBody(std::string body);
        void        setHeaderCompleted(bool headerCompleted);
        void        setBodyCompleted(bool bodyCompleted);
        void        setServer(ServerInfo* server);
    
        int                         getMethod(void) const;
        std::string                 getPath(void) const;
        std::string                 getHost(void) const;
        std::string                 getPort(void) const;
        std::string                 getUserAgent(void) const;
        std::string                 getContentType(void) const;
        int                         getContentLenght(void) const;
        std::vector<std::string>    getCookies(void) const;
        std::string                 getConnection(void) const;
        std::string                 getBody(void) const;
        bool                        getHeaderCompleted(void);
        bool                        getBodyCompleted(void);
        ServerInfo*                 getServer(void);

    private:

        int                         _method;
        std::string                 _path;
        std::string                 _host;          // www.example.com
        std::string                 _port;
        std::string                 _userAgent;     // Mozilla/5.0 (...)... (navigateur du client)
        std::string                 _contentType;
        int                         _contentLenght; // taille du body
        std::vector<std::string>    _cookies;
        std::string                 _connection;
        std::string                 _body;

        ServerInfo*                 _server;

        bool                        _headerCompleted;
        bool                        _bodyCompleted;

        // pas obligatoire a traiter apparement :
        // std::vector<std::string>    _accept;
        // std::vector<std::string>    _acceptLanguage;
        // std::vector<std::string>    _acceptEncoding;
        // autres :
        // std::string                 _autor;
        // std::string                 _referer;
        // std::string                 _cacheControl;

};
std::ostream& operator<<(std::ostream& os, Request& obj);

#endif
