#include "Client.hpp"

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

Client &    Client::operator=(const Client & rhs){

    this->new_socket = rhs.new_socket;
    this->address = rhs.address;
    this->received = rhs.received;
    return *this;
}
