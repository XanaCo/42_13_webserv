#include "Response.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

const std::string host = "en.wikipedia.org";
const std::string path = "/wiki/Main_Page";

Response::Response(uint16_t port): _port(port), _content("")
{}

Response::~Response()
{}

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

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

