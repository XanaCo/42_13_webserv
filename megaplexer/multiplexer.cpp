#include "Base.hpp"
#include "Client.hpp"
#include "Server.hpp"

#define MAX_CLIENTS 1024
#define PORT "8080"





int main(int argc, char **argv){

    if (argc < 2)
        return (std::cout << "Use as ./a.out 'PORT' 'PORT' " << std::endl, 1);
    std::cout << "Welcome to the best web server ever ..." << std::endl << std::endl;
    Base    base(argc, argv);
    while(1)
    {

    }
    return (0);
}
