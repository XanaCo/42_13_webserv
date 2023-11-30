
#pragma once

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"

# define RESPONSE "\033[1;37mResponse\033[0m"

# define BODY_SIZE_LIMIT 40960
# define BUFFER_SIZE 4096

class Response
{
    public:

        Response(uint16_t port);
        ~Response();

        void        recieve();

        void    setReturnStatus(int returnStatus);
        void    setContent(int content);

        int            getReturnStatus(void) const;
        std::string    getContent(void) const;

    private:

        Response();

        uint16_t    _port;
        std::string _content;
        int         _returnStatus;

};
std::ostream& operator<<(std::ostream& os, Response const& obj);

#endif
