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
    _request = NULL;
    _response = NULL;
    _servers = servers; 
    _base = base;
    _server = NULL;
    _fdRessource = 0;
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

bool    Client::checkHttpVersion(void)
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
        _response->resetValues();
        if (_request->getReturnStatus() != 200)
        {
            // recup un fd de la loose


        }
        if (_request->getPath().find("/CGI/") != std::string::npos)
        {
            if (!_server->findCgiRessource(_request->getPath(), path))
                std::cerr << "Run : on ne trouve pas la ressource CGI" << std::endl;
            //exec cgi
            Cgi cgi(*_server, *_request, *_response);
            cgi.executeScript();    // pas oublier de renseigner le fd de la pipe
            // _fdRessource = ;
        }
        else
        {
            // resoudre le path
            if (!_server->findRessource(_request->getPath(), path))
            {
                // la ressource n'a pas ete trouvee 404
            }
            _fdRessource = open(path.c_str(), O_RDONLY);
            if (_fdRessource < 0)
            {
                // ouvrir un fichier de la loose
            }

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
            // recup un fd de la loose
        }
        if (_request->getPath().find("/CGI/") != std::string::npos)
        {
            if (!_server->findCgiRessource(_request->getPath(), path))
                std::cerr << "Run : on ne trouve pas la ressource CGI" << std::endl;
            //exec cgi
            Cgi cgi(*_server, *_request, *_response);
            cgi.executeScript();    // pas oublier de renseigner le fd de la pipe
            // _fdRessource = ;
        }
        else
        {
            // resoudre le path
            if (!_server->findRessource(_request->getPath(), path))
            {
                // la ressource n'a pas ete trouvee 404
            }
            _fdRessource = open(path.c_str(), O_RDONLY);
            if (_fdRessource < 0)
            {
                // ouvrir un fichier de la loose
            }
            _response->postRessource(path, _request->getBody());   // penser a mettre a jour le status du retour

        }

    }
    return (_response->readRessource(_fdRessource));
}

bool    Client::deleteRes()
{
    std::string path;

    if (_fdRessource < 3)
    {
        _response->resetValues();
        if (_request->getReturnStatus() != 200)
        {
            // recup un fd de la loose
        }
        else
        {
            // resoudre le path
            if (!_server->findRessource(_request->getPath(), path))
            {
                // la ressource n'a pas ete trouvee 404
            }
            _fdRessource = open(path.c_str(), O_RDONLY);
            if (_fdRessource < 0)
            {
                // ouvrir un fichier de la loose
            }
            _response->deleteRessource(path);
            // open un fichier du success
        }

    }
    return (_response->readRessource(_fdRessource));
}

bool    Client::executeMethod()
{
    if (_client_status == REQ_RECEIVED)
    {
        _server = this->findServer();
    }
    int method = _request->getMethod() >> 1;

    return ((this->*(Client::methodFunctions()[method]))());
}

void    Client::routine()
{
    switch (_client_status)
    {
        case RECEIVED_REQ_HEADER:
        {
            _request->resetValues();
            if (!_request->fillHeader(_header)) // + faire des verifs et en fonction mettre a jour la variable de routine
                _client_status = WAITING_FOR_RES;
            else
                _client_status = RECEIVING_REQ_BODY;
            return ;
        }
        case REQ_RECEIVED:
        {
            _request->fillBody(_body); // + faire des verifs et en fonction mettre a jour la variable de routine
            _client_status = WAITING_FOR_RES;
            return ;
        }
        case WAITING_FOR_RES:
        {
            if (this->executeMethod())
            {  // trouver le server + check la method + recup un buffer
                _client_status = RES_READY_TO_BE_SENT;
                this->_base->change_poll_event(this->_new_socket, pollout);
            }
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
    this->_received = "";
    this->_header = "";
    this->_body = "";
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
        std::cout << "Client " << this->get_socket() << " closed connection" << std::endl; // Handle a client closing
        return false;
    }
    else if (nbytes < 0)
    {
        std::cout << "Client " << *this << " encountered error while recv" << std::endl; // See for exception and handling of recv error
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
    this->_received += buffer;
    this->_bytes_received += nbytes;
    if (found_header_end(&found))
    {
        //Passer le header complet a Pablo _request->fillContent(string)
        //send_to_pablo(curated_header(_received));
        _header = curated_header(found);
        _header_bytes = found + 4;
        std::cout << "Header : "<< _header << std::endl;
        std::cout << std::endl << "_received still got : " << std::endl << _received << std::endl;
        if (this->_received.size() > 0)
        {
            this->_client_status = RECEIVING_REQ_BODY;
            this->routine();
        }
        else
        {
            this->_client_status = REQ_RECEIVED;
            this->routine();
            this->_base->change_poll_event(this->_new_socket, pollout);
        }
        return ;
    }
//    std::cout << this->_received << std::endl;
    return ;
}

void    Client::receive_body_data(char *buffer, int nbytes){

    this->_received += buffer;
    this->_bytes_received += nbytes;
    this->_body_bytes += nbytes;
    if (nbytes < BUFFER_SIZE)
    {
        this->_client_status = REQ_RECEIVED;
        this->routine();
        //this->_base->change_poll_event(this->_new_socket, pollout);
        return ;
    }
    if (this->_request->getContentLenght() > 0)
    {
        if (this->_body_bytes == this->_request->getContentLenght())
        {
            this->_client_status = REQ_RECEIVED;
            this->routine();
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
            std::string to_send = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + con.str() + "\n\n" + this->getResponse()->getContent() + "\r\n\r\n";
            return (to_send);
}

bool    Client::send_data(void)
{
            std::string to_send = make_temp_header();
            int len = strlen(to_send.c_str());
            if (!send_all(this->_new_socket, to_send.c_str(), &len))
            {
                std::cout << "Only " << len << " bytes have been sent because of error" << std::endl;
                return false;
            }
            return true;
}
/*
bool    Client::send_data(void)
{ 
            std::string to_send = this->_response->getContent();
            int len = strlen(to_send.c_str());
            if (!send_all(this->_new_socket, to_send.c_str(), &len))
            {
                std::cout << "Only " << len << " bytes have been sent because of error" << std::endl;
                return false;
            }
            return true;
}*/

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
Request*            Client::getRequest(void) const {return _request;}
Response*           Client::getResponse(void) const {return _response;}
int                 Client::getFdRessource(void) const {return _fdRessource;}
ServerInfo*         Client::getServer(void) const {return _server;}

void    Client::set_socket(int sock) {this->_new_socket = sock;}
void    Client::set_addr_struct(struct sockaddr_in addr) {this->_address = addr;}
void    Client::set_received(std::string buf) {this->_received = buf;}
void    Client::set_status(int status) {this->_client_status = status;}
void    Client::set_bytes_received(int nbytes) {this->_bytes_received = nbytes;}
void    Client::setFdRessource(int fd) {_fdRessource = fd;}
void    Client::setServer(ServerInfo* server) {_server = server;}
