
#include "../inc/Request.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Request::Request(void) : _method(0), _path(""), _version(""), _host(""), _userAgent(""), _contentType(""), _contentLenght(0), _connection(""), _body("")
{
	if (PRINT)
		std::cout << REQUEST << "🐥 constructor called" << std::endl;
}

Request::Request(const Request& obj) : _method(obj._method), _path(obj._path), _version(obj._version), _host(obj._host), _userAgent(obj._userAgent), _contentType(obj._contentType), _contentLenght(obj._contentLenght), _connection(obj._connection), _body(obj._body)
{
	if (PRINT)
		std::cout << REQUEST << "🐥 constructor called" << std::endl;
}

Request::~Request()
{
	if (PRINT)
		std::cout << REQUEST << "🗑️  destructor called" << std::endl;
}

// ************************************************************************** //
//  OPERATORS
// ************************************************************************** //

Request	&Request::operator=(Request const &obj)
{
    if (this != &obj)
    {
        this->setMethod(obj.getMethod());
        this->setPath(obj.getPath());
        this->setHost(obj.getHost());
        this->setUserAgent(obj.getUserAgent());
        this->setContentType(obj.getContentType());
        this->setContentLength(obj.getContentLenght());
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

bool    Request::isCompleted(void) const
{
    return (_headerCompleted && _bodyCompleted);
}

void	Request::findHost(std::vector<ServerInfo>& servers, ServerInfo &server)
{
    for (std::vector<ServerInfo>::iterator i = servers.begin(); i < servers.end(); i++)
    {
        std::stringstream ss;
        ss << i->getHost();
		if (ss.str() == _host && i->getListen() == _port)
        {
            server = *i;
    		return ;
        }
    }
    server = servers[0];
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
    this->setContentLength(0);
    // this->setCookies();
    this->setConnection("");
    this->setBody("");

    this->setBodyCompleted(false);
    this->setHeaderCompleted(false);
}

bool Request::fillContent(std::string request)
{
    time_t  startTime;
    time_t  endTime;
    startTime = clock();

    std::vector<std::string> lines = splitString(request, '\n');
    int size = lines.size();

    for (int i = 0; i < size; i++)
    {
        if (size >= 3 && lines[i].substr(0, 3) == "GET")
        {
            this->setMethod(GET);
            this->setPath(lines[i].substr(4, size));    // attention a ne pas recuperer la version d'HTML
        }
        else if (size >= 4 && lines[i].substr(0, 4) == "POST")
        {
            this->setMethod(POST);
            this->setPath(lines[i].substr(5, size));
        }
        else if (size >= 6 && lines[i].substr(0, 6) == "DELETE")
        {
            this->setMethod(DELETE);
            this->setPath(lines[i].substr(7, size));
        }
        deEncodingHexa(_path);
        if (size >= 5 && lines[i].substr(0, 5) == "Host:")
        {
            std::string rest = lines[i].substr(6, size);
            size_t      limit = rest.find(':');

            this->setHost(rest.substr(0, limit));
            this->setPort(rest.substr(0, limit));

        }
        else if (size >= 11 && lines[i].substr(0, 11) == "User-Agent:")
            this->setUserAgent(lines[i].substr(12, size));
        else if (size >= 13 && lines[i].substr(0, 13) == "Content-Type:")
            this->setContentType(lines[i].substr(14, size));
        else if (size >= 15 && lines[i].substr(0, 15) == "Content-Lenght:")
            this->setContentLength(atoi(lines[i].substr(16, size).c_str()));
        else if (size >= 6 && lines[i].substr(0, 6) == "Cookie:")
            this->setCookies(splitString(lines[i].substr(7, size), ' '));
        else if (size >= 11 && lines[i].substr(0, 11) == "Connection:")
            this->setConnection(lines[i].substr(12, size));
        else if (size && lines[i].substr(0, 1) == "{")
        {
            bool    in = true;
            for (int j = i + 1; j < size; j++)
            {
                if (in)
                    this->setBody(_body.append(lines[i]));
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

    endTime = clock();
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

void    Request::setMethod(int method) {_method = method;}
void    Request::setPath(std::string path) {_path = path;}
void    Request::setVersion(std::string version) {_version = version;}
void    Request::setHost(std::string host) {_host = host;}
void    Request::setPort(std::string port) {_port = port;}
void    Request::setUserAgent(std::string userAgent) {_userAgent = userAgent;}
void    Request::setContentType(std::string contentType) {_contentType = contentType;}
void    Request::setContentLength(int contentLenght) {_contentLenght = contentLenght;}
void    Request::setCookies(std::vector<std::string> cookies) {_cookies = cookies;}
void    Request::setConnection(std::string connection) {_connection = connection;}
void    Request::setBody(std::string body) {_body = body;}
void    Request::setHeaderCompleted(bool headerCompleted) {_headerCompleted = headerCompleted;}
void    Request::setBodyCompleted(bool bodyCompleted) {_bodyCompleted = bodyCompleted;}
void    Request::setServer(ServerInfo* server) {_server = server;}

int                         Request::getMethod(void) const {return (_method);}
std::string                 Request::getPath(void) const {return (_path);}
std::string                 Request::getVersion(void) const {return (_version);}
std::string                 Request::getHost(void) const {return (_host);}
std::string                 Request::getPort(void) const {return (_port);}
std::string                 Request::getUserAgent(void) const {return (_userAgent);}
std::string                 Request::getContentType(void) const {return (_contentType);}
int                         Request::getContentLenght(void) const {return (_contentLenght);}
std::vector<std::string>    Request::getCookies(void) const {return (_cookies);}
std::string                 Request::getConnection(void) const {return (_connection);}
std::string                 Request::getBody(void) const {return (_body);}
bool                        Request::getHeaderCompleted(void) {return (_headerCompleted);}
bool                        Request::getBodyCompleted(void) {return (_bodyCompleted);}
ServerInfo*                 Request::getServer(void) {return (_server);}
