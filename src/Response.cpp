/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atardif <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:25:13 by atardif           #+#    #+#             */
/*   Updated: 2024/02/07 11:26:51 by atardif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Response::Response(void) {}
Response::Response(const Response& obj) {*this = obj;}
Response::~Response() {}

// ************************************************************************** //
//  OPERATORS
// ************************************************************************** //

Response	&Response::operator=(const Response& obj)
{
    if (this != &obj)
    {
        _content = obj._content;
        _contentType = obj._contentType;
        _contentLength = obj._contentLength;
        _statusCode = obj._statusCode;
    }
    return (*this);
}

std::ostream&   operator<<(std::ostream& os, const Response& obj)
{
    os << "--" << RESPONSE << "--" << std::endl;
    os << "return status  : " << obj.getStatusCode() << std::endl;
    os << "content type   : " << obj.getContentType() << std::endl;
    os << "content Length : " << obj.getContentLength() << std::endl;
    os << "content        : " << obj.getContent() << std::endl;
    return (os);
}

// ************************************************************************** //
//	GET / POST / DELETE METHODS
// ************************************************************************** //

bool    Response::craftAutoIndex(std::string path){
    
    (void)path;


    return true;
}

int    Response::readRessource(int fd)
{
    if (fd < 3)
        return (true);

    char    buffer[BUFFER_SIZE];
    int     nbBytesReaded = read(fd, buffer, BUFFER_SIZE);

    if (nbBytesReaded < 0)
    {
        return (2);
    }
    _content.append(buffer, nbBytesReaded);
    if (nbBytesReaded < BUFFER_SIZE)
        return (1);
    return (0);
}

bool    Response::postRessource(int fd, std::string content)
{
    return (write(fd, content.c_str(), content.size()) >= 0);
}

void Response::deleteRessource(std::string path)
{
    std::remove(path.c_str());
}

void    Response::resetValues(void)
{
    _content = "";
    _location = "";
    _statusCode = "";
    _contentType = "";
    _contentLength = 0;
}
 
std::string    Response::extract_extension(std::string path){

    size_t  dot = path.find_last_of('.');
    if (dot != std::string::npos && path.size() > 3)
        return (path.substr(path.find_last_of('.'), path.length()));
    return (path);
}

void    Response::setMimeType(std::string path, Base *_base){

    std::string key = extract_extension(path);
    std::map<std::string, std::string>::const_iterator search = _base->getMimeType().find(key);
    if (search != _base->getMimeType().end())
        return (void)(_contentType = search->second);
    return (void)(_contentType = "content-Type: text/html\n");
}

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

void    Response::setContent(std::string content) {_content = content;}
void    Response::setContentType(std::string contentType) {_contentType = contentType;}
void    Response::setContentLength(int contentLength) {_contentLength = contentLength;}
void    Response::setCgiPid(pid_t cgiPid) {_cgiPid = cgiPid;}
void    Response::setCgiFd(int cgiFd) {_cgiFd = cgiFd;}
void    Response::setCgiOutput(std::string cgiOutput) {_cgiOutput = cgiOutput;}
void    Response::setCgiBytesWritten(long cgiBytesWritten) {_cgiBytesWritten = cgiBytesWritten;}
void    Response::setCgiFdRessource(int cgiFdRessource) {_cgiFdRessource = cgiFdRessource;}
void    Response::setStatusCode(std::string statusCode) {_statusCode = statusCode;}
void    Response::setProtocol(std::string protocol) {_protocol = protocol;}
void    Response::setLocation(std::string location) {_location = location;}
void    Response::setCookies(std::string cookies) {_cookies = cookies;}


pid_t           Response::getCgiPid(void) const {return (_cgiPid);}
int             Response::getCgiFd(void) const {return (_cgiFd);}
std::string     Response::getCgiOutput(void) const {return (_cgiOutput);}
long            Response::getCgiBytesWritten(void) const {return (_cgiBytesWritten);}
int             Response::getCgiFdRessource(void) const {return (_cgiFdRessource);}
std::string     Response::getContent() const {return (_content);}
int             Response::getContentLength() const {return (_contentLength);}
std::string     Response::getContentType() const {return (_contentType);}
std::string     Response::getStatusCode() const {return (_statusCode);}
std::string     Response::getProtocol() const {return (_protocol);}
std::string     Response::getLocation() const {return (_location);}
std::string     Response::getCookies() const {return (_cookies);}
