#include "../inc/Base.hpp"

// Constructors

Base::Base() : _servers(0), _clients(0), _pfds(0), _sock_count(0){
    if (PRINT)
        std::cout << BASE << "🐥 constructor called" << std::endl;
    return ;
}

/*
Base::Base(int argc, char **argv) : _clients(0), _sock_count(0){
    for (int i = 0; i + 1 < argc; i++)
    {
        this->add_to_servers(argv[i + 1]);
        this->add_to_poll_in(_servers[i].getSocket());
        std::cout << _servers[i];
    }
    std::cout << std::endl << std::endl;
    return ;
}*/

Base::Base(std::vector<ServerInfo> & Servers) :  _clients(0), _pfds(0), _sock_count(0){
    if (PRINT)
        std::cout << BASE << "🐥 constructor called" << std::endl;
    this->_servers = Servers;
}

Base::Base(const Base & rhs) {
    *this = rhs; 
}


// Destructors

Base::~Base(){
   
    for (unsigned int i = 0; i < _clients.size(); i++)
    {
        delete _clients[i]->getRequest();
        delete _clients[i]->getResponse();
        delete _clients[i];
    }
    _servers.clear();
    _clients.clear();
    for (unsigned int i = 0; i < _pfds.size(); i++)
        close(_pfds[i].fd);
    _pfds.clear();
	if (PRINT)
		std::cout << BASE << "🗑️  destructor called" << std::endl;
    return ;
}

// Operator overload

Base &  Base::operator=(const Base & rhs) { 
    this->_servers = rhs._servers; 
    this->_clients = rhs._clients; 
    this->_pfds = rhs._pfds; 
    return *this;
}

// Methods

// These functions are used to add into our 3 vectors depending of a port number or socket number

/*
void    Base::add_to_servers(char *port){
    
    ServerInfo  server(port);

    this->_servers.push_back(server);
}*/

void    Base::add_to_clients(int socket, struct sockaddr_in* address, std::vector<ServerInfo> servers){

    Client  *tmp = new  Client(socket, address, servers, this);

    this->_clients.push_back(tmp);
}

void    Base::add_to_poll_in(int socket)
{
    struct  pollfd  pfd;

    pfd.fd = socket;
    pfd.events = POLLIN;
    pfd.revents = 0;
    this->_pfds.push_back(pfd);
    this->_sock_count++;
}

void    Base::add_to_poll_out(int socket)
{
    struct  pollfd  pfd;

    pfd.fd = socket;
    pfd.events = POLLOUT;
    pfd.revents = 0;
    this->_pfds.push_back(pfd);
    this->_sock_count++;
}

// These functions are used to remove items from our 3 vectors

void    Base::remove_from_servers(int socket){

    for(unsigned int i = 0; i < this->_servers.size() ; i++)
    {
        if (socket == _servers[i].getSocket())
        {
            //std::cout << "Server n " << socket << " trying to be erased" << std::endl;
            _servers.erase(_servers.begin() + i);
            //std::cout << "Server n " << socket << " erased" << std::endl;
            break ;
        }
    }
}

void    Base::remove_from_clients(int socket){

    for(unsigned int i = 0; i < this->_clients.size() ; i++)
    {
        if (socket == _clients[i]->get_socket())
        {
            //std::cout << "Client n " << socket << " trying to be erased" << std::endl;
            delete _clients[i];
            _clients.erase(_clients.begin() + i);
            //std::cout << "Client n " << socket << " erased" << std::endl;
            break ;
        }
    }
}

void    Base::remove_from_poll(int socket){

    for(unsigned int i = 0; i < this->_pfds.size() ; i++)
    {
        if (socket == _pfds[i].fd)
        {
            //std::cout << "fd n " << socket << " trying to be erased from poll" << std::endl;
            close(_pfds[i].fd);
            _pfds.erase(_pfds.begin() + i);
            this->_sock_count--;
            //std::cout << "fd n " << socket << " erased from poll" << std::endl;
            break ;
        }
    }
}

bool    Base::is_first_server(int lim, std::string port){

    int count = 0;

    for (int i = 0; i <= lim; i++)
    {
        if(_servers[i].getListen() == port)
            count++;
        if (count > 1)
            return false;
    }
    return true;
}

ServerInfo &    Base::get_first_server(const ServerInfo & curr){

    for (size_t i = 0; i < _servers.size(); i++)
    {
        if (_servers[i].getListen() == curr.getListen())
            return (_servers[i]);
    }
    return (_servers[0]);
}

void    Base::assign_socket_same_port(ServerInfo & curr, ServerInfo & same){

    curr.setSameListen(same.getSocket()); 
    return ;
}

bool    Base::set_servers_sockets(void){

    for (size_t i = 0; i < this->_servers.size(); i++)
    {
        if (is_first_server(i, _servers[i].getListen()))
        {
            if (!_servers[i].setListenSocket(_servers[i].getListen()))
                return false;
            add_to_poll_in(_servers[i].getSocket());
            std::cout << "Server named : " << _servers[i].getServerName() << " listening on port " << _servers[i].getListen() << " and socket n " << _servers[i].getSocket() << std::endl;
        }
        else
        {
            assign_socket_same_port(_servers[i], get_first_server(_servers[i]));
            std::cout << "Server named : " << _servers[i].getServerName() << " listening on port " << _servers[i].getListen() << " and socket n " << _servers[i].getSocket() << std::endl;
        }
    }
    return true;
}

// function used to accept new connection on listening socket

void    Base::handle_new_connection(int serv_sock)
{
    struct sockaddr_storage remoteaddr;
    int new_fd;
    socklen_t   addrlen;
    char    remoteIP[INET6_ADDRSTRLEN];

    addrlen = sizeof remoteaddr;
    new_fd = accept(serv_sock, (struct sockaddr*)&remoteaddr, &addrlen);
    if (new_fd == -1)
        std::cout << "Error accepting connection" << std::endl; //See with Ana for exception
    else
    {
        this->add_to_poll_in(new_fd);
        std::cout << "New connection from " << inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr), remoteIP, INET6_ADDRSTRLEN) << " on socket n " << new_fd << std::endl; 
        this->add_to_clients(new_fd, (struct sockaddr_in*)this->get_in_sockaddr((struct sockaddr*)&remoteaddr), this->_servers); //Add the adress to clients and maybe see the same for server but i think ana did it
        std::cout << get_cli_from_sock(new_fd) << std::endl;
    }
}

// function used to change a poll event

void    Base::change_poll_event(int socket, int event){

    struct pollfd   *tmp;

    tmp = get_poll_from_sock(socket);
    switch (event)
    {
        case pollout :
            tmp->events = POLLOUT;
            break ;
        case pollin :
            tmp->events = POLLIN;
            break ;
        default :
            break ;
    }
    return ;
}

// function used to receive data on a client
/*
void    Base::receive_client_data(int client_sock){

    Client  &client = get_cli_from_sock(client_sock);
    char   buffer[BUFFER_SIZE + 1];

    int nbytes = recv(client.new_socket, buffer, sizeof buffer, 0);

    if (nbytes <= 0)
    {
            std::cout << "Socket " << client.new_socket << " closed connection or recv failed" << std::endl;
            remove_from_poll(client_sock);
            remove_from_clients(client_sock);
            return ;
    }
    client.received += buffer;
    change_poll_event(client_sock, pollout);
    std::cout << client.get_received() << std::endl;
}*/

// These functions are used to return sockaddr_in or sockaddr_in->sin_addr

void*   Base::get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void*   Base::get_in_sockaddr(struct sockaddr *sa){

    if (sa->sa_family == AF_INET){
        return (((struct sockaddr_in*)sa));
    }
    return (((struct sockaddr_in6*)sa));

}

// This function is used to identify if a given fd is a listening fd

bool    Base::is_a_server(int socket)
{
    for (unsigned int i = 0; i < this->_servers.size(); i++)
    {
        if (_servers[i].getSocket() == socket)
            return true;
    }
    return false;
}

// This function is used to identify if a given fd is a client fd

bool    Base::is_a_client(int socket)
{
    for (unsigned int i = 0; i < this->_clients.size(); i++)
    {
        if (_clients[i]->get_socket() == socket)
            return true;
    }
    return false;
}

// function used to return a client/poll ref from its socket

Client &    Base::get_cli_from_sock(int client_sock){

    unsigned int i = 0;
    while (i < this->_clients.size())
    {
        if (_clients[i]->get_socket() == client_sock)
            break ;
        i++;
    }
    return *(_clients[i]);
}

ServerInfo &    Base::get_serv_from_sock(int sock){

    unsigned int i = 0;
    while (i < this->_servers.size())
    {
        if (_servers[i].getSocket() == sock)
            break ;
        i++;
    }
    return _servers[i];
}


struct pollfd    *Base::get_poll_from_sock(int client_sock){

    unsigned int i = 0;
    while (i < this->_pfds.size())
    {
        if (_pfds[i].fd == client_sock)
            break ;
        i++;
    }
    return (&_pfds[i]);
}

// main loop that only call the poll reviewer

void    Base::start_servers(void) {


    if(!this->set_servers_sockets())
        return ;
    while (1)
    {
       int  poll_count = poll(this->_pfds.data(), this->_pfds.size(), -1); 
       //std::cout << "poll is called" << std::endl;
       if (poll_count == -1)
       {
           std::cout << "Error in polling sockets" << std::endl;
           break ;
       }
       this->review_poll();
    }
}

// function used to review the poll of fd

void    Base::review_poll(void){

    // (void)char test[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 45\n\nPablo va bientot m'envoyer de belles reponses";

    for (int i = 0; i < this->_sock_count; i++)
    {
        if (_pfds[i].revents & POLLIN)
        {
            if (is_a_server(_pfds[i].fd))
                handle_new_connection(_pfds[i].fd);
            else
            {
                if(get_cli_from_sock(_pfds[i].fd).receive_data())
                {
                    get_cli_from_sock(_pfds[i].fd).run(_servers);
                    change_poll_event(_pfds[i].fd, pollout);
                }
                else
                {
                    remove_from_clients(_pfds[i].fd);
                    remove_from_poll(_pfds[i].fd);
                }
            }
        }
        if(_pfds[i].revents & POLLOUT)
        {
            if (get_cli_from_sock(_pfds[i].fd).send_data())
                change_poll_event(_pfds[i].fd, pollin);
            else
                {
                    remove_from_clients(_pfds[i].fd);
                    remove_from_poll(_pfds[i].fd);
                }
        }
    }
}
