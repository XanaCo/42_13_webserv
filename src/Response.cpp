
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
        _returnStatus = obj._returnStatus;
    }
    return (*this);
}

std::ostream&   operator<<(std::ostream& os, const Response& obj)
{
    os << "--" << RESPONSE << "--" << std::endl;
    os << "return status  : " << obj.getReturnStatus() << std::endl;
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

bool    Response::readRessource(int fd)
{
    char    buffer[BUFFER_SIZE];
    int     nbBytesReaded = read(fd, buffer, BUFFER_SIZE);

    if (nbBytesReaded < 0)
    {
        close(fd);
        return (true); // a changer
    }
    _content.append(buffer, nbBytesReaded);
    if (nbBytesReaded < BUFFER_SIZE)
        return (true);
    return (false);
}

// a conserver pour ameliorer l'ouverture d'un fichier de reponse
// void    Response::readRessource(std::string path)
// {
//     const char*       cPath = path.c_str();
// 	struct stat sb;
//     // Response    response;

// 	if (stat(cPath, &sb) != 0)
//     {
//         this->setReturnStatus(E_NOT_FOUND);
//         std::cerr << "read ressource : " << path << " : does not exist" << std::endl;
//         return ;
//     }
// 	if (!S_ISREG(sb.st_mode))
//     {
//         this->setReturnStatus(E_CONFLICT); // je suis pas certain pour ce code d'erreur
//         std::cerr << "read ressource : " << path << " : is not a regular file" << std::endl;
//         return ;
//     }
// 	std::ifstream ifs(cPath);
// 	if (!ifs.is_open())
//     {
//         this->setReturnStatus(E_FORBIDDEN); // je suis pas certain pour ce code d'erreur
//         std::cerr << "read ressource : " << path << " : cant be opened" << std::endl;
// 		return ;
//     }
//     _content.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
//     _returnStatus = S_OK;
// 	return ;
// }

void    Response::postRessource(const std::string path, std::string content)
{
    std::ofstream file(path.c_str());
    file << content;
    file.close();
}

void Response::deleteRessource(const std::string path)
{
    if (unlink(path.c_str()) != 0)
        std::cerr << "delete ressource : " << path  << " : erreur lors de la suppression du fichier"<< std::endl;
    else
        std::cout << "Le fichier " << path << " a été supprimé avec succès." << std::endl;
}

// ************************************************************************** //
//	
// ************************************************************************** //

bool    Response::addBuffer(int fd)
{
    // if (_fdFileToSend < 3)
    // {
    //     // le fichier n'est pas ouvert
    //     return (false);
    // }
    int     bytesReaded;
    char    line[BUFFER_SIZE + 1];
    bytesReaded = read(fd, line, BUFFER_SIZE);
    if (bytesReaded < 0)
    {

        return (false);
    }
    else if (bytesReaded)
    {
        line[bytesReaded] = 0;
        _content += line;
    }
    if (bytesReaded < BUFFER_SIZE)
    {
        return (false);
        // close le fichier
        // mettre a jour le status en mode "on a tout recu"
    }
    return (true);
}

// ************************************************************************** //
//	
// ************************************************************************** //

void    Response::resetValues(void)
{
    // _port = 0;
    _content = "";
    _returnStatus = I_CONTIUE;
    _contentType = "";
    _contentLength = 0;
}

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

void    Response::setReturnStatus(int returnStatus) {returnStatus = returnStatus;}
void    Response::setContent(std::string& content) {_content = content;}
void    Response::setContentType(std::string& contentType) {_contentType = contentType;}
void    Response::setContentLength(int contentLength) {_contentLength = contentLength;}
void    Response::setCgiPid(pid_t cgiPid) {_cgiPid = cgiPid;}
void    Response::setCgiFd(int cgiFd) {_cgiFd = cgiFd;}
void    Response::setCgiOutput(std::string cgiOutput) {_cgiOutput = cgiOutput;}
void    Response::setCgiBytesWritten(long cgiBytesWritten) {_cgiBytesWritten = cgiBytesWritten;}
void    Response::setCgiFdRessource(int cgiFdRessource) {_cgiFdRessource = cgiFdRessource;}
void    Response::setStatusCode(std::string statusCode) {_statusCode = statusCode;}
void    Response::setProtocol(std::string protocol) {_protocol = protocol;}
// void    Response::setPort(uint16_t port) {_port = port;}


pid_t           Response::getCgiPid(void) const {return (_cgiPid);}
int             Response::getCgiFd(void) const {return (_cgiFd);}
std::string     Response::getCgiOutput(void) const {return (_cgiOutput);}
long            Response::getCgiBytesWritten(void) const {return (_cgiBytesWritten);}
int             Response::getCgiFdRessource(void) const {return (_cgiFdRessource);}
int             Response::getReturnStatus() const {return (_returnStatus);}
std::string     Response::getContent() const {return (_content);}
int             Response::getContentLength() const {return (_contentLength);}
std::string     Response::getContentType() const {return (_contentType);}
std::string     Response::getStatusCode() const {return (_statusCode);}
std::string     Response::getProtocol() const {return (_protocol);}
// uint16_t        Response::getPort() const {return (_port);}
