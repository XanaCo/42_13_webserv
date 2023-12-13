
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
    if (this != &obj)
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

// void    Request::handle()
// {
//     if (this.getPath() == "/site")
//         ;// get the content of the right html file with
//     else if (this.getPath() == "/site/scriptCGI")
//         ;// do the right script CGI
// }

bool    Request::isCompleted(void) const
{
    return (_headerCompleted && _bodyCompleted);
}

// ************************************************************************** //
//	CONTROLE QUALITE && CONSULTATION DES ARCHIVES
// ************************************************************************** //

// ici on a recu la requete et on cherche a savoir si elle est validee par la street
bool    Request::checkup(void)
{
    // verification de la syntaxe
    
    // verification de la methode

    // verification de la validite de l'URI

    // verification de la version du protocole HTTP ?

    // Verification des en-tetes de requete

    // Verification de la taille du body

    // Protections contre les attaques !?

    return (true);
}


// ici la requete est validee et on cherche a savoir quel sous-serveur est concerne par cette derniere
bool	Request::findHost(std::vector<ServerInfo> servers, ServerInfo &server)
{
    int size = servers.size();

    for (int i = 0; i < size; i++)
    {
		if (_host == servers[i].getServerName())
        {
            server = servers[i];
            // + link le server avec la requete
    		return (true);
        }
    }

	return (false);
}

// ici le host est identifie et on cherche a voir si la ressource est coherente
// bool    Request::findRessource(std::string& path)
// {
//     // on normalise le chemin, c'est a dire qu'on va enlever les '.' et '..'
//     // en fait on va pas accepter les .. car c'est pas securise, humm c'est interdit
//     // + compression des barres obliques "///" -> "/"

//     path = _server->getRoot() + _path;


//     compressionOfSlashes(path);
//     if (containsParentDirectory (path))
// 	{
// 		//_status MAJ
// 		throw ServerInfo::ServerInfoError("Pablo is awesome and checked this error");
// 	}
//     return (true);
// }

// /!\ apparement quand on recois un requete il y a une phase de decodage
// pour l'URI (le texte est encode en %XX base 16)

// ************************************************************************** //
//	LAS METHODAS (LAs GETAS Y LAs POSTAS Y LAs DELETAS)
// ************************************************************************** //

// ici on a compris qu'on doit lire une ressource, on recupere le contenu de la ressource
// bool Request::readRessource(const std::string& path, std::string& content)
// {
//     std::ifstream fichier(path.c_str());
//     if (fichier)
//     {
//         std::stringstream buffer;
//         buffer << fichier.rdbuf();
//         content = buffer.str();
//         fichier.close();
//         return (true);
//     }
//     std::cerr << "Erreur lors de la lecture du fichier : " << path << std::endl;
//     return (false);
// }

// {
//     if (resources.find(resource) != resources.end())
//     {
//         std::cerr << "Post error : Ressource already exist : " << resource << std::endl;
//     }
//     else
//     {
//         resources[resource] = content;
//         // std::cout << "Ressource created : " << resource << std::endl;
//     }
// }

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
            this->setPath(lines[i].substr(4, size));
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
        else if (size >= 5 && lines[i].substr(0, 5) == "Host:")
            this->setHost(lines[i].substr(6, size));
        else if (size >= 11 && lines[i].substr(0, 11) == "User-Agent:")
            this->setUserAgent(lines[i].substr(12, size));
        else if (size >= 13 && lines[i].substr(0, 13) == "Content-Type:")
            this->setContentType(lines[i].substr(14, size));
        else if (size >= 15 && lines[i].substr(0, 15) == "Content-Lenght:")
            this->setContentLenght(atoi(lines[i].substr(16, size).c_str()));
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
void    Request::setHost(std::string host) {_host = host;}
void    Request::setUserAgent(std::string userAgent) {_userAgent = userAgent;}
void    Request::setContentType(std::string contentType) {_contentType = contentType;}
void    Request::setContentLenght(int contentLenght) {_contentLenght = contentLenght;}
void    Request::setCookies(std::vector<std::string> cookies) {_cookies = cookies;}
void    Request::setConnection(std::string connection) {_connection = connection;}
void    Request::setBody(std::string body) {_body = body;}
void    Request::setHeaderCompleted(bool headerCompleted) {_headerCompleted = headerCompleted;}
void    Request::setBodyCompleted(bool bodyCompleted) {_bodyCompleted = bodyCompleted;}

int                         Request::getMethod(void) const {return (_method);}
std::string                 Request::getPath(void) const {return (_path);}
std::string                 Request::getHost(void) const {return (_host);}
std::string                 Request::getUserAgent(void) const {return (_userAgent);}
std::string                 Request::getContentType(void) const {return (_contentType);}
int                         Request::getContentLenght(void) const {return (_contentLenght);}
std::vector<std::string>    Request::getCookies(void) const {return (_cookies);}
std::string                 Request::getConnection(void) const {return (_connection);}
std::string                 Request::getBody(void) const {return (_body);}
bool                        Request::getHeaderCompleted(void) {return (_headerCompleted);}
bool                        Request::getBodyCompleted(void) {return (_bodyCompleted);}
