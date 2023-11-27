
#pragma once

#ifndef REQUEST_HPP
# define REQUEST_HPP

# define REQUEST "\033[1;37mRequest\033[0m"

enum methodes
{
    GET,
    POST,
    DELETE,
}

class Request
{
    public:
        Request(const std::string str);
        ~Request();

    private:
        Request();

        std::string             _path; // URI
        int                     _method;
        
        std::string             _host;
        std::string             _userAgent;

        vecteur<std::string>    _accept;
        vecteur<std::string>    _acceptLanguage;
        vecteur<std::string>    _acceptEncoding;
        // autorisation
        std::string             _contentType;
        int                     _contentLenght;
        std::string             _referer;
        vecteur<std::string>    _cookies;

        std::string             _connection;
        std::string             _cacheControl;


};
std::ostream& operator<<(std::ostream& os, Request const& obj);

#endif
