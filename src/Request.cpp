
#include "../inc/Request.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Request(const std::string str)
{
    if (PRINT)
        std::cout << GREEN << "Request constructor called" << END_COLOR << std::endl;
    this->setMethod(0);
    this->setPath("");
    this->setHost("");
    this->setUserAgent("");
    this->setContentType("");
    this->setContentLenght(0);
    // this->setCookies();
    this->setConnection("");
    this->setBody("");
}

Request::~Request()
{
    if (PRINT)
        std::cout << RED << "Response destructor called" << END_COLOR << std::endl;
}

// ************************************************************************** //
//  OPERATORS
// ************************************************************************** //

Request	&Request::operator=(Request const &obj)
{
    if (this != obj)
    {
        this->setMethod(obj.getMethod());
        this->setPath(obj.getPath());
        this->setHost(obj.getHost());
        this->setUserAgent(obj.getUserAgent());
        this->setContentType(obj.getContentType());
        this->setContentLenght(obj.getContentLenght());
        this->setCookies(obj.getCookies());
        this->setConnection(obj.getConnection());
        this->setBody(obj.getBody());
    }
    return (*this);
}

std::ostream	&operator<<(std::ostream &os, Request &obj)
{
    os << REQUEST << " method: " <<obj.getMethod() << " path: " << obj.getPath();
    os << " host: " <<obj.getHost() << " user agent: " << obj.getUserAgent();
    os << " content type: " <<obj.getContentType() << " content lenght: " << obj.getContentLenght();
    os << " cookies: " <<obj.getCookies() << " connection: " << obj.getConnection() << std::endl;
    os << " body: " <<obj.getBody() << std::endl;
    return (os);
}

// ************************************************************************** //
//	METHODS
// ************************************************************************** //

// ************************************************************************** //
//	PARSING METHODS
// ************************************************************************** //

void fillContent(std::string line)
{
    int size = line.size();

    Request request;

    if (size >= 3 && line.substr(0, 3) == "GET")
    {
        request.setMethod(GET);
        request.setPath(line.substr(4, size));
    }
    else if (size >= 4 && line.substr(0, 4) == "POST")
    {
        request.setMethod(POST);
        request.setPath(line.substr(5, size));
    }
    else if (size >= 6 && line.substr(0, 6) == "DELETE")
    {
        request.setMethod(DELETE);
        request.setPath(line.substr(7, size));
    }
    else if (size >= 5 && line.substr(0, 5) == "Host:")
        request.setHost(line.substr(6, size));
    else if (size >= 11 && line.substr(0, 11) == "User-Agent:")
        request.setUserAgent(line.substr(12, size));
    else if (size >= 13 && line.substr(0, 13) == "Content-Type:")
        request.setContentType(line.substr(14, size));
    else if (size >= 15 && line.substr(0, 15) == "Content-Lenght:")
        request.setContentLenght(line.substr(16, size));
    else if (size >= 6 && line.substr(0, 6) == "Cookie:")
        request.setCookies(splitString(line.substr(7, size)));
    else if (size >= 11 && line.substr(0, 11) == "Connection:")
        request.setConnection(line.substr(12, size));
    if (size && line.substr(0, 1) == "{")
    {
        return (true);
        // fil body;
    }
    return ();
    // return (OTHER_CONTENT);
}
// A voir si on decide de traiter ces elements :
    // if (line.substr(0, 7) == "Accept:")
    //     return (ACCEPT);
    // if (line.substr(0, 17) == "Accept-Language:")
    //     return (ACCEPT_LANGUAGE);
    // if (line.substr(0, 16) == "Accept-Encoding:")
    //     return (ACCEPT_ENCODING);
    // if (line.substr(0, 14) == "Autorization:")
    //     return (AUTOR);
    // if (line.substr(0, 8) == "Referer:")
    //     return (REFERER);
    // if (line.substr(0, 14) == "Cache-Control:")
    //     return (CACHECONTROL);

void    fillBody(std::vector<std::string> lines)
{
    int     size = lines.size();
    bool    in;

    for (int i = 0; i < size, i++)
    {
        if (in)
            request.setBody(request.getBody().append(lines[i]));
        if (!in && size && lines[i].substr(0, 1) == "{")
            in = true;
        if (size && lines[i].substr(0, 1) == "}")
            break ;
    }
}

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

void    Request::setMethod(int method)
{
    _method = method;
}

void    Request::setPath(std::string path)
{
    _path = path;
}

void    Request::setHost(std::string host)
{
    _host = host;
}

void    Request::setUserAgent(std::string userAgent)
{
    _userAgent = userAgent;
}

void    Request::setContentType(std::string contentType)
{
    _contentType = contentType;
}

void    Request::setContentLenght(std::string contentLenght)
{
    _contentLenght = contentLenght;
}

void    Request::setCookies(std::vector<std::string> cookies)
{
    _cookies = cookies;
}

void    Request::setConnection(std::string connection)
{
    _connection = connection;
}

void    Request::setBody(std::string body)
{
    _body = body;
}

int    Request::getMethod(void) const
{
    return (_method);
}

std::string    Request::getPath(void) const
{
    return (_path);
}

std::string    Request::getHost(void) const
{
    return (_host);
}

std::string    Request::getUserAgent(void) const
{
    return (_userAgent);
}

std::string    Request::getContentType(void) const
{
    return (_contentType);
}

int    Request::getContentLenght(void) const
{
    return (_contentLenght);
}

std::vector<std::string>    Request::getCookies(void) const
{
    return (_cookies);
}

std::string    Request::getConnection(void) const
{
    return (_connection);
}

std::string    Request::getBody(void) const
{
    return (_body);
}
