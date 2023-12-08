
#include "../inc/webserv.hpp"

int strToInt(const std::string& str) {

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

std::vector<std::string> splitString(const std::string& input, char delimiter) {

	std::vector<std::string> tokens;
	size_t start = 0;
	size_t end = input.find(delimiter);

	while (end != std::string::npos)
	{
		tokens.push_back(input.substr(start, end - start));
		start = end + 1;
		end = input.find(delimiter, start);
	}
	tokens.push_back(input.substr(start));
	
	return tokens;
}

bool semiColonEnding(std::string &content) {

	if (*(content.rbegin()) == ';')
	{
		content.erase(content.end() - 1);
		return true;
	}
	return false;

}

std::pair<int, std::string> createPairErrorPage(std::string num, std::string word) {

	int n = strToInt(num);

	if (n < 1 || n > 599)
		throw ServerInfo::ServerInfoError("Invalid return error code in location");
	if (word.empty() || word.find(".html", word.size() - 5) == std::string::npos)
		throw ServerInfo::ServerInfoError("Invalid return path in location");

	return std::pair<int, std::string>(n, word);
}
