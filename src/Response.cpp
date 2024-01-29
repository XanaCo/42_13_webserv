
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
        _statusCode = obj._statusCode;
    }
    return (*this);
}

std::ostream&   operator<<(std::ostream& os, const Response& obj)
{
    os << "--" << RESPONSE << "--" << std::endl;
    os << "return status  : " << obj.getStatusCode() << std::endl;
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
        // close(fd);
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

// bool    Response::addBuffer(int fd)
// {
//     // if (_fdFileToSend < 3)
//     // {
//     //     // le fichier n'est pas ouvert
//     //     return (false);
//     // }
//     int     bytesReaded;
//     char    line[BUFFER_SIZE + 1];
//     bytesReaded = read(fd, line, BUFFER_SIZE);
//     if (bytesReaded < 0)
//     {

//         return (false);
//     }
//     else if (bytesReaded)
//     {
//         line[bytesReaded] = 0;
//         _content += line;
//     }
//     if (bytesReaded < BUFFER_SIZE)
//     {
//         return (false);
//         // close le fichier
//         // mettre a jour le status en mode "on a tout recu"
//     }
//     return (true);
// }

// ************************************************************************** //
//	
// ************************************************************************** //

void    Response::resetValues(void)
{
    // _port = 0;
    _content = "";
    _location = "";
    _contentType = "";
    _contentLength = 0;
}
        
void    Response::setMimeType(std::string path)
{
    if (path.size() >= 3)
    {
        if (!path.compare(path.length() -3, 3, ".bz"))
            return (void)(_contentType = "Content-Type: application/x-bzip\n");
        else if (!path.compare(path.length() -3, 3, ".js"))
            return (void)(_contentType = "Content-Type: application/javascript\n");
        else if (!path.compare(path.length() -3, 3, ".sh"))
            return (void)(_contentType = "Content-Type: application/x-sh\n");
        else if (!path.compare(path.length() -3, 3, ".ts"))
            return (void)(_contentType = "Content-Type: application/typescript\n");
        else if (!path.compare(path.length() -3, 3, ".7z"))
            return (void)(_contentType = "Content-Type: application/x-7z-compressed\n");
    }
    if (path.size() >= 4)
    {
        if (!path.compare(path.length() -4, 4, ".aac"))
            return (void)(_contentType = "Content-Type: audio/aac\n");
        else if (!path.compare(path.length() -4, 4, ".abw"))
            return (void)(_contentType = "Content-Type: application/x-abiword\n");
        else if (!path.compare(path.length() -4, 4, ".arc"))
            return (void)(_contentType = "Content-Type: application/octet-stream\n");
        else if (!path.compare(path.length() -4, 4, ".avi"))
            return (void)(_contentType = "Content-Type: video/x-msvideo\n");
        else if (!path.compare(path.length() -4, 4, ".azw"))
            return (void)(_contentType = "Content-Type: application/vnd.amazon.ebook\n");
        else if (!path.compare(path.length() -4, 4, ".bin"))
            return (void)(_contentType = "Content-Type: application/octet-stream\n");
        else if (!path.compare(path.length() -4, 4, ".bmp"))
            return (void)(_contentType = "Content-Type: image/bmp\n");
        else if (!path.compare(path.length() -4, 4, ".bz2"))
            return (void)(_contentType = "Content-Type: application/x-bzip2\n");
        else if (!path.compare(path.length() -4, 4, ".csh"))
            return (void)(_contentType = "Content-Type: application/x-csh\n");
        else if (!path.compare(path.length() -4, 4, ".css"))
            return (void)(_contentType = "Content-Type: text/css\n");
        else if (!path.compare(path.length() -4, 4, ".csv"))
            return (void)(_contentType = "Content-Type: text/csv\n");
        else if (!path.compare(path.length() -4, 4, ".doc"))
            return (void)(_contentType = "Content-Type: application/msword\n");
        else if (!path.compare(path.length() -4, 4, ".eot"))
            return (void)(_contentType = "Content-Type: application/vnd.ms-fontobject\n");
        else if (!path.compare(path.length() -4, 4, ".gif"))
            return (void)(_contentType = "Content-Type: image/gif\n");
        else if (!path.compare(path.length() -4, 4, ".htm"))
            return (void)(_contentType = "Content-Type: text/html\n");
        else if (!path.compare(path.length() -4, 4, ".ico"))
            return (void)(_contentType = "Content-Type: image/x-icon\n");
        else if (!path.compare(path.length() -4, 4, ".ics"))
            return (void)(_contentType = "Content-Type: text/calendar\n");
        else if (!path.compare(path.length() -4, 4, ".jar"))
            return (void)(_contentType = "Content-Type: application/java-archive\n");
        else if (!path.compare(path.length() -4, 4, ".jpg"))
            return (void)(_contentType = "Content-Type: image/jpeg\n");
        else if (!path.compare(path.length() -4, 4, ".mid"))
            return (void)(_contentType = "Content-Type: audio/midi\n");
        else if (!path.compare(path.length() -4, 4, ".odp"))
            return (void)(_contentType = "Content-Type: application/vnd.oasis.opendocument.presentation\n");
        else if (!path.compare(path.length() -4, 4, ".ods"))
            return (void)(_contentType = "Content-Type: application/vnd.oasis.opendocument.spreadsheet\n");
        else if (!path.compare(path.length() -4, 4, ".odt"))
            return (void)(_contentType = "Content-Type: application/vnd.oasis.opendocument.text\n");
        else if (!path.compare(path.length() -4, 4, ".oga"))
            return (void)(_contentType = "Content-Type: audio/ogg\n");
        else if (!path.compare(path.length() -4, 4, ".ogv"))
            return (void)(_contentType = "Content-Type: video/ogg\n");
        else if (!path.compare(path.length() -4, 4, ".ogx"))
            return (void)(_contentType = "Content-Type: application/ogg\n");
        else if (!path.compare(path.length() -4, 4, ".otf"))
            return (void)(_contentType = "Content-Type: font/otf\n");
        else if (!path.compare(path.length() -4, 4, ".png"))
            return (void)(_contentType = "Content-Type: image/png\n");
        else if (!path.compare(path.length() -4, 4, ".pdf"))
            return (void)(_contentType = "Content-Type: application/pdf\n");
        else if (!path.compare(path.length() -4, 4, ".ppt"))
            return (void)(_contentType = "Content-Type: application/vnd.ms-powerpoint\n");
        else if (!path.compare(path.length() -4, 4, ".rar"))
            return (void)(_contentType = "Content-Type: application/x-rar-compressed\n");
        else if (!path.compare(path.length() -4, 4, ".rtf"))
            return (void)(_contentType = "Content-Type: application/rtf\n");
        else if (!path.compare(path.length() -4, 4, ".svg"))
            return (void)(_contentType = "Content-Type: image/svg+xml\n");
        else if (!path.compare(path.length() -4, 4, ".swf"))
            return (void)(_contentType = "Content-Type: application/x-shockwave-flash\n");
        else if (!path.compare(path.length() -4, 4, ".tar"))
            return (void)(_contentType = "Content-Type: application/x-tar\n");
        else if (!path.compare(path.length() -4, 4, ".tif"))
            return (void)(_contentType = "Content-Type: image/tiff\n");
        else if (!path.compare(path.length() -4, 4, ".ttf"))
            return (void)(_contentType = "Content-Type: font/ttf\n");
        else if (!path.compare(path.length() -4, 4, ".vsd"))
            return (void)(_contentType = "Content-Type: application/vnd.visio\n");
        else if (!path.compare(path.length() -4, 4, ".wav"))
            return (void)(_contentType = "Content-Type: audio/x-wav\n");
        else if (!path.compare(path.length() -4, 4, ".xls"))
            return (void)(_contentType = "Content-Type: application/vnd.ms-excel\n");
        else if (!path.compare(path.length() -4, 4, ".xml"))
            return (void)(_contentType = "Content-Type: application/xml\n");
        else if (!path.compare(path.length() -4, 4, ".xul"))
            return (void)(_contentType = "Content-Type: application/vnd.mozilla.xul+xml\n");
        else if (!path.compare(path.length() -4, 4, ".zip"))
            return (void)(_contentType = "Content-Type: application/zip\n");
        else if (!path.compare(path.length() -4, 4, ".3gp"))
            return (void)(_contentType = "Content-Type: video/3gpp\n");
        else if (!path.compare(path.length() -4, 4, ".3g2"))
            return (void)(_contentType = "Content-Type: video/3gpp2\n");
    }
    if (path.size() >= 5)
    {
        if (!path.compare(path.length() -5, 5, ".docx"))
            return (void)(_contentType = "Content-Type: application/vnd.openxmlformats-officedocument.wordprocessingml.document\n");
        else if (!path.compare(path.length() -5, 5, ".epub"))
            return (void)(_contentType = "Content-Type: application/epub+zip\n");
         else if (!path.compare(path.length() -5, 5, ".html"))
            return (void)(_contentType = "Content-Type: text/html\n");
         else if (!path.compare(path.length() -5, 5, ".jpeg"))
            return (void)(_contentType = "Content-Type: image/jpeg\n");
         else if (!path.compare(path.length() -5, 5, ".json"))
            return (void)(_contentType = "Content-Type: application/json\n");
         else if (!path.compare(path.length() -5, 5, ".midi"))
            return (void)(_contentType = "Content-Type: audio/midi\n");
         else if (!path.compare(path.length() -5, 5, ".mpeg"))
            return (void)(_contentType = "Content-Type: video/mpeg\n");
         else if (!path.compare(path.length() -5, 5, ".mpkg"))
            return (void)(_contentType = "Content-Type: application/vnd.apple.installer+xml\n");
         else if (!path.compare(path.length() -5, 5, ".pptx"))
            return (void)(_contentType = "Content-Type: application/vnd.openxmlformats-officedocument.presentationml.presentation\n");
         else if (!path.compare(path.length() -5, 5, ".tiff"))
            return (void)(_contentType = "Content-Type: image/tiff\n");
         else if (!path.compare(path.length() -5, 5, ".weba"))
            return (void)(_contentType = "Content-Type: audio/webm\n");
         else if (!path.compare(path.length() -5, 5, ".webm"))
            return (void)(_contentType = "Content-Type: video/webm\n");
         else if (!path.compare(path.length() -5, 5, ".webp"))
            return (void)(_contentType = "Content-Type: image/webp\n");
         else if (!path.compare(path.length() -5, 5, ".woff"))
            return (void)(_contentType = "Content-Type: font/woff\n");
         else if (!path.compare(path.length() -5, 5, ".xlsx"))
            return (void)(_contentType = "Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet\n");
    }
   if (path.size() >= 6)
    {
        if (!path.compare(path.length() -6, 6, ".woff2"))
            return (void)(_contentType = "Content-Type: font/woff2\n");
        else if (!path.compare(path.length() -6, 6, ".xhtml"))
            return (void)(_contentType = "Content-Type: application/xhtml+xml\n");
    }
    else
            return (void)(_contentType = "Content-Type: text/html\n");
    }

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

// void    Response::setReturnStatus(int returnStatus) {_returnStatus = returnStatus;}
void    Response::setContent(std::string content) {_content = content;}
void    Response::setContentType(std::string contentType) {_contentType = contentType;}
void    Response::setContentLength(int contentLength) {_contentLength = contentLength;}
void    Response::setCgiPid(pid_t cgiPid) {_cgiPid = cgiPid;}
void    Response::setCgiFd(int cgiFd) {_cgiFd = cgiFd;}
void    Response::setCgiOutput(std::string cgiOutput) {_cgiOutput = cgiOutput;}
void    Response::setCgiBytesWritten(long cgiBytesWritten) {_cgiBytesWritten = cgiBytesWritten;}
void    Response::setCgiFdRessource(int cgiFdRessource) {_cgiFdRessource = cgiFdRessource;}
void    Response::setStatusCode(std::string statusCode) {_statusCode = statusCode;}
void    Response::setProtocol(std::string protocol) {_protocol = protocol;}
void    Response::setLocation(std::string location) {_location = location;}
// void    Response::setPort(uint16_t port) {_port = port;}


pid_t           Response::getCgiPid(void) const {return (_cgiPid);}
int             Response::getCgiFd(void) const {return (_cgiFd);}
std::string     Response::getCgiOutput(void) const {return (_cgiOutput);}
long            Response::getCgiBytesWritten(void) const {return (_cgiBytesWritten);}
int             Response::getCgiFdRessource(void) const {return (_cgiFdRessource);}
// int             Response::getReturnStatus() const {return (_returnStatus);}
std::string     Response::getContent() const {return (_content);}
int             Response::getContentLength() const {return (_contentLength);}
std::string     Response::getContentType() const {return (_contentType);}
std::string     Response::getStatusCode() const {return (_statusCode);}
std::string     Response::getProtocol() const {return (_protocol);}
std::string     Response::getLocation() const {return (_location);}
// uint16_t        Response::getPort() const {return (_port);}
