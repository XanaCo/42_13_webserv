
#include "../inc/FileParser.hpp"

void eraseComments(std::string &content) {

	size_t	pos_start;
	size_t	pos_end;

	pos_start = content.find('#');
	while (pos_start != std::string::npos)
	{
		pos_end = content.find('\n', pos_start);
		content.erase(pos_start, pos_end - pos_start);
		pos_start = content.find('#');
	}

}

std::vector<std::string> cSplitLine(std::string line, char const *charset) {

	std::vector<std::string> res;
	size_t	pos_start = 0;
	size_t	pos_end  = 0;

	for ( ; ; )
	{
		pos_start = line.find_first_not_of(charset, pos_end);
		if (pos_start == std::string::npos)
			break ;
		pos_end = line.find_first_of(charset, pos_start);
		res.push_back(line.substr(pos_start, pos_end - pos_start));
	}

	return res;
}

void	signalHandler(int signal) {
	
	if (signal == SIGPIPE)
		return;
	else
		throw FileParser::SignalExit();
}

void	printStringVector(std::vector<std::string> stringVector) {

	for (size_t i = 0; i < stringVector.size(); i++)
		std::cout << stringVector[i] << std::endl;

}

void	printServersInfo(std::vector<ServerInfo *> stringVector) {

	for (size_t i = 0; i < stringVector.size(); i++)
		std::cout << stringVector[i] << std::endl;
}

char hexToChar(const std::string& hex) {
    std::istringstream converter(hex);
    int value;
    converter >> std::hex >> value;
    return static_cast<char>(value);
}

// ************************************************************************** //
//	CONTROLE QUALITE DU PATH DE LA RESSOURCE DEMANDEE
// ************************************************************************** //

bool	containsParentDirectory(const std::string& str)
{
	int	size = str.length();

	for (std::string::size_type i = 0; i < size; i++)
	{
        if (str.substr(i, 3) == "../")
            return (true);
    }
    return (false);
}

void compressionOfSlashes(std::string& str)
{
    for (std::string::iterator i = str.begin(); i != str.end(); )
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

bool isAlphaDigit(char c)
{
    return ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

char hexToChar(const std::string& hex)
{
    std::istringstream converter(hex);
    int value;
    converter >> std::hex >> value;
    return (static_cast<char>(value));
}

void remplacerPercentEncoding(std::string& chaine)
{
    std::string				resultat;
	int						size = chaine.lenght();
    std::string::size_type	i = 0;

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
