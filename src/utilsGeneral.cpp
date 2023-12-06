
#include "../inc/webserv.hpp"

// ************************************************************************** //
//	SIGNALS
// ************************************************************************** //

void	signalHandler(int signal) {
	
	if (signal == SIGPIPE)
		return;
	else
		throw FileParser::SignalExit();
}

// ************************************************************************** //
//	CONTROLE QUALITE DU PATH DE LA RESSOURCE DEMANDEE
// ************************************************************************** //

bool	containsParentDirectory(const std::string& str) {

	size_t	size = str.length();

	for (size_t i = 0; i < size; i++)
	{
		if (str.substr(i, 3) == "../")
			return (true);
	}

	return (false);
}

void compressionOfSlashes(std::string& str) {

	for (std::string::iterator i = str.begin(); i != str.end(); ) // check if ++ is missing
	{
		if (*i == '/' && *(i + 1) == '/')
			i = str.erase(i + 1);
		else
			i++;
	}
}

// ************************************************************************** //
//	DE-ENCODAGE DE L'URI
// ************************************************************************** //

bool isAlphaDigit(char c) {

	return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

char hexToChar(const std::string& hex) {

	std::istringstream converter(hex);
	int value;

	converter >> std::hex >> value;
	
	return (static_cast<char>(value));
}

void remplacerPercentEncoding(std::string& chaine) {

	std::string	resultat;
	size_t		size = chaine.length();
	size_t		i = 0;

	while (i < size)
	{
		if (chaine[i] == '%' && i + 2 < size && isAlphaDigit(chaine[i + 1]) && isAlphaDigit(chaine[i + 2]))
		{
			std::string hex = chaine.substr(i + 1, 2);
			resultat += hexToChar(hex);
			i += 3;
		}
		else
		{
			resultat += chaine[i];
			++i;
		}
	}
	chaine = resultat;
}
