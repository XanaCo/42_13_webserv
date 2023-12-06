
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

        Response    &operator=(Response const &obj);

        void    redirection(void);
        bool    readRessource(const char* path, std::stringstream& content);
        void    resetValues(void);

        bool    cgiRead();
        bool    cgiWrite();

        void    setReturnStatus(int returnStatus);
        void    setContent(int content);
        void    setPort(uint16_t port);
        void    setCgiOutput(std::string cgiOutput);
        void    setPort(uint16_t port);
        void    setPort(uint16_t port);
        void    setCgiBytesWritten(long cgiBytesWritten);
        void    setCgiFdRessource(int cgiFdRessource);

        int             getReturnStatus(void) const;
        std::string     getContent(void) const;
        uint16_t        getPort(void) const;
        pid_t           getCgiPid(void) const;
        int             getCgiFd(void) const;
        std::string     getCgiOutput(void) const;
        long            getCgiBytesWritten(void) const;
        int             getCgiFdRessource(void) const;

    private:

        uint16_t    _port;
        std::string _content;
        int         _returnStatus;

        pid_t       _cgiPid;
        int         _cgiFd;
        std::string _cgiOutput;

        long        _cgiBytesWritten;
        int         _cgiFdRessource;
};
std::ostream& operator<<(std::ostream& os, Response const& obj);

#endif
