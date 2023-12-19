
#include "../inc/Response.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Response::Response(void) : _port(0), _content(""), _returnStatus(I_CONTIUE)
{
	if (PRINT)
		std::cout << RESPONSE << "🐥 constructor called" << std::endl;
}

Response::Response(const Response& obj): _port(obj._port), _content(obj._content), _returnStatus(obj._returnStatus)
{
	if (PRINT)
		std::cout << RESPONSE << "🐥 constructor called" << std::endl;
}

Response::~Response()
{
	if (PRINT)
		std::cout << RESPONSE << "🗑️  destructor called" << std::endl;
}

// ************************************************************************** //
//  OPERATORS
// ************************************************************************** //

Response	&Response::operator=(const Response& obj)
{
    if (this != &obj)
    {
        this->setPort(obj.getPort());
        this->setContent(obj.getContent());
        this->setReturnStatus(obj.getReturnStatus());
        this->setCgiPid(obj.getCgiPid());
        this->setCgiFd(obj.getCgiFd());
        this->setCgiOutput(obj.getCgiOutput());
        this->setCgiBytesWritten(obj.getCgiBytesWritten());
        this->setCgiFdRessource(obj.getCgiFdRessource());
    }
    return (*this);
}

std::ostream&   operator<<(std::ostream& os, const Response& obj)
{
    os << "--" << RESPONSE << "--" << std::endl;
    // os << "port : " << obj.getPort() << std::endl;
    os << "return status : " << obj.getReturnStatus() << std::endl;
    os << "content : " << obj.getContent() << std::endl;
    return (os);
}

// ************************************************************************** //
//	GET / POST / DELETE METHODS
// ************************************************************************** //

void    Response::readRessource(std::string path)
{
    const char*       cPath = path.c_str();
	struct stat sb;
    // Response    response;

    std::cout << "y a t-il un pilote dans l'avion" << std::endl;
	if (stat(cPath, &sb) != 0)
    {
        this->setReturnStatus(E_NOT_FOUND);
        std::cerr << "read ressource : " << path << " : does not exist" << std::endl;
        return ;
    }
	if (!S_ISREG(sb.st_mode))
    {
        this->setReturnStatus(E_CONFLICT); // je suis pas certain pour ce code d'erreur
        std::cerr << "read ressource : " << path << " : is not a regular file" << std::endl;
        return ;
    }
	std::ifstream ifs(cPath);
	if (!ifs.is_open())
    {
        this->setReturnStatus(E_FORBIDDEN); // je suis pas certain pour ce code d'erreur
        std::cerr << "read ressource : " << path << " : cant be opened" << std::endl;
		return ;
    }
    _content.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
    _returnStatus = S_OK;
	return ;
}

void    Response::postRessource(const std::string path)
{
    std::ofstream file(path.c_str());
    // file << fileContent;
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
//	CGI METHODS
// ************************************************************************** //

// bool    Response::cgiRead()
// {
//     char    buffer[BUFFER_SIZE] = {};
//     int     bytesReaded;

//     // + gerer le timeout
//     bytesReaded = read(_cgiFd, buffer, BUFFER_SIZE);
//     if (!bytesReaded)
//         return (true);//
//     if (bytesReaded < 0)
//     {
//         _returnStatus = E_INTERNAL_SERVER;
//         return (false);
//     }
//     _cgiOutput.append(buffer);
//     return (true);
// }

// bool    Response::cgiWrite()
// {
//     long    bytesWritten;

//     // if (!(_method & POST))
//     //     return ;
    
//     bytesWritten = write(_cgiFdRessource, body + _cgiBytesWritten, BUFFER_SIZE);
//     if (bytesWritten < 0)
//     {
        
//         return (false);
//     }
//     _cgiBytesWritten += bytesWritten;
//     return (true);
// }

// ************************************************************************** //
//	PARSING METHODS
// ************************************************************************** //

void    Response::resetValues(void)
{
    _port = 0;
    _content = "";
    _returnStatus = I_CONTIUE;
    _cgiPid = 0;
    _cgiFd = 0;
    _cgiOutput = "";
    _cgiBytesWritten = 0;
    _cgiFdRessource = 0;
}

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

void    Response::setReturnStatus(int returnStatus) {returnStatus = returnStatus;}
void    Response::setContent(std::string content) {_content = content;}
void    Response::setPort(uint16_t port) {_port = port;}
void    Response::setCgiPid(pid_t cgiPid) {_cgiPid = cgiPid;}
void    Response::setCgiFd(int cgiFd) {_cgiFd = cgiFd;}
void    Response::setCgiOutput(std::string cgiOutput) {_cgiOutput = cgiOutput;}
void    Response::setCgiBytesWritten(long cgiBytesWritten) {_cgiBytesWritten = cgiBytesWritten;}
void    Response::setCgiFdRessource(int cgiFdRessource) {_cgiFdRessource = cgiFdRessource;}

int             Response::getReturnStatus(void) const {return (_returnStatus);}
std::string     Response::getContent(void) const {return (_content);}
uint16_t        Response::getPort(void) const {return (_port);}
pid_t           Response::getCgiPid(void) const {return (_cgiPid);}
int             Response::getCgiFd(void) const {return (_cgiFd);}
std::string     Response::getCgiOutput(void) const {return (_cgiOutput);}
long            Response::getCgiBytesWritten(void) const {return (_cgiBytesWritten);}
int             Response::getCgiFdRessource(void) const {return (_cgiFdRessource);}
