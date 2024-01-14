
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
        bool        addBuffer(int fd);
        // void        openFileToSend(std::string& file);


        bool    cgiRead();
        bool    cgiWrite();

        void    setReturnStatus(int returnStatus);
        void    setContent(std::string& content);
        void    setPort(uint16_t port);
        void    setContentType(std::string& contentType);
        void    setContentLenght(int contentLenght);
        void    setCgiPid(pid_t cgiPid);
        void    setCgiFd(int cgiFd);
        void    setCgiOutput(std::string cgiOutput);
        void    setCgiBytesWritten(long cgiBytesWritten);
        void    setCgiFdRessource(int cgiFdRessource);
        
        pid_t           getCgiPid(void) const;
        int             getCgiFd(void) const;
        std::string     getCgiOutput(void) const;
        long            getCgiBytesWritten(void) const;
        int             getCgiFdRessource(void) const;
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
        pid_t       _cgiPid;
        int         _cgiFd;
        std::string _cgiOutput;
        long        _cgiBytesWritten;
        int         _cgiFdRessource;
};
std::ostream& operator<<(std::ostream& os, const Response& obj);

#endif
