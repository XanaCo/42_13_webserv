#include "Client.hpp"
#include "Base.hpp"

// ************************************************************************** //
//	CONSTRUCTOR / DESTRUCTOR
// ************************************************************************** //

Client::Client(void){

    return  ;
}

Client::Client(int socket, struct sockaddr_in *r_address){
    
    _new_socket = socket;
    _address = *r_address;
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
    return *this;
}

std::ostream &  operator<<(std::ostream & o, Client const & rhs){

    o << "This client is on socket n " << rhs.get_socket() << " its received contains " << rhs.get_received() << std::endl;
    return o;
}

// ************************************************************************** //
//  OPERATORS
// ************************************************************************** //

void    run(std::string strRequest, std::vector<ServerInfo> serverList)
{

    // si jamais on a fini de recevoir la requete mais qu'on a rien construit en reponse
    _request.resetValues();
    _response.resetValues();

    if (!_request.fillContent(strRequest))
    {
        std::cerr << "au secours, on parse mal" << std::endl;
        return ;
    }
    if (!this->findHost(serverList))
    {
        std::cerr << "on ne trouve pas l'ost, qui est le responsable ?" << std::endl;
        return ;
    }
    std::string path;
    if (!this->findRessource(path))
    {
        std::cerr << "on ne trouve par la ressource, qui a mange le dernier carre de chocolat ?" << std::endl;
        return ;
    }

	std::vector<Location> loca = this->_server->getLocations();
	loca.getLAllowed();

    this->readRessource(path);

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

// void    Client::setReturnStatus(Request request)
// {
//     _request = request;
// }

// Request    Client::getReturnStatus(void) const
// {
//     return (_request);
// }

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
