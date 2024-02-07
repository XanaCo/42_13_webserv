/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilsParsing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atardif <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:19:59 by atardif           #+#    #+#             */
/*   Updated: 2024/02/07 11:23:31 by atardif          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

std::string intToStr(const int number) {

	std::string res;
	std::stringstream ss;

	ss << number;
	res = ss.str();

	return res;
}

char **mapToCharTab(std::map<std::string, std::string> mapToConvert) { 

	std::map<std::string, std::string>::iterator iter;
	std::vector<char *> vectorTab;

	for (iter = mapToConvert.begin(); iter != mapToConvert.end(); iter++) {

		std::string content = iter->first + "=" + iter->second;

		std::vector<char> vectorOfChars(content.begin(), content.end());
		vectorOfChars.push_back('\0');

		char *PtrVector = new char[vectorOfChars.size()];
		strcpy(PtrVector, vectorOfChars.data());

		vectorTab.push_back(PtrVector);
	}

	char **result = new char *[vectorTab.size() + 1];

	for (size_t it = 0; it != vectorTab.size(); it++)
		result[it] = vectorTab[it];

	result [vectorTab.size()] = NULL;

	return result;
}

void freeCharTab(char **charTab) { 

	for (size_t it = 0; charTab[it] != NULL; it++) 
		delete[] charTab[it];
	delete[] charTab;
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

bool checkPathExists(std::string pathToCheck) {

	struct stat filestat;
	
	if (pathToCheck.empty() || !pathToCheck.length())
		return false;
	if (stat(pathToCheck.c_str(), &filestat) == -1)
		return false;
	if (!(filestat.st_mode & S_IFDIR))
		return false;
	return true;
}

bool checkFileExists(std::string fileToCheck) {


	struct stat filestat;
	
	if (fileToCheck.empty() || !fileToCheck.length())
		return false;
	if (stat(fileToCheck.c_str(), &filestat) == -1)
		return false;
	if (!(filestat.st_mode & S_IFREG))
		return false;
	return true;
}

bool strToUInt(const std::string& str, unsigned int &number) {

	double result = 0;
	double sign = 1;

	for (size_t i = 0; i < str.size(); ++i)
	{
		if (i == 0 && (str[i] == '+' || str[i] == '-'))
			sign = (str[i] == '-') ? -1 : 1;
		else if (isdigit(str[i]))
			result = result * 10 + (str[i] - '0');
		else
			break;
	}
	if (result * sign > UINT_MAX || result * sign < 0)
		return false;

	number = (unsigned int)result * sign;
	return true;
}
