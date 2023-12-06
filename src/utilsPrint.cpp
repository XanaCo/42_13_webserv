
#include "../inc/webserv.hpp"

void	printStringVector(std::vector<std::string> stringVector) {

	for (size_t i = 0; i < stringVector.size(); i++)
	{
		std::cout << stringVector[i] << std::endl;
		std::cout << " ---------- " << std::endl;
	}

}

void	printServersInfo(std::vector<ServerInfo> stringVector) {

	for (size_t i = 0; i < stringVector.size(); i++)
	{
		std::cout << stringVector[i] << std::endl;
		std::cout << " ---------- " << std::endl;
	}
}

std::string	returnStringVector(std::vector<std::string> stringVector) {

	std::string res;

	for (size_t i = 0; i < stringVector.size(); i++)
		res += stringVector[i] + " | ";

	return res;
}
