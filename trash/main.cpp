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

// We'll try to implement a first baby server, in which we'll try to put in application all the documentation we've read in order to take a grasp of how a server works, TCP protocol and maybe HTTP protocol

int main(void){

    // Part in which we get the socket that we'll use to listen connections

    int server_fd;

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Error with socket\n");
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
        printf("Error binding \n");
        return (1);
    }

    while (1)
    {
        if (listen(server_fd, 10) < 0)
        {
            printf("Error listening\n");
            return(1);
        }
        int new_socket;
        size_t  addrlen = sizeof(address);
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t *)&addrlen)) < 0) 
        {
            printf("Error accepting\n");
            return(1);
        }
        char    buffer[1024] = {0};
        int valread = read(new_socket, buffer, 1024);
        write(1, buffer, strlen(buffer));
        if (valread < 0)
            write(2, "Read error\n", 11);
        char test[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 23\n\nThis project seems huge";
        write(new_socket, test, strlen(test));
        close(new_socket);
    }
    close(server_fd);
    return (0);
}

