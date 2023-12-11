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

// void    Client::setReturnStatus(Request request)
// {
//     _request = request;
// }

// Request    Client::getReturnStatus(void) const
// {
//     return (_request);
// }

// Work in progress

bool    Client::receive_data(void){

    char   buffer[BUFFER_SIZE + 1];

    int nbytes = recv(this->_new_socket, buffer, sizeof buffer, 0);

    if (nbytes <= 0)
    {
            std::cout << "Socket " << this->_new_socket << " closed connection or recv failed" << std::endl;
            return false;
    }
    this->_received += buffer;
    std::cout << this->_received << std::endl;
    return true;
}
