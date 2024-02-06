#include "../inc/Base.hpp"

// Constructors

Base::Base() : _servers(0), _clients(0), _pfds(0), _sock_count(0){

    this->init_mime_types();
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

    this->init_mime_types();
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
        //delete _clients[i]->getRequest();
        //delete _clients[i]->getResponse();
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

void    Base::init_mime_types(void){

    _mime_types[".html"] = "Content-Type: text/html\n";
    _mime_types[".css"] = "Content-Type: text/css\n";
    _mime_types[".ico"] = "Content-Type: image/x-icon\n";
    _mime_types[".jpeg"] = "Content-Type: image/jpeg\n";
    _mime_types[".jpg"] = "Content-Type: image/jpeg\n";
    _mime_types[".mpeg"] = "Content-Type: video/mpeg\n";
    _mime_types[".htm"] = "Content-Type: text/html\n";
    _mime_types[".bz"] = "Content-Type: application/x-bzip\n";
    _mime_types[".js"] = "Content-Type: application/javascript\n";
    _mime_types[".sh"] = "Content-Type: application/x-sh\n";
    _mime_types[".ts"] = "Content-Type: application/typescript\n";
    _mime_types[".7z"] = "Content-Type: application/x-7z-compressed\n";
    _mime_types[".aac"] = "Content-Type: audio/aac\n";
    _mime_types[".abw"] = "Content-Type: application/x-abiword\n";
    _mime_types[".arc"] = "Content-Type: application/octet-stream\n";
    _mime_types[".avi"] = "Content-Type: video/x-msvideo\n";
    _mime_types[".azw"] = "Content-Type: application/vnd.amazon.ebook\n";
    _mime_types[".bin"] = "Content-Type: application/octet-stream\n";
    _mime_types[".bmp"] = "Content-Type: image/bmp\n";
    _mime_types[".bz2"] = "Content-Type: application/x-bzip2\n";
    _mime_types[".csh"] = "Content-Type: application/x-csh\n";
    _mime_types[".csv"] = "Content-Type: text/csv\n";
    _mime_types[".doc"] = "Content-Type: application/msword\n";
    _mime_types[".eot"] = "Content-Type: application/vnd.ms-fontobject\n";
    _mime_types[".gif"] = "Content-Type: image/gif\n";
    _mime_types[".ics"] = "Content-Type: text/calendar\n";
    _mime_types[".jar"] = "Content-Type: application/java-archive\n";
    _mime_types[".mid"] = "Content-Type: audio/midi\n";
    _mime_types[".odp"] = "Content-Type: application/vnd.oasis.opendocument.presentation\n";
    _mime_types[".ods"] = "Content-Type: application/vnd.oasis.opendocument.spreadsheet\n";
    _mime_types[".odt"] = "Content-Type: application/vnd.oasis.opendocument.text\n";
    _mime_types[".oga"] = "Content-Type: audio/ogg\n";
    _mime_types[".ogv"] = "Content-Type: video/ogg\n";
    _mime_types[".ogx"] = "Content-Type: application/ogg\n";
    _mime_types[".otf"] = "Content-Type: font/otf\n";
    _mime_types[".png"] = "Content-Type: image/png\n";
    _mime_types[".pdf"] = "Content-Type: application/pdf\n";
    _mime_types[".ppt"] = "Content-Type: application/vnd.ms-powerpoint\n";
    _mime_types[".rar"] = "Content-Type: application/x-rar-compressed\n";
    _mime_types[".rtf"] = "Content-Type: application/rtf\n";
    _mime_types[".svg"] = "Content-Type: image/svg+xml\n";
    _mime_types[".swf"] = "Content-Type: application/x-shockwave-flash\n";
    _mime_types[".tar"] = "Content-Type: application/x-tar\n";
    _mime_types[".tif"] = "Content-Type: image/tiff\n";
    _mime_types[".ttf"] = "Content-Type: font/ttf\n";
    _mime_types[".vsd"] = "Content-Type: application/vnd.visio\n";
    _mime_types[".wav"] = "Content-Type: audio/x-wav\n";
    _mime_types[".xls"] = "Content-Type: application/vnd.ms-excel\n";
    _mime_types[".xml"] = "Content-Type: application/xml\n";
    _mime_types[".xul"] = "Content-Type: application/vnd.mozilla.xul+xml\n";
    _mime_types[".zip"] = "Content-Type: application/zip\n";
    _mime_types[".3gp"] = "Content-Type: video/3gpp\n";
    _mime_types[".3g2"] = "Content-Type: video/3gpp2\n";
    _mime_types[".docx"] = "Content-Type: application/vnd.openxmlformats-officedocument.wordprocessingml.document\n";
    _mime_types[".epub"] = "Content-Type: application/epub+zip\n";
    _mime_types[".json"] = "Content-Type: application/json\n";
    _mime_types[".midi"] = "Content-Type: audio/midi\n";
    _mime_types[".mpkg"] = "Content-Type: application/vnd.apple.installer+xml\n";
    _mime_types[".pptx"] = "Content-Type: application/vnd.openxmlformats-officedocument.presentationml.presentation\n";
    _mime_types[".tiff"] = "Content-Type: image/tiff\n";
    _mime_types[".weba"] = "Content-Type: audio/webm\n";
    _mime_types[".webm"] = "Content-Type: video/webm\n";
    _mime_types[".webp"] = "Content-Type: image/webp\n";
    _mime_types[".woff"] = "Content-Type: font/woff\n";
    _mime_types[".xlsx"] = "Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet\n";
    _mime_types[".woff2"] = "Content-Type: font/woff2\n";
    _mime_types[".xhtml"] = "Content-Type: application/xhtml+xml\n";
    return ;
}

void    Base::add_to_clients(int socket, struct sockaddr_in* address, std::vector<ServerInfo> servers, int serv_sock){

    Client  *tmp = new  Client(socket, address, servers, this, serv_sock);

    // tmp->set_max_body_size(this->get_serv_from_sock(serv_sock).getMaxClientBody()); // a remetre plus tard
    this->_clients.push_back(tmp);
}

void    Base::add_to_poll_in(int socket)
{
    struct  pollfd  pfd;

    pfd.fd = socket;
    pfd.events = POLLIN | POLLERR | POLLHUP;
    pfd.revents = 0;
    this->_pfds.push_back(pfd);
    this->_sock_count++;
}

void    Base::add_to_poll_out(int socket)
{
    struct  pollfd  pfd;

    pfd.fd = socket;
    pfd.events = POLLOUT | POLLERR;
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
            if (_clients[i]->getFdRessource() > 2)
                close(_clients[i]->getFdRessource());
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

std::map<std::string, std::string> &Base::getMimeType(void) {

	return _mime_types;
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
            std::cout << BLUE << "Server named : " << _servers[i].getServerName() << " listening on port " << _servers[i].getListen() << " and socket n " << _servers[i].getSocket() << END_COLOR << std::endl << std::endl;
        }
        else
        {
            assign_socket_same_port(_servers[i], get_first_server(_servers[i]));
            std::cout << BLUE << "Server named : " << _servers[i].getServerName() << " listening on port " << _servers[i].getListen() << " and socket n " << _servers[i].getSocket() << END_COLOR << std::endl << std::endl;
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
        getactualTimestamp();
        std::cout << YELLOW << "New connection from " << inet_ntop(remoteaddr.ss_family, get_in_addr((struct sockaddr*)&remoteaddr), remoteIP, INET6_ADDRSTRLEN) << " on socket n " << new_fd << END_COLOR << std::endl; 
        this->add_to_clients(new_fd, (struct sockaddr_in*)this->get_in_sockaddr((struct sockaddr*)&remoteaddr), this->_servers, serv_sock); //Add the adress to clients and maybe see the same for server but i think ana did it
        //std::cout << get_cli_from_sock(new_fd) << std::endl;
    }
}

// function used to change a poll event
void    Base::change_poll_event(int socket, int event){

    struct pollfd   *tmp;

    tmp = get_poll_from_sock(socket);
    switch (event)
    {
        case pollout :
            tmp->events = POLLOUT | POLLERR;
            break ;
        case pollin :
            tmp->events = POLLIN | POLLERR | POLLHUP;
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
           getactualTimestamp();
           std::cout << "Error in polling sockets" << std::endl;
           break ;
       }
       this->review_poll();
    }
}

// function used to review the poll of fd

bool    Base::client_is_timed_out(Client & client){

    if (is_timedout(client.get_timestamp(), client.get_timeout()))
    {
        getactualTimestamp();
        std::cout << "Client n : " << client.get_socket() << " has been timed out. Connexion Closed." << std::endl; 
        return true;
    }
    return false;
}

void    Base::review_poll(void){

    for (int i = 0; i < this->_sock_count; i++)
    {
        if (_pfds[i].revents & POLLIN)
        {
            if (is_a_server(_pfds[i].fd))
                handle_new_connection(_pfds[i].fd);
            else
            {
                if(/*client_is_timed_out(get_cli_from_sock(_pfds[i].fd)) ||*/ !get_cli_from_sock(_pfds[i].fd).receive_data())
                {
                    remove_from_clients(_pfds[i].fd);
                    remove_from_poll(_pfds[i].fd);
                }
            }
        }
        if(_pfds[i].revents & POLLOUT)
        {
            if (/*client_is_timed_out(get_cli_from_sock(_pfds[i].fd)) ||*/ !get_cli_from_sock(_pfds[i].fd).send_data())
            {
                remove_from_clients(_pfds[i].fd);
                remove_from_poll(_pfds[i].fd);
            }
        }
        if(_pfds[i].revents & POLLERR || _pfds[i].revents & POLLHUP)
        {
                remove_from_clients(_pfds[i].fd);
                remove_from_poll(_pfds[i].fd);
        }
    }
}
