
#pragma once

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"

# define RESPONSE "\033[1;37mResponse\033[0m"

# define BODY_SIZE_LIMIT 40960
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

class Response
{
    public:

        Response();
        Response(uint16_t port);
        ~Response();

        void    redirection(void);
        bool    readRessource(const char* path, std::stringstream& content);

        void    setReturnStatus(int returnStatus);
        void    setContent(int content);
        void    setPort(uint16_t port);

        int             getReturnStatus(void) const;
        std::string     getContent(void) const;
        uint16_t        getPort(void) const;


    private:

        uint16_t    _port;
        std::string _content;
        int         _returnStatus;

};
std::ostream& operator<<(std::ostream& os, Response const& obj);

#endif
