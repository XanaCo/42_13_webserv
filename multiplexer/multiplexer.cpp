#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <dirent.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <cstdio>

#define MAX_CLIENTS 1024


class   Client{

    public :

        int socket;

        struct  sockaddr_in address;
        std::string received;

    private :
};

class   Server{

    public :

    int listen_socket;
    int const   port;
    struct sockaddr_in address; 

    private :

};

int main(void){



}
