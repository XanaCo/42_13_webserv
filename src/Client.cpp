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
//  METHODS
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

void    Client::run(std::vector<ServerInfo> serverList)
{
    ServerInfo  server;
    // _request->resetValues();
    // _response->resetValues();
    if (!_request->fillContent(_received))
    {
        std::cerr << "error, on a du mal a fill la structure Request" << std::endl;
        return ;
    }
    std::cout << "Run : ma requete ressemble a ca : " << (*_request) << std::endl;
    _request->findHost(serverList, server);
    std::cout << "Run : voici l'host : " << server.getServerName() << std::endl;
    std::string path;
    if (_request->getPath().find("/CGI/") != std::string::npos)
    {
        if (!server.findCgiRessource(_request->getPath(), path))
            std::cerr << "Run : on ne trouve pas la ressource CGI" << std::endl;
        //exec cgi
        Cgi cgi_le_2(server, *_request, *_response);
        cgi_le_2.executeScript();
        return ;
    }
    if (!server.findRessource(_request->getPath(), path))
    {
        std::cerr << "Run : on ne trouve par la ressource" << std::endl;
        return ;
    }
    std::cout << "Run : je trouve ce chemin de ressource : " << path << std::endl;


    int    method = _request->getMethod() / 2; // ici c'est pour avoir 0->GET / 1->POST / 2->DELETE
    std::cout << "Run : method trouvee : " << method << " (0->GET/1->POST/2->DELETE)" <<std::endl;

    static void (Response::*methods[3])(const std::string) = { &Response::readRessource, NULL, &Response::deleteRessource };
    if (method == 1)
        _response->postRessource(path, _request->getBody());
    else
        (_response->*methods[method])(path);
    std::cout << std::endl << *_response << std::endl;
}

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

int Client::get_socket(void) const{
    
    return this->_new_socket;
} 

struct sockaddr_in Client::get_addr_struct(void) const{
    
    return this->_address;
}

std::string Client::get_received(void) const{
    
    return this->_received;
}

int Client::get_status(void) const{
    
    return this->_client_status;
}

int Client::get_bytes_received(void) const{

    return this->_bytes_received;
}

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

void    Client::set_socket(int sock){

    this->_new_socket = sock;
    return ;
}

void    Client::set_addr_struct(struct sockaddr_in addr){

    this->_address = addr;
    return ;
}

void    Client::set_received(std::string buf){

    this->_received = buf;
    return ;
}

void    Client::set_status(int status){

    this->_client_status = status;
    return ;
}

void    Client::set_bytes_received(int nbytes){

    this->_bytes_received = nbytes;
}

// void    Client::setReturnStatus(Request request)
// {
//     _request = request;
// }

// Request    Client::getReturnStatus(void) const
// {
//     return (_request);
// }

// Work in progress

bool    Client::alloc_req_resp(void){ // A proteger et a delete si on satisfait a la requete et la reponse, revoir au niveau de la creation du client, les destructeurs sont appeles en sortant du scope, peut etre allouer les clients... i don t know

    this->_request = new Request;
    this->_response = new Response;
    return true;
}

bool    Client::receive_data(void){

    char   buffer[BUFFER_SIZE + 1];

    if (this->_client_status == WANT_TO_RECEIVE_REQ)
        this->alloc_req_resp();
    memset(buffer, 0, BUFFER_SIZE);
    int nbytes = recv(this->_new_socket, buffer, BUFFER_SIZE, 0);
    if (this->_client_status == WANT_TO_RECEIVE_REQ)
        this->_bytes_received = 0;
        //Mettre fonction de reset du client
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
    else
    {
        _received += buffer;
        std::cout << _received << std::endl;
    }
    /*else A remettre en place apres
    {
        if (this->_client_status == WANT_TO_RECEIVE_REQ || this->_client_status == RECEIVING_REQ_HEADER)
            this->receive_header_data(buffer, nbytes);
        else if (this->_client_status == RECEIVING_REQ_BODY)
            this->receive_body_data(buffer, nbytes);
    }*/
    return true;
}

Request*   Client::getRequest(void) {return _request;}
Response*   Client::getResponse(void) {return _response;}

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
        this->_client_status = RECEIVING_REQ_BODY;
        //_request->fillContent(_header + _body)
        return ;
    }
//    std::cout << this->_received << std::endl;
    return ;
}

void    Client::receive_body_data(char *buffer, int nbytes){


    this->_received += buffer;
    this->_bytes_received += nbytes;
    return ;
}



