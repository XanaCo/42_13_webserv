
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

        std::string     _path; // URI
        int             _method;


};
std::ostream& operator<<(std::ostream& os, Request const& obj);

#endif
