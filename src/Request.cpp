
#include "../inc/Request.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Request::Request() {}
Request::Request(const Request& obj) {*this = obj;}
Request::~Request() {}

// ************************************************************************** //
//  OPERATORS
// ************************************************************************** //

Request	&Request::operator=(Request const &obj)
{
    if (this != &obj)
    {
        
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
    os << " body: " << obj.getBody() << std::endl;
    return (os);
}

// ************************************************************************** //
//	METHODS
// ************************************************************************** //

void    Request::resetValues()
{
    _method = 0;
    _path = "";
    _host = "";
    _userAgent = "";
    _contentType = "";
    _contentLenght = 0;
    _connection = "";
    _body = "";
}

bool	Request::fillMethod(std::string& line)
{
	int	lenght = line.size();
    if (lenght >= 3 && line.substr(0, 3) == "GET")
    {
        this->setMethod(GET);
        this->setPath(line.substr(4, lenght));    // attention a ne pas recuperer la version d'HTML
    }                                                 // si il faut recuperer la version et la mettre ailleur
    else if (lenght >= 4 && line.substr(0, 4) == "POST")
    {
        this->setMethod(POST);
        this->setPath(line.substr(5, lenght));
    }
    else if (lenght >= 6 && line.substr(0, 6) == "DELETE")
    {
        this->setMethod(DELETE);
        this->setPath(line.substr(7, lenght));
    }
	else
		return (false);
    deEncodingHexa(_path);
	return (true);
}

bool Request::fillHeader(std::string& header)	// attention c'est pontentielement juste le header
{
    time_t  startTime;
    time_t  endTime;
    startTime = clock();

    // clean le header avant

    removeBackSlashR(header);

    std::vector<std::string> lines = splitString(header, '\n');	// voir si on doit pas creer notre propre split
    int size = lines.size();
	int	lenght = lines[0].size();
    if (lenght > 70)
    {
        _returnStatus = 431;
        return (false);
    }

	if (size)
    	this->fillMethod(lines[0]);
    for (int i = 1; i < size; i++)
    {
		lenght = lines[i].size();
        if (lenght >= 5 && lines[i].substr(0, 5) == "Host:")
        {
            std::string rest = lines[i].substr(6, lenght);
            size_t      limit = rest.find(':');
            this->setHost(rest.substr(0, limit));
            this->setPort(rest.substr(0, limit));
        }
        else if (lenght >= 11 && lines[i].substr(0, 11) == "User-Agent:")
            this->setUserAgent(lines[i].substr(12, lenght));
        else if (lenght >= 13 && lines[i].substr(0, 13) == "Content-Type:")
            this->setContentType(lines[i].substr(14, lenght));
        else if (lenght >= 15 && lines[i].substr(0, 15) == "Content-Lenght:")
            this->setContentLength(atoi(lines[i].substr(16, lenght).c_str()));
        else if (lenght >= 6 && lines[i].substr(0, 6) == "Cookie:")
            this->setCookies(splitString(lines[i].substr(7, lenght), ' '));
        else if (lenght >= 11 && lines[i].substr(0, 11) == "Connection:")
            this->setConnection(lines[i].substr(12, lenght));
        for (int j = i + 1; j < size; j++)
        {
            if (lines[i] == lines[j]) // si deux lignes sont identiques, c'est interdit
            {
                _returnStatus = 400;
                return (false); // -> penser a modifier le status dans le client;
            }
        }
    }
    // changer la status pour receiving body ou alors si ca passe mal -> waiting for respond
    endTime = clock();
    std::cout << REQUEST << " fillContent method : exec time : " << endTime - startTime << std::endl;
    return (true);
}

bool    Request::fillBody(std::string&  body)   // a voir si on fait des modifs
{
    _body = body;
    // mettre a jour le status en waiting for response quel que soit le status;
    return (true);
}

int Request::checkHttpVersion()
{
    if (_version != "HTTP/1.0" && _version != "HTTP/1.1" && _version != "HTTP/0.9")
        return (false); // 505
    // _httpVersion = "HTTP/1.1";
    return (true);
}

// ************************************************************************** //
//	GET / SET
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
// void    Request::setServer(ServerInfo* server) {_server = server;}
void    Request::setReturnStatus(int returnStatus) {_returnStatus = returnStatus;}

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
// ServerInfo*                 Request::getServer(void) {return (_server);}
int                         Request::getReturnStatus(void) const {return (_returnStatus);}
