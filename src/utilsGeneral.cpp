
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

// std::string getFirstNameDir(const std::string& path)
// {
//	 size_t lastSep = path.find('/');
		
//	 if (lastSep != std::string::npos && path[1])
//	 {
//		 if (lastSep == 0)
//			 return path;
//		 return path.substr(0, lastSep);
//	 }
//	 else
//		 return ".";
// }

std::string getNameDir(const std::string& path)
{
	size_t lastSep = path.find_last_of('/');
		
	if (lastSep != std::string::npos && path[1])
	{
		if (lastSep == 0 && path.find(".") != std::string::npos)
			return "/";
        else if (lastSep == 0)
            return path;
		return path.substr(0, lastSep);
	}
	else
		return "/";
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

void	closeFile(int* fd)
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

int hexStrToInt(const std::string& hexStr)
{
	std::istringstream iss(hexStr);
	int num;
	iss >> std::hex >> num;
	return num;
}

void getactualTimestamp(void){

	std::time_t actualTime = std::time(0);
	std::tm *localTime = std::localtime(&actualTime);

	std::cout << localTime->tm_hour << ":" << localTime->tm_min << ":" << localTime->tm_sec << " ";
	return ;
}

unsigned long    get_micro_time_stamp(void){

    struct timeval  tv;

    gettimeofday(&tv, NULL);
    return ((tv.tv_sec * 1000000) + tv.tv_usec);
}

bool            is_timedout(unsigned long start, int timeout){

    unsigned long   now;

    now = get_micro_time_stamp();
    if ((now - start) / 1000000 >= (unsigned long)timeout)
        return true;
    return false;
}

// ************************************************************************** //
//	COOKIES
// ************************************************************************** //

// std::string	random_file_generator()
// {
// 	unsigned char	buffer[8];
// 	int				fd;
// 	unsigned long	nbr;
// 	std::string     file_name;

// 	fd = open("/dev/urandom", O_RDONLY);
// 	if (fd < 0)
// 		fd = open("/dev/random", O_RDONLY);
// 	if (fd < 0)
// 		return ("");
// 	else
// 	{
// 		read(fd, buffer, 8);
// 		close(fd);
// 		memcpy(&nbr, buffer, 8);
// 	}
// 	file_name = std::to_string(nbr);
// 	if (file_name == "")
// 		return ("");
// 	return (file_name);
// }

// faire une fonction qui check si il y a pas un fichier qui porte le nom du cookie
// auquel cas le cookie a deja ete attribue a qq1
// bool	ft_get_random_name(std::string& name)
// {
// 	while (1)
// 	{
// 		name = random_file_generator();
// 		if (name == "")
// 			return (false);
// 		if (access(name.c_str(), F_OK))
// 			break ;
// 	}
// 	return (true);
// }

std::string	get_cookie()
{
	static char	cookie[] = "000000000000";
	char		digit = cookie[11];
	int			pow = 0;

	while (digit == '9')
	{
		cookie[11 - pow] = '0';
		pow++;
		digit = cookie[11 - pow];
	}
	cookie[11 - pow]++;
	// std::cout << "COOKIES : je donne un nouveau cookie !\n";
	return (std::string(cookie));
}

std::string formCookies(std::vector<std::string> cookies)
{
	std::string cook = "";

	for (std::vector<std::string>::iterator i = cookies.begin(); i != cookies.end(); i++)
	{
		cook += "Set-Cookie: " + *i + "\n";
	}
	return (cook);
}

void removeLastChar(std::string &str)
{
	if (str[str.size() - 1] == ';')
		str = str.substr(0, str.size() - 1);
}
void timeoutHandler(int sign) {

	if (sign == SIGALRM)
		exit(E_INTERNAL_SERVER);
}