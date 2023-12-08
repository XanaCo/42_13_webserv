
#include "../inc/webserv.hpp"

void	printStringVector(std::vector<std::string> stringVector) {

	for (size_t i = 0; i < stringVector.size(); i++)
		std::cout << stringVector[i] << std::endl;
}

void	printServersInfo(std::vector<ServerInfo> stringVector) {

	for (size_t i = 0; i < stringVector.size(); i++)
	{
		std::cout << " ---------- " << std::endl;
		std::cout << stringVector[i] << std::endl;
		printStringVector(stringVector[i].getErrorPages());
		printServerLocations(stringVector[i]);
	}
}

std::string	returnStringVector(std::vector<std::string> stringVector) {

	std::string res;

	for (size_t i = 0; i < stringVector.size(); i++)
		res += stringVector[i] + " | ";

	return res;
}

void	printServerLocations(ServerInfo Server) {

	std::vector<Location> loc = Server.getLocations();

	for (size_t i = 0; i < loc.size(); i++)
	{
		std::cout << loc[i] << std::endl;
		std::vector<std::string> locCGI = loc[i].getLCgi();
		printStringVector(locCGI);
	}

}
