#include <iostream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
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

// We'll try to implement a first baby server, in which we'll try to put in application all the documentation we've read in order to take a grasp of how a server works, TCP protocol and maybe HTTP protocol

int main(void){

    // Part in which we get the socket that we'll use to listen connections

    int server_fd;

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        strerror(errno);
        return (1);
    }

    // Part in which we'll bind the socket to our IP Address

    struct sockaddr_in  address;
    const int   PORT = 8080;
    memset((char *)&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        strerror(errno);
        return (1);
    }
    return (0);
}

