
#include "../inc/Request.hpp"

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
