#include "../inc/Client.hpp"
#include "../inc/Base.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Client::Client(void){
    if (PRINT)
        std::cout << CLIENT << "🐥 constructor called" << std::endl;
    _request = new Request;
    std::cout << "adresse de ma Request :" << _request << std::endl;
    _response = new Response;
    return  ;
}

Client::Client(int socket, struct sockaddr_in *r_address, std::vector<ServerInfo> servers, Base  *base){
    if (PRINT)
        std::cout << CLIENT << "🐥 constructor called" << std::endl;
    _new_socket = socket;
    _address = *r_address;
    _client_status = WANT_TO_RECEIVE_REQ;
    _bytes_received = 0;
    _header_bytes = 0;
    _body_bytes = 0;
    _max_body_size = 0;
    _req_end = false;
    _request = NULL;
    _response = NULL;
    _servers = servers; 
    _base = base;
    _server = NULL;
    _fdRessource = 0;
    _first_chunk = true;
    _chunk_index_type = CHUNK_SIZE;
    return  ;
}

Client::Client(const Client & rhs){
    if (PRINT)
        std::cout << CLIENT << "constructor called" << std::endl;
    *this = rhs;
    return  ;
}

Client::~Client(void){

    delete _request;
    delete _response;
	if (PRINT)
		std::cout << CLIENT << "🗑️  destructor called" << std::endl;

    return ;
}

// ************************************************************************** //
//  OPERATORS
// ************************************************************************** //

Client &    Client::operator=(const Client & rhs){
    if (this != &rhs)
    {
        this->_new_socket = rhs._new_socket;
        this->_address = rhs._address;
        this->_received = rhs._received;
        this->_client_status = rhs._client_status;
    }
    return *this;
}

std::ostream &  operator<<(std::ostream & o, Client const & rhs){

    o << "This client is on socket n " << rhs.get_socket() <<  " its status is actually " << rhs.display_status() << " its received contains " << rhs.get_received() << std::endl;
    return o;
}

// ************************************************************************** //
//  |               |
//  v METHODS PABLO v
// ************************************************************************** //

bool    Client::parseCgiExit()
{
    std::vector<std::string>    lines = splitString(_body, '\n');

    int size = lines.size();
    for (int i = 0; i < size; i++)
    {
        int length = lines[0].size();
        if (length >= 13 && lines[i].substr(0, 13) == "content-type:")
            _response->setContentType(lines[i]);
        else if (length >= 12 && lines[i].substr(0, 12) == "status-code:")
            _response->setContentType(lines[i]);
        else if (length >= 8 && lines[i].substr(0, 8) == "protocol:")
            _response->setProtocol(lines[i]);
        else if (length >= 6 && lines[i].substr(0, 6) == "<html>")
        {
            _response->setContent(lines[i]);
            for (; i < size; i++)
            {
                std::string newContent = _response->getContent() + "\n" + lines[i];
                _response->setContent(newContent);
                if (lines[i] == "</html>")
                    return (true);
            }
        }
    }
    return (false);
}

void    Client::openErrorPage()
{
    int status = _request->getReturnStatus();

    if (status == E_NOT_FOUND)
        _fdRessource = open("../site/errorPages/error404.html", O_RDONLY);
    else if (E_BAD_REQUEST <= status  && status < E_INTERNAL_SERVER)
        _fdRessource = open("../site/errorPages/error400.html", O_RDONLY);
    else
        _fdRessource = open("../site/errorPages/error500.html", O_RDONLY);

}

bool    Client::checkHttpVersion()
{
    std::string version = _request->getVersion();
    if (version != "HTTP/1.0" && version != "HTTP/1.1" && version != "HTTP/0.9")
    {
        _response->setReturnStatus(E_HTTP_VERSION);
        return (false);
    }
    return (true);
}

ServerInfo*    Client::findServer()
{
    if (_servers.size() == 1)
        return (&(_servers[0]));

    for (std::vector<ServerInfo>::iterator i = _servers.begin(); i != _servers.end(); i++)
    {
        if (i->getServerName() == _request->getHost())
            return &(*i);
    }
    return &(_servers[0]);
    // throw exception ?
}

bool    Client::getRes()
{
    std::string path;

    if (_fdRessource < 3)
    {
        std::string contentType;
        _response->resetValues();
        if (_request->getReturnStatus() != 200)
        {
            openErrorPage();
            contentType = "Content-Type: text/html";
            _response->setContentType(contentType);
        }
        /*else
        {
            if (_request->getPath().size() > 5)
            {
                if (!_request->getPath().compare(_request->getPath().length() - 5, 5, ".html"))
                {
                    // contentType = "Content-Type: text/html";
                    _response->setContentType("Content-Type: text/html");
                }
                else if (!_request->getPath().compare(_request->getPath().length() - 4, 4, ".mp4"))
                {
                    // contentType = "Content-Type: video/mp4";
                    _response->setContentType("Content-Type: video/mp4");
                }
                else if (!_request->getPath().compare(_request->getPath().length() - 4, 4, ".css"))
                {
                    contentType = "Content-Type: text/css";
                    _response->setContentType("Content-Type: text/css");
                }
                else if (!_request->getPath().compare(_request->getPath().length() - 4, 4, ".png"))
                {
                    // contentType = "Content-Type: image/png";
                    _response->setContentType("Content-Type: image/png");
                }
                else if (!_request->getPath().compare(_request->getPath().length() - 5, 5, ".jpeg"))
                {
                    // contentType = "Content-Type: image/jpeg";
                    _response->setContentType("Content-Type: image/jpeg");
                }
                else if (!_request->getPath().compare(_request->getPath().length() - 4, 4, ".css"))
                {
                    // contentType = "Content-Type: text/css";
                    _response->setContentType("Content-Type: text/css");
                }
            }
        }*/
        if (_request->getPath().find("/CGI/scriptCGI/") != std::string::npos)
        {
            if (!(_request->getMethod() & GET) && !(_request->getMethod() & POST))
            {
                std::cerr << "Run : methode interdite avec les CGI" << std::endl;

                openErrorPage();
            }
            if (!_server->findCgiRessource(_request->getPath(), path))
            {
                std::cerr << "Run : on ne trouve pas la ressource CGI" << std::endl;

                openErrorPage();
            }
            //exec cgi
            Cgi cgi(*_server, *_request, *_response);
            cgi.executeScript();    // pas oublier de renseigner le fd de la pipe
            _fdRessource = _response->getCgiFdRessource();
        }
        else
        {
            // resoudre le path
            if (!_server->findRessource(_request->getPath(), path))
            {
                // la ressource n'a pas ete trouvee 404
                // ouvrir un fd de la loose
            }
            /*if (_server->getOneLocation(_request->getPath())->getLAutoindex())
                return (this->_response->craftAutoIndex(path));*/ // handle the auto index and handle the fact that there could be a default file too
            _fdRessource = open(path.c_str(), O_RDONLY);
            if (_fdRessource < 0)
            {
                // ouvrir un fichier de la loose
            }

        }
        if (_fdRessource < 0)
        {
            std::cerr << "Open : on ne peut pas ouvrir le fichier" << std::endl;
            _request->setReturnStatus(500);
            openErrorPage();
        }
    }
    if (_request->getPath().size() >= 5)
    {
        if (!_request->getPath().compare(_request->getPath().length() - 5, 5, ".html"))
        {
                // contentType = "Content-Type: text/html";
            _response->setContentType("Content-Type: text/html\n");
        }
        else if (!_request->getPath().compare(_request->getPath().length() - 4, 4, ".mp4"))
        {
                // contentType = "Content-Type: video/mp4";
            _response->setContentType("Content-Type: video/mp4\n");
        }
        else if (!_request->getPath().compare(_request->getPath().length() - 4, 4, ".png"))
        {
                // contentType = "Content-Type: image/png";
            _response->setContentType("Content-Type: image/png\n");
        }
        else if (!_request->getPath().compare(_request->getPath().length() - 5, 5, ".jpeg") || !_request->getPath().compare(_request->getPath().length() - 5, 5, ".jpg"))
        {
                // contentType = "Content-Type: image/jpeg";
            _response->setContentType("Content-Type: image/jpeg\n");
        }
        else if (!_request->getPath().compare(_request->getPath().length() - 4, 4, ".css"))
        {
                // contentType = "Content-Type: text/css";
            _response->setContentType("Content-Type: text/css\n");
        }
    }
    return (_response->readRessource(_fdRessource));
}

bool    Client::postRes()
{
    std::string path;

    if (_fdRessource < 3)
    {
        _response->resetValues();
        if (_request->getReturnStatus() != 200)
        {
            this->openErrorPage();
        }
        if (_request->getPath().find("/CGI/") != std::string::npos)
        {
            if (!_server->findCgiRessource(_request->getPath(), path))
            {

                std::cerr << "Run : on ne trouve pas la ressource CGI" << std::endl;
                _request->setReturnStatus(404);
                this->openErrorPage();
            }
            //exec cgi
            Cgi cgi(*_server, *_request, *_response);
            cgi.executeScript();
            // pas oublier de renseigner le fd de la pipe
            _fdRessource = _response->getCgiFdRessource() ;
        }
        else
        {
            // resoudre le path
            if (!_server->findRessource(_request->getPath(), path))
            {
                _request->setReturnStatus(404);
                this->openErrorPage();
            }
            _fdRessource = open(path.c_str(), O_RDONLY);
            if (_fdRessource > 2)
                _response->postRessource(path, _request->getBody());   // penser a mettre a jour le status du retour

        }
        if (_fdRessource < 0)
        {
            std::cerr << "Open error: can't open file" << std::endl;
            _request->setReturnStatus(500);
            this->openErrorPage();
        }
    }
    if (_response->readRessource(_fdRessource))
    {
        if (_request->getPath().find("/CGI/") != std::string::npos)
            this->parseCgiExit();
        return (true);
    }
    return (false);
}

bool    Client::deleteRes()
{
    std::string path;

    if (_fdRessource < 3)
    {
        _response->resetValues();
        if (_request->getReturnStatus() != 200)
        {
            this->openErrorPage();
        }
        else
        {
            // resoudre le path
            if (!_server->findRessource(_request->getPath(), path))
            {
                _request->setReturnStatus(404);
                this->openErrorPage();
            }
            _fdRessource = open(path.c_str(), O_RDONLY);
            if (_fdRessource < 0)
            {
                _request->setReturnStatus(500);
                this->openErrorPage();
            }
            _response->deleteRessource(path);
            // open un fichier du success
        }
        if (_fdRessource < 0)
        {
            _request->setReturnStatus(500);
            this->openErrorPage();
        }
    }
    return (_response->readRessource(_fdRessource));
}

bool    Client::executeMethod()
{
    // if (_client_status == REQ_RECEIVED)
    // {
        // _server = this->findServer();
    // }
    int method = _request->getMethod() >> 1;

    return ((this->*(Client::methodFunctions()[method]))());
}

void    Client::routine(int nbytes)
{
    switch (_client_status)
    {
        case RECEIVED_REQ_HEADER:
        {
            _request->resetValues();
            if (!_request->fillHeader(_header)) // + faire des verifs et en fonction mettre a jour la variable de routine
            {
                _client_status = WAITING_FOR_RES;
                this->_base->change_poll_event(this->_new_socket, pollout);
                return ;
            }
            else
            {
                if (this->_req_end == true)
                {
                    _server = this->findServer();
                    _client_status = WAITING_FOR_RES;
                    this->_base->change_poll_event(this->_new_socket, pollout);
                    return ;
                }
                else if (!this->_req_end && nbytes < 1024)
                {
                    _client_status = REQ_RECEIVED;
                    _body.append(_received);
                }
                else
                {
                    if (this->_request->getChunkTransf())
                        receive_chunked_body_data((char *)_received.c_str(), nbytes);
                    else
                        _client_status = RECEIVING_REQ_BODY;
                }
            }
            _server = this->findServer();
            if (_client_status != REQ_RECEIVED)
                return ;
        }
        case REQ_RECEIVED:
        {
            _request->fillBody(_body); // + faire des verifs et en fonction mettre a jour la variable de routine
            _client_status = WAITING_FOR_RES;
            this->_base->change_poll_event(this->_new_socket, pollout);
            return ;
        }
        case WAITING_FOR_RES:
        {
            //std::cout << "Je suis en train de construire la reponse" << std::endl;
            if (this->executeMethod())
                _client_status = RES_READY_TO_BE_SENT;
        }
    }
}

// ************************************************************************** //
//  ^               ^
//  | METHODS PABLO |
// ************************************************************************** //

std::string    Client::display_status(void) const{

    switch (this->_client_status)
    {
        case WANT_TO_RECEIVE_REQ :
            return "WANT_TO_RECEIVE_REQ";
        case RECEIVING_REQ_HEADER :
            return "RECEIVING_REQ_HEADER";
        case RECEIVING_REQ_BODY :
            return "RECEIVING_REQ_BODY";
        case REQ_RECEIVED :
            return "REQ_RECEIVED";
        case WAITING_FOR_RES :
            return "WAITING_FOR_RES";
        case RES_READY_TO_BE_SENT :
            return "RES_READY_TO_BE_SENT";
        case SENDING_RES_HEADER :
            return "SENDING_RES_HEADER";
        case SENDING_RES_BODY :
            return "SENDING_RES_BODY";
        case UPLOADING_FILE :
            return "UPLOADING_FILE";
        case RES_SENT :
            return "RES_SENT";
        case ERROR_WHILE_SENDING :
            return "ERROR_WHILE_SENDING";
        default :
            return "NO STATUS";
    }
}

// Work in progress

bool    Client::alloc_req_resp(void){ // A proteger et a delete si on satisfait a la requete et la reponse, revoir au niveau de la creation du client, les destructeurs sont appeles en sortant du scope, peut etre allouer les clients... i don t know

    if (this->_request || this->_response)
        return false;
    /*if (this->_request || this->_response)
        delete this->_request;
    if (this->_response)
        delete this->_response;*/
    this->_request = new Request;
    this->_response = new Response;
    return true;
}

void    Client::reset_client(void){

    this->_bytes_received = 0;
    this->_header_bytes = 0;
    this->_body_bytes = 0;
    this->_bytes_to_send = 0;
    this->_bytes_sent = 0;
    this->_received = "";
    this->_header = "";
    this->_body = "";
    this->_to_send = "";
    this->_chunk_pool = "";
    this->_first_chunk = true;
    this->_chunk_index_type = CHUNK_SIZE;
    this->_req_end = false;
    return ;
}

bool    Client::receive_data(void){

    char   buffer[BUFFER_SIZE + 1];

    if (this->_client_status == WANT_TO_RECEIVE_REQ)
    {
        this->alloc_req_resp();
        this->reset_client();
    }
    memset(buffer, 0, BUFFER_SIZE);
    int nbytes = recv(this->_new_socket, buffer, BUFFER_SIZE, 0);
    if (nbytes == 0)
    {
        getactualTimestamp();
        std::cout << "Client " << this->get_socket() << " closed connection" << std::endl; // Handle a client closing
        return false;
    }
    else if (nbytes < 0)
    {
        getactualTimestamp();
        std::cout << "Client " << *this << " encountered error while recv" << std::endl; // See for exception and hgetandling of recv error
        return false;
    }
    /*else
    {
        _received += buffer;
        //std::cout << _received << std::endl;
    }*/
    else 
    {
        if (this->_client_status == WANT_TO_RECEIVE_REQ || this->_client_status == RECEIVING_REQ_HEADER)
            this->receive_header_data(buffer, nbytes);
        else if (this->_client_status == RECEIVING_REQ_BODY && this->_request->getChunkTransf())
            this->receive_chunked_body_data(buffer, nbytes);
        else if (this->_client_status == RECEIVING_REQ_BODY)
            this->receive_body_data(buffer, nbytes);
    }
    return true;
}


//////////////////////////////////////////////////////////////////////
//                                                                  //  
//            W.I.P. fonctions d envoi/reception                    //  
//            a mettre en place une fois qu on aura quelque         //     
//            chose qui marche                                      //
//                                                                  //  
//////////////////////////////////////////////////////////////////////

bool    Client::found_header_end(size_t *found) const {

    *found = _received.find("\r\n\r\n");
    if ( *found != std::string::npos)
        return true;
    return false;
}

std::string Client::curated_header(size_t end){

    std::string header = _received.substr(0, end + 4);
    _received = _received.substr((end + 4), _received.size());
    return header; 
}

void    Client::receive_header_data(char *buffer, int nbytes){

    size_t found = 0;
    if (this->_client_status == WANT_TO_RECEIVE_REQ && nbytes > 0)
        this->_client_status = RECEIVING_REQ_HEADER;
    this->_received.append(buffer, nbytes);
    this->_bytes_received += nbytes;
    if (found_header_end(&found))
    {
        _header = curated_header(found);
        _header_bytes = found + 4;
        //std::cout << "Header : "<< _header << std::endl;
        //std::cout << std::endl << "_received still got : " << std::endl << _received << std::endl;
        getactualTimestamp();
        std::cout << "Client n : " << this->_new_socket << " sent a request." << std::endl;
        if (this->_received.size() > 0)
        {
            this->_client_status = RECEIVED_REQ_HEADER;
            this->routine(nbytes);
        }
        else
        {
            this->_client_status = RECEIVED_REQ_HEADER;
            this->_req_end = true;
            this->routine(nbytes);
            //this->_base->change_poll_event(this->_new_socket, pollout);
        }
        return ;
    }
//    std::cout << this->_received << std::endl;
    return ;
}

void    Client::receive_chunked_body_data(char *buffer, int nbytes){

    size_t         chunk_size = 0;
    this->_chunk_index_type = CHUNK_SIZE;
    if (this->_first_chunk && this->_received.size())
    {
        _chunk_pool.clear();
        _chunk_pool.append(_received);
        _bytes_received -= _received.size();
        _received.clear();
        this->_first_chunk = false;
    }
    _chunk_pool.append(buffer);
    if (_chunk_pool.find("\r\n") == std::string::npos)
        return ;
    std::vector<std::string>    chunk_vec = cSplitLine(_chunk_pool, "\r\n");
    for (size_t i = 0; i < chunk_vec.size(); i++)
    {
        if (this->_chunk_index_type == CHUNK_SIZE)
        {
            chunk_size = hexStrToInt(chunk_vec[i]);
            if (i + 1 < chunk_vec.size())
                this->_chunk_index_type = CHUNK_DATA;
        }
        else if (this->_chunk_index_type == CHUNK_DATA)
        {
            if (chunk_vec[i].size() == chunk_size)
            {
                _received.append(chunk_vec[i]);
                _chunk_pool = _chunk_pool.substr(_chunk_pool.find(chunk_vec[i]) + chunk_size + 2);
                _bytes_received += chunk_size;
                this->_chunk_index_type = CHUNK_SIZE;
            }
            else if (chunk_vec[i].size() != chunk_size && i != (chunk_vec.size() - 1))
            {
                this->_client_status = REQ_RECEIVED;
                this->_request->setReturnStatus(400);
                this->_chunk_index_type = CHUNK_SIZE;
                this->_first_chunk = true;
                this->routine(nbytes);
                return ;
            }
            else if (chunk_vec[i].size() != chunk_size && i == (chunk_vec.size() - 1))
            {
                if (nbytes == 1024)
                    return ;
                else
                {
                    this->_client_status = REQ_RECEIVED;
                    this->_request->setReturnStatus(400);
                    this->_chunk_index_type = CHUNK_SIZE;
                    this->_first_chunk = true;
                    return ;
                }
            }
        }
    }
    if (chunk_size == 0)
    {
        this->_first_chunk = true;
        this->_client_status = REQ_RECEIVED;
        _chunk_pool.clear();
    }
    return ;
}

void    Client::receive_body_data(char *buffer, int nbytes){

    this->_received.append(buffer, nbytes);
    this->_bytes_received += nbytes;
    this->_body_bytes += nbytes;
    if (nbytes < BUFFER_SIZE)
    {
        this->_client_status = REQ_RECEIVED;
        this->routine(nbytes);
        //this->_base->change_poll_event(this->_new_socket, pollout);
        return ;
    }
    if (this->_request->getContentLength() > 0)
    {
        if (this->_body_bytes == this->_request->getContentLength())
        {
            this->_client_status = REQ_RECEIVED;
            this->routine(nbytes);
            //this->_base->change_poll_event(this->_new_socket, pollout);
            return ;
        }
    }
    return ;
}

std::string Client::make_temp_header(void){

        int cont_len = this->getResponse()->getContent().size();
        std::stringstream con;
        con << cont_len;
        std::string to_send = "HTTP/1.1 200 OK\n" + this->getResponse()->getContentType() + "Content-Length: " + con.str() + "\n\n" + this->getResponse()->getContent() + "\r\n\r\n";
        return (to_send);
}

/*bool    Client::send_data(void)
{
    if (this->_client_status == WAITING_FOR_RES)
        this->routine();
    else
    {
        std::string to_send = make_temp_header();
        int len = strlen(to_send.c_str());
        if (!send_all(this->_new_socket, to_send.c_str(), &len))
        {
            std::cout << "Only " << len << " bytes have been sent because of error" << std::endl;
            return false;
        }
        this->_base->change_poll_event(this->_new_socket, pollin);
    }
    return true;
}*/


bool    Client::send_data(void)
{
    if (this->_client_status == WAITING_FOR_RES)
        this->routine(0);
    else
    {
        if (!send_partial(this->_new_socket))
        {
            std::cout << "Only " << _bytes_sent << " bytes have been sent because of error" << std::endl;
            return false;
        }
    }
    return true;
}

bool    Client::send_partial(int socket){

    int sent = 0;
    static bool craft_header = false;
    if (!craft_header)
    {
        _to_send = make_temp_header();
        _bytes_to_send = _to_send.size();
        craft_header = true;
    }
    if (_bytes_to_send - _bytes_sent > BUFFER_SIZE)
        sent = send(socket, _to_send.c_str() + _bytes_sent, BUFFER_SIZE, 0);
    else
        sent = send(socket, _to_send.c_str() + _bytes_sent, _bytes_to_send - _bytes_sent, 0);
    if (sent == -1)
    {
        this->_client_status = ERROR_WHILE_SENDING;
        return false;
    }
    _bytes_sent += sent;
    if (_bytes_sent == _bytes_to_send)
    {
        craft_header = false;
        this->_client_status = WANT_TO_RECEIVE_REQ;
        this->_base->change_poll_event(this->_new_socket, pollin);
    }
    return true;
}


bool    Client::send_all(int s, const char *buf, int *len){

    int total = 0;
    int b_left = *len;
    int n;

    while(total < *len)
    {
        n = send(s, buf+total, b_left, 0);
        if (n == -1)
            break;
        total += n;
        b_left -= n;
    }
    *len = total;
    if (n == -1)
        return false;
    return true;
}

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

int                 Client::get_socket(void) const {return this->_new_socket;}
struct sockaddr_in  Client::get_addr_struct(void) const {return this->_address;}
std::string         Client::get_received(void) const {return this->_received;}
int                 Client::get_status(void) const {return this->_client_status;}
int                 Client::get_bytes_received(void) const {return this->_bytes_received;}
bool                Client::get_req_end(void) const {return this->_req_end;}
Request*            Client::getRequest(void) const {return _request;}
Response*           Client::getResponse(void) const {return _response;}
int                 Client::getFdRessource(void) const {return _fdRessource;}
ServerInfo*         Client::getServer(void) const {return _server;}

void    Client::set_socket(int sock) {this->_new_socket = sock;}
void    Client::set_addr_struct(struct sockaddr_in addr) {this->_address = addr;}
void    Client::set_received(std::string buf) {this->_received = buf;}
void    Client::set_status(int status) {this->_client_status = status;}
void    Client::set_bytes_received(int nbytes) {this->_bytes_received = nbytes;}
void    Client::set_req_end(bool end){this->_req_end = end;}
void    Client::setFdRessource(int fd) {_fdRessource = fd;}
void    Client::setServer(ServerInfo* server) {_server = server;}
