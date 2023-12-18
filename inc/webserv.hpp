
// ************************************************************************** //
//						A WEBSERV TO RULE THEM ALL
// ************************************************************************** //

#pragma once

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

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
# include <ctime>		// clock() -> a supprimer avant de push

// # include <stdio.h>
// # include <stdlib.h>

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
// # include <sys/event.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/signal.h>
// # include <signal.h>


// ************************************************************************** //
//  MACROS
// ************************************************************************** //

# define GET		1
# define POST		2
# define DELETE		4

// ************************************************************************** //
//  CLASSES
// ************************************************************************** //

# include "FileParser.hpp"
# include "ServerInfo.hpp"
# include "Location.hpp"
# include "Base.hpp"
# include "Client.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "CGI.hpp"


// ************************************************************************** //
//  COLORS & PRINT
// ************************************************************************** //

# define WHITE "\033[97;1m"
# define YELLOW "\033[33;1m"
# define BLUE "\033[34;1m"
# define MAGENTA "\033[95;1m"
# define RED "\033[31;1m"
# define GREEN "\033[32;1m"
# define END_COLOR "\033[0m"

# define PRINT false // changer a 1 pour voir les constructors/destructors des instances

// ************************************************************************** //
//  RETURN STATUS
// ************************************************************************** //

// Informations
# define I_CONTIUE				100 // continue to send your request
# define I_SWITCHING_PROTOCOLS	101 // quand on accepte de changer de protocole

// Succes
# define S_OK					200 // requete comprise et traite par le server
# define S_CREATED				201 // requete comprise traite et une ressource a ete cree en consequence
# define S_NO_CONTENT			204 // requete comprise traite mais pas de contenu a retourner dans la reponse
# define S_PARTIAL_CONTENT		206 // la ressource demandee ne peut pas etre fournie dans un format que le client accepte

// Redirections
# define R_MOVED_PERMANENTLY	301 // la ressource demandee a ete deplacee de maniere permanente vers une nouvelle URL
# define R_FOUND				302 // Humm, c'est interdit
# define R_NOT_MODIFIED			304 // la ressource demandee n'a pas ete modifiee depuis la derniere fois ou il la demande
# define R_TEMPORARY_REDIRECT	307 // Humm, c'est interdit

// Client errors
# define E_BAD_REQUEST			400 // la requete n'a pas pu etre comprise ou traitee
# define E_UNAUTHORIZED			401 // la requete n'a pas pu aboutir car le client n'a pas fourni les informations d'identification necessaires
# define E_FORBIDDEN			403 // la requete est correctement authentifiee mais le client n'a pas les droits pour acceder a la ressource
# define E_NOT_FOUND			404 // la ressource demandee n'a pas ete trouvee
# define E_METHOD_NOT_ALLOWED	405 // la methode de requete n'est pas autorisee pour la ressource demandee
# define E_CONFLICT				409 // il y a un conflit d'etat entre la requete du client et l'etat actuel de la ressource
# define E_GONE					410 // la ressource demandee etait presente autrefois mais n'existe plus
# define E_BODY_SIZE			413 // la taille du body de la requete depasse la taille maximale acceptee
# define E_TOO_MANY_REQUESTS	429 // le client a envoye trop de requetes dans un court lapse de temps

// Server errors
# define E_INTERNAL_SERVER		500 // une condition inattendue s'est produite du cote serveur
# define E_NOT_IMPLEMENTED		501 // le serveur ne prends pas en charge la fonctionnalite requise
# define E_BAD_GATEWAY			502 // renvoye par un serveur proxi pour indiquer qu'il a recu une reponse invalide d'un serveur en amont
# define E_SERVICE_UNAVAILABLE	503 // le server n'est pas en mesure de traiter la demande du client pour le moment
# define E_GATEWAY_TIMEOUT		504 // renvoye par un serveur proxi pour indiquer qu'il n'a pas recu de reponde de la part d'un serveur en amont

typedef enum e_headerRequest {
	RQ_METHOD,
	RQ_HOST,
	RQ_USER_AGENT,
	RQ_ACCEPT,
	RQ_ACCEPT_LANGUAGE,
	RQ_ACCEPT_ENCODING,
	RQ_AUTOR,
	RQ_CONTENT_TYPE,
	RQ_CONTENT_LENGHT,
	RQ_REFERER,
	RQ_COOKIES,
	RQ_CONNECTION,
	RQ_CACHECONTROL,
	RQ_BODY_START,
	RQ_OTHER_CONTENT
}	t_headerRequest;

typedef enum e_headerRq {
	RP_CONNECTION,
	RP_CACHECONTROL,
	RP_DATE,
	RP_CONTENT_TYPE,
	RP_CONTENT_LENGHT,
	RP_LOCATION,			// nouvelle URL de redirection
	RP_COOKIES,
	RP_SECURITY,
	RP_COMPRESSIOM
}	t_headerRq;

typedef enum e_scriptType {
	PY,
	PHP
}	t_scriptType;

// ************************************************************************** //
//  PROTOTYPES
// ************************************************************************** //

// Main program
void						multiplexer(void);

// General utils
void						signalHandler(int signal);
bool						containsParentDirectory(const std::string& str);
void						compressionOfSlashes(std::string& str);
bool						isAlphaDigit(char c);
char						hexToChar(const std::string& hex);
void						remplacerPercentEncoding(std::string& chaine);

// Print and Debug utils
void						printStringVector(std::vector<std::string> stringVector);
void						printServersInfo(std::vector<ServerInfo> stringVector);
std::string					returnStringVector(std::vector<std::string> stringVector);
void						printServerLocations(ServerInfo Server);
void 						printCharTab(char **charTab);

// Parsing utils
int							strToInt(const std::string& str);
std::vector<std::string>	splitString(const std::string& input, char delimiter);
void						eraseComments(std::string &content);
std::vector<std::string>	cSplitLine(std::string line, char const *charset);
bool 						semiColonEnding(std::string &content);
std::pair<int, std::string>	createPairErrorPage(std::string num, std::string word);
std::string 				getNameDir(const std::string& path);
std::string					getNameFile(const std::string& path);
bool 						checkPathExists(std::string pathToCheck);
bool 						checkFileExists(std::string fileToCheck);
char 						**mapToCharTab(std::map<std::string, std::string> mapToConvert);
void 						freeCharTab(char **charTab);
void						deEncodingHexa(std::string& chaine);

#endif
