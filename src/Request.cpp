
#include "../inc/Request.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Request::Request(void)
{
    if (PRINT)
        std::cout << GREEN << "Request constructor called" << END_COLOR << std::endl;
    this->resetValues();
}

Request::Request(const std::string str)
{
    if (PRINT)
        std::cout << GREEN << "Request constructor called" << END_COLOR << std::endl;
    this->fillContent(str);
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
    os << " cookies: ";
    printStringVector(obj.getCookies());
    os << std::endl << " connection: " << obj.getConnection() << std::endl;
    os << " body: " <<obj.getBody() << std::endl;
    return (os);
}

// ************************************************************************** //
//	METHODS
// ************************************************************************** //

void    Request::handle()
{
    if (this.getPath() == "/site")
        ;// get the content of the right html file with 
    else if (this.getPath() == "/site/scriptCGI")
        ;// do the right script CGI
}

// ************************************************************************** //
//	PARSING METHODS
// ************************************************************************** //

void    Request::resetValues(void)
{
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

bool Request::fillContent(std::string request)
{
    time_t  startTime;
    time_t  endTime;
    clock(&startTime);

    std::vector<string> lines;
    int size = lines.size();

    for (int i = 0; i < size; i++;)
    {
        if (size >= 3 && line.substr(0, 3) == "GET")
        {
            this->setMethod(GET);
            this->setPath(line.substr(4, size));
        }
        else if (size >= 4 && line.substr(0, 4) == "POST")
        {
            this->setMethod(POST);
            this->setPath(line.substr(5, size));
        }
        else if (size >= 6 && line.substr(0, 6) == "DELETE")
        {
            this->setMethod(DELETE);
            this->setPath(line.substr(7, size));
        }
        else if (size >= 5 && line.substr(0, 5) == "Host:")
            this->setHost(line.substr(6, size));
        else if (size >= 11 && line.substr(0, 11) == "User-Agent:")
            this->setUserAgent(line.substr(12, size));
        else if (size >= 13 && line.substr(0, 13) == "Content-Type:")
            this->setContentType(line.substr(14, size));
        else if (size >= 15 && line.substr(0, 15) == "Content-Lenght:")
            this->setContentLenght(line.substr(16, size));
        else if (size >= 6 && line.substr(0, 6) == "Cookie:")
            this->setCookies(splitString(line.substr(7, size)));
        else if (size >= 11 && line.substr(0, 11) == "Connection:")
            this->setConnection(line.substr(12, size));
        else if (size && line.substr(0, 1) == "{")
        {
            bool    in = true;
            for (int j = i + 1; j < size; j++)
            {
                if (in)
                    this->setBody(request.getBody().append(lines[i]));
                if (!in && size && lines[i].substr(0, 1) == "{")
                    in = true;
                if (size && lines[i].substr(0, 1) == "}")
                {
                    in = false;
                    break ;
                }
            }
            return (!in);
        }
    }

    clock(&endTime);
    std::cout << REQUEST << " fillContent method : exec time : " << endTime - startTime << std::endl;

    return (true);
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
