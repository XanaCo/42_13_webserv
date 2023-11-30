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

void    Base::add_to_clients(int socket, struct sockaddr_in* address){

    Client  client(socket, address);

    this->clients.push_back(client);
}

void    Base::add_to_poll_in(int socket)
{
    struct  pollfd  pfd;

    pfd.fd = socket;
    pfd.events = POLL_IN;
    this->pfds.push_back(pfd);
    this->sock_count++;
}

void    Base::add_to_poll_out(int socket)
{
    struct  pollfd  pfd;

    pfd.fd = socket;
    pfd.events = POLL_OUT;
    this->pfds.push_back(pfd);
    this->sock_count++;
}

bool    Base::is_a_server(int socket)
{
    for (unsigned int i = 0; i < this->servers.size(); i++)
    {
        if (servers[i].listen_socket == socket)
            return true;
    }
    return false;
}

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

Client &    Base::get_cli_from_sock(int client_sock){

    unsigned int i = 0;
    while (i < this->clients.size())
    {
        if (clients[i].new_socket == client_sock)
            break ;
        i++;
    }
    return clients[i];
}

void    Base::receive_client_data(int client_sock){

    Client  client(get_cli_from_sock(client_sock));

    int nbytes = recv(client.new_socket, client.buffer, sizeof client.buffer, 0);

    if (nbytes <= 0)
    {
        std::cout << "Socket " << client.new_socket << " closed connection or recv failed" << std::endl;
    }
    client.received += client.buffer;
    std::cout << client.received << std::endl;
}

void    Base::check_read_operations(void){

    for (int i = 0; i < this->sock_count; i++)
    {
        if (pfds[i].revents & POLLIN)
        {
            if (is_a_server(pfds[i].fd))
                handle_new_connection(pfds[i].fd);
            else
                receive_client_data(pfds[i].fd);
                //std::cout << "client fd n " << pfds[i].fd << "ready to send data" << std::endl;
        }
    }
}

void    Base::review_poll(void){

    this->check_read_operations();
    //this->check_send_operations();


}

void    Base::start_servers(void) {

    while (1)
    {
       int  poll_count = poll(this->pfds.data(), this->pfds.size(), -1); 
       if (poll_count == -1)
       {
           std::cout << "Error in polling sockets" << std::endl;
           break ;
       }
       this->review_poll();
    }
}
