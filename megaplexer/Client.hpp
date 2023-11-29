#ifndef CLIENT_H
# define CLIENT_H

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
#include <vector>

class   Client{

    public :

        int new_socket;

        struct  sockaddr_in address;
        std::string received;

    private :
};

#endif
