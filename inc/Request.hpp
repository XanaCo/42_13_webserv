
#pragma once

#ifndef REQUEST_HPP
# define REQUEST_HPP

# define REQUEST "\033[1;37mRequest\033[0m"

# include "webserv.hpp"



int findContent(std::string line)
{
    int size 
    if (line.substr(0, 5) == "Host:")
        return (HOST);
    if (line.substr(0, 11) == "User-Agent:")
        return (USER_AGENT);
    if (line.substr(0, 7) == "Accept:")
        return (ACCEPT);
    if (line.substr(0, 17) == "Accept-Language:")
        return (ACCEPT_LANGUAGE);
    if (line.substr(0, 16) == "Accept-Encoding:")
        return (ACCEPT_ENCODING);
    if (line.substr(0, 14) == "Autorization:")
        return (AUTOR);
    if (line.substr(0, 13) == "Content-Type:")
        return (CONTENT_TYPE);
    if (line.substr(0, 15) == "Content-Lenght:")
        return (CONTENT_LENGHT);
    if (line.substr(0, 8) == "Referer:")
        return (REFERER);
    if (line.substr(0, 6) == "Cookie:")
        return (COOKIES);
    if (line.substr(0, 11) == "Connection:")
        return (CONNECTION);
    if (line.substr(0, 14) == "Cache-Control:")
        return (CACHECONTROL);
    if (line.substr(0, 1) == "{")
        return (BODY_START);
    return ();
}

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

    private:
        Request();

        int                     _method;    // ok
        std::string             _host;      // www.example.com
        
        std::string             _userAgent; // Mozilla/5.0 (...)...

        // indique ce que le client peut traiter;
        std::vector<std::string>    _accept;
        std::vector<std::string>    _acceptLanguage;
        std::vector<std::string>    _acceptEncoding;
    
        // autorisations
        std::string                 _autor;

        std::string                 _contentType;
        int                         _contentLenght; // taille du body

        std::string                 _referer;

        std::vector<std::string>    _cookies;

        std::string                 _connection;
        std::string                 _cacheControl;

        std::string                 _body;


};
std::ostream& operator<<(std::ostream& os, Request const& obj);

#endif
