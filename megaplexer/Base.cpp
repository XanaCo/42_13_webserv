#include "Base.hpp"

Base::Base() : servers(0), clients(0), pfds(0), sock_count(0){ 
    return ;
}

Base::Base(int argc, char **argv) : clients(0), sock_count(0){
    for (int i = 0; i + 1 < argc; i++)
    {
        this->add_to_servers(argv[i + 1]);
        this->add_to_poll_in(servers[i].listen_socket);
        std::cout << servers[i];
    }
    std::cout << std::endl << std::endl;
    return ;
}

Base::Base(const Base & rhs) {
    *this = rhs; 
}

Base::~Base(){
    return ;
}

Base &  Base::operator=(const Base & rhs) { 
    this->servers = rhs.servers; 
    this->clients = rhs.clients; 
    this->pfds = rhs.pfds; 
    return *this;
}

void    Base::add_to_servers(char *port){
    
    Server  server(port);

    this->servers.push_back(server);
}

void    Base::add_to_poll_in(int socket)
{
    struct  pollfd  pfd;

    pfd.fd = socket;
    pfd.events = POLL_IN;
    this->pfds.push_back(pfd);
    this->sock_count++;
}
