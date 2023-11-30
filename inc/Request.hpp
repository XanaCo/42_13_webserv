
#pragma once

#ifndef REQUEST_HPP
# define REQUEST_HPP

# define REQUEST "\033[1;37mRequest\033[0m"

# include "webserv.hpp"

{
    std::string request;

    std::vector<std::string> lines = splitString(request, '\n');

    int nb_lines = lines.size();
    for (int i = 0; i < size; i++)
    {
        // checker quel est le mot clef
        findContent(lines[i]);
        // fill la bonne variable
    }
}

class Request
{
    public:
        Request(const std::string str);
        ~Request();

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

        int                     _method;    // ok
        std::string             _path;
        std::string             _host;      // www.example.com
        
        std::string             _userAgent; // Mozilla/5.0 (...)...

        // indique ce que le client peut traiter :
        // std::vector<std::string>    _accept;
        // std::vector<std::string>    _acceptLanguage;
        // std::vector<std::string>    _acceptEncoding;
    
        // autorisations
        // std::string                 _autor;

        std::string                 _contentType;
        int                         _contentLenght; // taille du body

        // std::string                 _referer;

        std::vector<std::string>    _cookies;

        std::string                 _connection;
        // std::string                 _cacheControl;

        std::string                 _body;


};
std::ostream& operator<<(std::ostream& os, Request const& obj);

#endif
