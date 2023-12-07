#include "Base.hpp"

// Constructors

Base::Base() : _servers(0), _clients(0), _pfds(0), _sock_count(0){ 
    return ;
}

Base::Base(int argc, char **argv) : _clients(0), _sock_count(0){
    for (int i = 0; i + 1 < argc; i++)
    {
        this->add_to_servers(argv[i + 1]);
        this->add_to_poll_in(_servers[i].get_socket());
        std::cout << _servers[i];
    }
    std::cout << std::endl << std::endl;
    return ;
}

Base::Base(const Base & rhs) {
    *this = rhs; 
}


// Destructors

Base::~Base(){
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

void    Base::add_to_servers(char *port){
    
    Server  server(port);

    this->_servers.push_back(server);
}

void    Base::add_to_clients(int socket, struct sockaddr_in* address){

    Client  client(socket, address);

    this->_clients.push_back(client);
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
        if (socket == _servers[i].get_socket())
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
        if (socket == _clients[i].get_socket())
        {
            //std::cout << "Client n " << socket << " trying to be erased" << std::endl;
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
        std::cout << "Error accepting connection" << std::endl;
    else
    {
        this->add_to_poll_in(new_fd);
        std::cout << "New connection from " << inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr), remoteIP, INET6_ADDRSTRLEN) << " on socket n " << new_fd << std::endl; 
        this->add_to_clients(new_fd, (struct sockaddr_in*)this->get_in_sockaddr((struct sockaddr*)&remoteaddr));
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


// This functions call send until the number of bytes to send is sent or error happened
bool    Base::send_all(int s, char *buf, int *len){

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
        if (_servers[i].get_socket() == socket)
            return true;
    }
    return false;
}

// This function is used to identify if a given fd is a client fd

bool    Base::is_a_client(int socket)
{
    for (unsigned int i = 0; i < this->_clients.size(); i++)
    {
        if (_clients[i].get_socket() == socket)
            return true;
    }
    return false;
}

// function used to return a client/poll ref from its socket

Client &    Base::get_cli_from_sock(int client_sock){

    unsigned int i = 0;
    while (i < this->_clients.size())
    {
        if (_clients[i].get_socket() == client_sock)
            break ;
        i++;
    }
    return _clients[i];
}

Server &    Base::get_serv_from_sock(int sock){

    unsigned int i = 0;
    while (i < this->_servers.size())
    {
        if (_servers[i].get_socket() == sock)
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

    while (1)
    {
       int  poll_count = poll(this->_pfds.data(), this->_pfds.size(), -1); 
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

    char test[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 45\n\nPablo va bientot m'envoyer de belles reponses";

    for (int i = 0; i < this->_sock_count; i++)
    {
        if (_pfds[i].revents & POLLIN)
        {
            if (is_a_server(_pfds[i].fd))
                handle_new_connection(_pfds[i].fd);
            else
            {
                if(!get_cli_from_sock(_pfds[i].fd).receive_data())
                {
                    remove_from_clients(_pfds[i].fd);
                    remove_from_poll(_pfds[i].fd);
                }
                change_poll_event(_pfds[i].fd, pollout);
            }
        }
        if(_pfds[i].revents & POLLOUT)
        {
            int len = strlen(test);

            if (!send_all(_pfds[i].fd, test, &len))
                std::cout << "Only " << len << " bytes have been sent because of error" << std::endl;
            change_poll_event(_pfds[i].fd, pollin);
        }
    }
}
