/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancolmen <ancolmen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 11:54:05 by ancolmen          #+#    #+#             */
/*   Updated: 2024/02/07 12:26:18 by ancolmen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef FILEPARSER_HPP
# define FILEPARSER_HPP

# include "webserv.hpp"

# define FILEPARSER "\033[1;33mFileParser\033[0m "

class ServerInfo;

class FileParser {

	public:
		FileParser(std::string const &filePath);
		~FileParser();

		std::string const 			&getFilePath() const;
		std::vector<std::string>	getRawFile() const;
		std::vector<std::string>	getRawServer() const;
		std::vector<ServerInfo>		getAllServers() const;
		int							getNServers() const;

		void						parseFile();
		std::string					checkFileValid();
		void						splitServers();
		size_t						serverEnd(size_t pos);
		void						stockServerInfo();
		ServerInfo					stockInfos(std::vector<std::string> serverTab);

		class FileParserError : public std::exception {
		
			public:
				FileParserError(std::string errorMsg) throw();
				~FileParserError() throw();

				virtual const char *what() const throw();
			
			private:
				std::string _errorMsg;
		};

		class SignalExit : public std::exception {
				
			public:
				SignalExit(std::string signalMsg) throw();
				~SignalExit() throw();

				virtual const char *what() const throw();
			
			private:
				std::string _signalMsg;
		};

	private:
		FileParser();
		FileParser(FileParser const &copy);
		FileParser	&operator=(FileParser const &other);

		std::vector<ServerInfo>		_allServers;
		std::string const			_filePath;
		std::vector<std::string>	_rawFile;
		std::vector<std::string>	_rawServer;
		int 						_nServers;

};

std::ostream &operator<<(std::ostream &out, FileParser const &other);

#endif
