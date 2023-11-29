
#pragma once

#ifndef REQUEST_HPP
# define REQUEST_HPP

# define REQUEST "\033[1;37mRequest\033[0m"

# include "webserv.hpp"

// enum methodes
// {
//     GET,
//     POST,
//     DELETE,
// };

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

        std::vector<std::string>    _accept;
        std::vector<std::string>    _acceptLanguage;
        std::vector<std::string>    _acceptEncoding;
        // autorisation
        std::string                 _contentType;
        int                         _contentLenght;
        std::string                 _referer;
        std::vector<std::string>    _cookies;

        std::string                 _connection;
        std::string                 _cacheControl;


};
std::ostream& operator<<(std::ostream& os, Request const& obj);

#endif
