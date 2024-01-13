
#pragma once

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "webserv.hpp"

# define RESPONSE "\033[1;37mResponse\033[0m"

// # define BODY_SIZE_LIMIT 40960 -> doublon avec un element du parsing

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 50
# endif

class Response
{
    public:

        Response();
        Response(const Response& obj);
        ~Response();

        Response    &operator=(const Response& obj);

        // void    redirection(void);
        void    resetValues(void);

        // bool        readRessource(const std::string& path, std::string& content);
        bool        readRessource(int fd);
        void        deleteRessource(const std::string path);
        void        postRessource(const std::string path, std::string content);

        bool    cgiRead();
        bool    cgiWrite();

        void    setReturnStatus(int returnStatus);
        void    setContent(std::string& content);
        void    setPort(uint16_t port);
        void    setContentType(std::string& contentType);
        void    setContentLenght(int contentLenght);

        int             getReturnStatus() const;
        std::string     getContent() const;
        uint16_t        getPort() const;
        int             getContentLenght() const;
        std::string     getContentType() const;

    private:

        uint16_t    _port;
        std::string _content;
        int         _contentLenght;
        std::string _contentType;
        int         _returnStatus;
};
std::ostream& operator<<(std::ostream& os, const Response& obj);

#endif
