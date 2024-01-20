
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

void deEncodingHexa(std::string& chaine)
{

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

std::string getNameDir(const std::string& path)
{
    size_t lastSep = path.find_last_of('/');
    
    if (lastSep != std::string::npos && path[1])
    {
        if (lastSep == 0)
            return path;
        return path.substr(0, lastSep);
    }
    else
        return ".";
}

std::string getNameFile(const std::string& path)
{
    size_t lastSep = path.find_last_of('/');

    // Si aucun séparateur n'est trouvé, le chemin est probablement juste le nom de fichier
    if (lastSep == std::string::npos)
        return path;
    return (path.substr(lastSep + 1));
}

void	removeBackSlashR(std::string& str)
{
    std::string::size_type r_pos = str.find('\r');
    while (r_pos != std::string::npos)
    {
        str.erase(r_pos, 1);
        r_pos = str.find('\r', r_pos);
    }
}

void    closeFile(int* fd)
{
    if (*fd > 2)
    {
        close(*fd);
        *fd = 0;
    }
}

bool isHtmlComplete(const std::string& html)
{
    int c = 0;

    for (std::size_t i = 0; i < html.size(); ++i)
    {
        if (html[i] == '<')
            ++c;
        else if (html[i] == '>')
        {
            --c;
            if (c < 0)
                return false;
        }
    }
    return c == 0;
}

int identifyFile(const std::string& str) // voir si on retourne une erreur en cas de ni .php ni py
{
    if (str.length() >= 3)
	{
        if (str.compare(str.length() - 3, 3, ".py") == 0)
		{
            return PY;
        }
    }
    return PHP;
}

/*bool    getactualTimestamp(long *actual){

    struct timeval tv;

    if(gettimeofday(&tv, NULL) < 0)
        return false;



    
}*/
