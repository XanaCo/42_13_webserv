
#include "../inc/webserv.hpp"

int	findMethod(std::string str)
{
	if (str.substr(0, 3) == "GET")
		return (GET);
	if (str.substr(0, 4) == "POST")
		return (POST);
	if (str.substr(0, 6) == "DELETE")
		return (DELETE);
	std::cout << "parsing request : Method not found" << std::endl;
	return (42);
}

std::string	findHost(std::string str)
{

	
}
