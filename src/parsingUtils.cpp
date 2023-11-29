
#include "../inc/webserv.hpp"


int strToInt(const std::string& str)
{
    int result = 0;
    int sign = 1;

    for (size_t i = 0; i < str.size(); ++i)
    {
        if (i == 0 && (str[i] == '+' || str[i] == '-'))
            sign = (str[i] == '-') ? -1 : 1;
        else if (isdigit(str[i]))
            result = result * 10 + (str[i] - '0');
        else
            break;
    }

    return (result * sign);
}

std::vector<std::string> splitString(const std::string& input, char delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = input.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end + 1;
        end = input.find(delimiter, start);
    }

    tokens.push_back(input.substr(start));
    return tokens;
}
