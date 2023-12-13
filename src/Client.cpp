#include "../inc/Client.hpp"
#include "../inc/Base.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Client::Client(void){

    return  ;
}

Client::Client(int socket, struct sockaddr_in *r_address){
    
    _new_socket = socket;
    _address = *r_address;
    _client_status = READ_READY;
    _bytes_received = 0;
    return  ;
}

Client::Client(const Client & rhs){

    *this = rhs;
    return  ;
}

Client::~Client(void){

    return ;
}

// ************************************************************************** //
//  OPERATORS
// ************************************************************************** //

Client &    Client::operator=(const Client & rhs){

    this->_new_socket = rhs._new_socket;
    this->_address = rhs._address;
    this->_received = rhs._received;
    this->_client_status = rhs._client_status;
    return *this;
}

std::ostream &  operator<<(std::ostream & o, Client const & rhs){

    o << "This client is on socket n " << rhs.get_socket() <<  " its status is actually " << rhs.display_status() << " its received contains " << rhs.get_received() << std::endl;
    return o;
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
        case READ_READY :
            return "READ_READY";
        case HEADER_READING :
            return "HEADER_READING";
        case BODY_READING :
            return "BODY_READING";
        case REQUEST_RECEIVED :
            return "REQUEST_RECEIVED";
        case RESPONSE_BUILT :
            return "RESPONSE BUILT";
        case RESPONSE_SENDING :
            return "RESPONSE_SENDING";
        case RESPONSE_SENT :
            return "RESPONSE_SENT";
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

bool    Client::found_header_end(size_t *found) const {

    if ((*found = _received.find("\r\n\r\n")) != std::string::npos)
        return true;
    return false;
}

std::string Client::curated_header(size_t end){

    std::string header = _received.substr(0, end);
    _received = _received.substr(end, _received.size());
    return header; 
}

void    Client::receive_header_data(char *buffer, int nbytes){

    size_t *found = 0;
    if (this->_client_status == READ_READY && nbytes > 0)
        this->_client_status = HEADER_READING;
    this->_received += buffer;
    this->_bytes_received += nbytes;
    if (found_header_end(found))
    {
        //Passer le header complet a Pablo _request->fillContent(string)
        //send_to_pablo(curated_header(_received));
        std::string header = curated_header(*found);
        std::cout << header << std::endl;
        std::cout << "_received still got : " << std::endl << _received << std::endl;
        this->_client_status = BODY_READING;
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


bool    Client::receive_data(void){

    char   buffer[BUFFER_SIZE + 1];

    memset(buffer, 0, BUFFER_SIZE);
    int nbytes = recv(this->_new_socket, buffer, BUFFER_SIZE, 0);
    if (nbytes == 0)
    {
        std::cout << "Client " << *this << " closed connection" << std::endl; // Handle a client closing
        return false;
    }
    else if (nbytes < 0)
    {
        std::cout << "Client " << *this << " encountered error while recv" << std::endl; // See for exception and handling of recv error
        return false;
    }
    else
    {
        if (this->_client_status == READ_READY || this->_client_status == HEADER_READING)
            this->receive_header_data(buffer, nbytes);
        else if (this->_client_status == BODY_READING)
            this->receive_body_data(buffer, nbytes);
    }
    return true;
}
