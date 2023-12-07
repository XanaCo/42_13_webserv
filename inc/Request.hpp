
#pragma once

#ifndef REQUEST_HPP
# define REQUEST_HPP

# define REQUEST "\033[1;37mRequest\033[0m"

# include "webserv.hpp"

class Request
{
    public:
        Request(const std::string str);
        ~Request();

        Request &operator=(Request const &obj)

        void        resetValues(void);
        bool        fillContent(std::string request);

        bool        isCompleted(void) const;

        bool        checkup(void);
        bool	    findHost(std::string host, vector<Server *> servers);
        bool        findRessource();

        bool        readRessource(const std::string& path, std::string& content);
        void        deleteRessource(const std::string& resource);
        void        postRessource(const std::string& resource, const std::string& content);

        void        run(std::string strRequest);

        void        setMethod(int method);
        void        setPath(std::string path);
        void        setHost(std::string host);
        void        setUserAgent(std::string userAgent);
        void        setContentType(std::string contentType);
        void        setContentLenght(std::string contentLenght);
        void        setCookies(std::vector<std::string> cookies);
        void        setConnection(std::string connection);
        void        setBody(std::string body);
    
        int                         getMethod(void) const;
        std::string                 getPath(void) const;
        std::string                 getHost(void) const;
        std::string                 getUserAgent(void) const;
        std::string                 getContentType(void) const;
        int                         getContentLenght(void) const;
        std::vector<std::string>    getCookies(void) const;
        std::string                 getConnection(void) const;
        std::string                 getBody(void) const;

    private:
        Request();

        int                         _method;
        std::string                 _path;
        std::string                 _host;          // www.example.com
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
std::ostream& operator<<(std::ostream& os, Request const& obj);

#endif
