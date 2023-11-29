
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

