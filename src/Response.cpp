#include "../inc/Response.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

const std::string host = "en.wikipedia.org";
const std::string path = "/wiki/Main_Page";

Response::Response(uint16_t port): _port(port), _content("")
{
    if (PRINT)
        std::cout << GREEN << "Response constructor called" << END_COLOR << std::endl;
    this->setReturnStatus(200);
    this->setContent("");
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
    if (this != obj)
    {
        _content = obj._content;
    }
    return (*this);
}

std::ostream	&operator<<(std::ostream &os, Response &obj)
{
    os << RESPONSE << "variable name :" << std::endl;
    return (os);
}

// ************************************************************************** //
//	METHODS
// ************************************************************************** //

// pour l'instant je traite le cas ou nous n'avons pas l'information demande
// on va donc demander l'information a un autre serveur.
// dans le sujet on appelle ca une redirection
void    Response::recieve()
{
    // Résolution du nom de domaine
    struct hostent* server = gethostbyname(host.c_str()); // /!\ la fonction n'est pas autorisee
    if (server == nullptr)
    {
        std::cerr << "Erreur de résolution du nom de domaine." << std::endl;
        return (1);
    }

    // Création de la socket server
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Erreur lors de la création de la socket." << std::endl;
        return (1);
    }

    // Configuration de l'adresse du serveur
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET; // on specifie qu'on traite des adresse IPV4
    serverAddress.sin_port = htons(80); // configure le port qu'on va utiliser pour communiquer avec le serveur
    bcopy((char*)server->h_addr, (char*)&serverAddress.sin_addr.s_addr, server->h_length);  // on copie l'adresse IP du server qu'on a vu precedement

    // Connexion au serveur
    if (connect(sockfd, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Erreur lors de la connexion au serveur." << std::endl;
        return (1);
    }

    char        buffer[BUFFER_SIZE];
    ssize_t     bytesRead;
    std::string response; // changer par un attribut de notre obj

    // recv nous permet de recevoir la donnee a partir d'une socket
    bytesRead = recv(sockfd, buffer, BUFFER_SIZE, 0);
    while (bytesRead)
    {
        response.append(buffer, bytesRead);
        bytesRead = recv(sockfd, buffer, BUFFER_SIZE, 0);
    }
}

// dans le cas de GET,
// verifier le path est autorise dans le fichier de configuration
// puis recuperer le contenu du fichier a l'aide de cette fonction :

bool readRessource(const char* path, std::stringstream& content)
{
	struct stat sb;
    Response    response;

	if (stat(path, &sb) != 0)
    {
        response.setReturnStatus(E_NOT_FOUND);
        // std::cerr << path << " : does not exist" << std::endl;
        return (false);
    }
	if (!S_ISREG(sb.st_mode))
    {
        response.setReturnStatus(E_CONFLICT); // je suis pas certain pour ce code d'erreur
        // std::cerr << path << " : is not a regular file" << std::endl;
        return (false);
    }
	std::ifstream ifs(path);
	if (!ifs.is_open())
    {
        response.setReturnStatus(E_FORBIDDEN); // je suis pas certain pour ce code d'erreur
        // std::cerr << path << " : cant be opened" << std::endl;
		return (false);
    }
	content << ifs.rdbuf();
	ifs.close();
	if (!content.tellp())
    {
        response.setReturnStatus(S_NO_CONTENT); // pas sur de return false;
        // std::cerr << path << " : is empty" << std::endl;
        return (false);
    }
	return (true);
}

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

void    Response::setReturnStatus(int returnStatus)
{
    _returnStatus = returnStatus;
}

void    Response::setContent(int content)
{
    _content = content;
}

int    Response::getReturnStatus(void) const;
{
    return (_returnStatus);
}

std::string    Response::getContent(void) const;
{
    return (_content);
}
