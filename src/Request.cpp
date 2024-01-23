
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
    os << " content type: " <<obj.getContentType() << " content length: " << obj.getContentLength();
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
    _contentLength = 0;
    _connection = "";
    _body = "";
}

void    Request::fillArgs(std::string str)
{
    std::istringstream    ss(str);

    std::getline(ss, _path, ' '); //Modifie par Alban, a remettre en etat
    std::getline(ss, _args, ' ');//Modifie par Alban, a remettre en etat
    std::getline(ss, _version, '\0');
}

bool	Request::fillMethod(std::string& line)
{
	int	length = line.size();
    if (length >= 3 && line.substr(0, 3) == "GET")
    {
        this->setMethod(GET);
        fillArgs(line.substr(4, length));
    }
    else if (length >= 4 && line.substr(0, 4) == "POST")
    {
        this->setMethod(POST);
        fillArgs(line.substr(5, length));
    }
    else if (length >= 6 && line.substr(0, 6) == "DELETE")
    {
        this->setMethod(DELETE);
        fillArgs(line.substr(7, length));
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
	int	length = lines[0].size();
    if (length > 70)
    {
        _returnStatus = 431;
        return (false);
    }

	if (size)
    	this->fillMethod(lines[0]);
    for (int i = 1; i < size; i++)
    {
		length = lines[i].size();
        if (length >= 5 && lines[i].substr(0, 5) == "Host:")
        {
            std::string rest = lines[i].substr(6, length);
            size_t      limit = rest.find(':');
            this->setHost(rest.substr(0, limit));
            this->setPort(rest.substr(0, limit));
        }
        else if (length >= 11 && lines[i].substr(0, 11) == "User-Agent:")
            this->setUserAgent(lines[i].substr(12, length));
        else if (length >= 13 && lines[i].substr(0, 13) == "Content-Type:")
            this->setContentType(lines[i].substr(14, length));
        else if (length >= 15 && lines[i].substr(0, 15) == "Content-length:")
            this->setContentLength(atoi(lines[i].substr(16, length).c_str()));
        else if (length >= 6 && lines[i].substr(0, 6) == "Cookie:")
            this->setCookies(splitString(lines[i].substr(7, length), ' '));
        else if (length >= 11 && lines[i].substr(0, 11) == "Connection:")
            this->setConnection(lines[i].substr(12, length));
        else if (length >= 18 && lines[i].substr(0, 11) == "Transfer-Encoding:")
            this->setTransfertEncoding(lines[i].substr(19, length));
        for (int j = i + 1; j < size; j++)
        {
            if (lines[i].size() && lines[i] == lines[j]) // si deux lignes sont identiques, c'est interdit
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
void    Request::setContentLength(int contentLength) {_contentLength = contentLength;}
void    Request::setCookies(std::vector<std::string> cookies) {_cookies = cookies;}
void    Request::setConnection(std::string connection) {_connection = connection;}
void    Request::setBody(std::string body) {_body = body;}
// void    Request::setServer(ServerInfo* server) {_server = server;}
void    Request::setReturnStatus(int returnStatus) {_returnStatus = returnStatus;}
void    Request::setArgs(std::string args) {_args = args;}
void    Request::setTransfertEncoding(std::string transfertEncoding) {_transfertEncoding = transfertEncoding;}


int                         Request::getMethod(void) const {return (_method);}
std::string                 Request::getPath(void) const {return (_path);}
std::string                 Request::getVersion(void) const {return (_version);}
std::string                 Request::getHost(void) const {return (_host);}
std::string                 Request::getPort(void) const {return (_port);}
std::string                 Request::getUserAgent(void) const {return (_userAgent);}
std::string                 Request::getContentType(void) const {return (_contentType);}
int                         Request::getContentLength(void) const {return (_contentLength);}
std::vector<std::string>    Request::getCookies(void) const {return (_cookies);}
std::string                 Request::getConnection(void) const {return (_connection);}
std::string                 Request::getBody(void) const {return (_body);}
// ServerInfo*                 Request::getServer(void) {return (_server);}
int                         Request::getReturnStatus(void) const {return (_returnStatus);}
std::string                 Request::getArgs(void) const {return (_args);};
std::string                 Request::getTransfertEncoding(void) const {return (_transfertEncoding);};

