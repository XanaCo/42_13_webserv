
#include "../inc/Response.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

const std::string host = "en.wikipedia.org";
// const std::string path = "/wiki/Main_Page";

Response::Response(uint16_t port): _port(port)
{
    if (PRINT)
        std::cout << GREEN << "Response constructor called" << END_COLOR << std::endl;
    this->resetValues();
}

Response::~Response()
{
    if (PRINT)
        std::cout << RED << "Response destructor called" << END_COLOR << std::endl;
}

// ************************************************************************** //
//  OPERATORS
// ************************************************************************** //

Response	&Response::operator=(Response const &obj)
{
    // if (this != obj)
    // {
        _port = obj._port;
        _content = obj._content;
        _returnStatus = obj._returnStatus;
        _cgiPid = obj._cgiPid;
        _cgiFd = obj._cgiFd;
        _cgiOutput = obj._cgiOutput;
        _cgiBytesWritten = obj._cgiBytesWritten;
        _cgiFdRessource = obj._cgiFdRessource;
    // }
    return (*this);
}

std::ostream	&operator<<(std::ostream &os, Response &obj)
{
    os << RESPONSE << " Port : " << obj.getPort();
    os << " return status : " << obj.getReturnStatus() << std::endl;
    os << "content : " << obj.getContent() << std::endl;
    return (os);
}

// ************************************************************************** //
//	METHODS
// ************************************************************************** //

// pour l'instant je traite le cas ou nous n'avons pas l'information demande
// on va donc demander l'information a un autre serveur.
// dans le sujet on appelle ca une redirection
void    Response::redirection(void)
{
    // Résolution du nom de domaine
    struct hostent* server = gethostbyname(host.c_str()); // /!\ la fonction n'est pas autorisee
    if (!server)
    {
        this->setReturnStatus(E_BAD_GATEWAY);
        // std::cerr << "Erreur de résolution du nom de domaine." << std::endl;
        return ;
    }

    // Création de la socket server
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Erreur lors de la création de la socket." << std::endl;
        return ;
    }

    // Configuration de l'adresse du serveur
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET; // on specifie qu'on traite des adresse IPV4
    serverAddress.sin_port = htons(80); // configure le port qu'on va utiliser pour communiquer avec le serveur
    bcopy((char*)server->h_addr, (char*)&serverAddress.sin_addr.s_addr, server->h_length);  // on copie l'adresse IP du server qu'on a vu precedement

    // Connexion au serveur
    if (connect(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
    {
        this->setReturnStatus(E_SERVICE_UNAVAILABLE);
        // std::cerr << "Erreur lors de la connexion au serveur." << std::endl;
        return ;
    }

    char        buffer[BUFFER_SIZE];
    ssize_t     bytesRead;
    std::string response; // changer par un attribut de notre obj

    // recv nous permet de recevoir la donnee a partir d'une socket
    bytesRead = recv(sockfd, buffer, BUFFER_SIZE, 0);
    while (bytesRead)
    {
        this->setContent(this->getContent().append(buffer, bytesRead));
        bytesRead = recv(sockfd, buffer, BUFFER_SIZE, 0);
    }
    this->setReturnStatus(S_OK);
}

// ************************************************************************** //
//	GET / POST / DELETE
// ************************************************************************** //

void    Response::readRessource(std::string path)
{
    const char*       cPath = path.c_str();
	struct stat sb;
    // Response    response;

	if (stat(cPath, &sb) != 0)
    {
        this->setReturnStatus(E_NOT_FOUND);
        // std::cerr << path << " : does not exist" << std::endl;
        return ;
    }
	if (!S_ISREG(sb.st_mode))
    {
        this->setReturnStatus(E_CONFLICT); // je suis pas certain pour ce code d'erreur
        // std::cerr << path << " : is not a regular file" << std::endl;
        return ;
    }
	std::ifstream ifs(cPath);
	if (!ifs.is_open())
    {
        this->setReturnStatus(E_FORBIDDEN); // je suis pas certain pour ce code d'erreur
        // std::cerr << path << " : cant be opened" << std::endl;
		return ;
    }
    // changer le READ RESSOURCE
	// this->setContent(ifs.rdbuf());
	// ifs.close();
	// if (!(this->getContent().tellp()))
    // {
    //     this->setReturnStatus(S_NO_CONTENT); // pas certain du return false;
    //     // std::cerr << path << " : is empty" << std::endl;
    //     return ;
    // }
    // this->setReturnStatus(S_OK);
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
        std::cerr << "Erreur lors de la suppression du fichier " << path << std::endl;
    else
        std::cout << "Le fichier " << path << " a été supprimé avec succès." << std::endl;
}


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
