#include "Client.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Client::Client(void){

    return  ;
}

Client::Client(int socket, struct sockaddr_in *r_address){
    
    new_socket = socket;
    address = *r_address;
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

    this->new_socket = rhs.new_socket;
    this->address = rhs.address;
    this->received = rhs.received;
    return *this;
}

std::ostream &  operator<<(std::ostream & o, Client const & rhs){

    o << "This client is on socket n " << rhs.new_socket << " its received contains " << rhs.received << std::endl;
    return o;
}

// ************************************************************************** //
//	LA GET-SET
// ************************************************************************** //

// void    Client::setReturnStatus(Request request)
// {
//     _request = request;
// }

// Request    Client::getReturnStatus(void) const
// {
//     return (_request);
// }
