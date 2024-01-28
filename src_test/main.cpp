# include <string>
# include <iostream>
# include <fstream>
# include <sstream>
# include <list>
# include <map>
# include <vector>
# include <cstdlib>
# include <csignal>
# include <ctime>
# include <exception>
# include <cmath>
# include <cerrno>
# include <cctype>
# include <algorithm>
# include <limits>
# include <utility>
# include <ctime>

# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <sys/types.h>
# include <dirent.h>
# include <poll.h>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/signal.h>

std::string getNameFile(const std::string& path)
{
    size_t lastSep = path.find_last_of('/');

    // Si aucun séparateur n'est trouvé, le chemin est probablement juste le nom de fichier
    if (lastSep == std::string::npos)
        return path;
    return (path.substr(lastSep + 1));
}

#include <stdio.h>
int	main()
{
	std::string s1 = "/";
	std::string s2 = "/test";
	printf("<%s>\n", getNameFile(s1).c_str());
	printf("<%s>\n", getNameFile(s2).c_str());
	return (0);
}