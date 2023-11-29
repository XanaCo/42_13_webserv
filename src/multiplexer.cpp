/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiplexer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pcheron <pcheron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 10:45:33 by pcheron           #+#    #+#             */
/*   Updated: 2023/11/28 12:38:22 by pcheron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/webserv.hpp"

// Ces infos de parametrage viendront du parsing
#define PORT "3490"  // port pour se connecter au server
#define BACKLOG 10   // nombre max de connexions

// ici on attend les process enfants termines
void sigchld_handler(int s)
{
    (void)s;
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0)
		;

    errno = saved_errno;
}

// retourne l'adresse ip, en IPv4 ou IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


// boucle principale pour accepter des connexions
void	getConnexionLoop(int listen_socket)
{
	int					new_connexion_socket;
	sockaddr_storage	client_addr;
	socklen_t			sin_size;

    std::cout << "server: waiting for connections..." << std::endl;
	while(1)
	{
        sin_size = sizeof(client_addr);

		// accepte une nouvelle connexion et cree un nouveau socket pour la communication
		new_connexion_socket = accept(listen_socket, (struct sockaddr *)&client_addr, &sin_size);
		if (new_connexion_socket < 0)
		{
			std::cerr << "webserv : accept error" << std::endl;
			continue;
		}

		// convertit l'adresse IP du client en format texte / fonction pas autorise a voir pour la changer
		inet_ntop(client_addr.ss_family, get_in_addr((struct sockaddr *)&client_addr), s, sizeof s);
		std::cout << "server: got connection from " << s << std::endl;

        // cree un processus enfant pour gerer la communication avec le client
        if (!fork())
		{
            close(listen_socket);
            if (send(new_connexion_socket, "Hello, world!", 13, 0) < 0)
				std::cerr << "send error" << std::endl;
            close(new_connexion_socket);
		    exit(0);
		}
		close(new_connexion_socket);
	}
}

// 						-nouvelles structures-
//
//	addrinfo : stocke les informations sur une/des adresse(s) reseau
//	elle est utilise dans le cadre de la resolution d'adresses reseau
//	-> (la resolution d'adresse reseau est le processus qui transforme 
//		un nom d'hote ("www.example.com") en adresse(s) IP.)
//	->	struct addrinfo
//		{
//		     int ai_flags;           // Indicateurs pour le traitement additionnel des options
//		     int ai_family;          // Famille d'adresses (AF_INET, AF_INET6, etc.)
//		     int ai_socktype;        // Type de socket (SOCK_STREAM, SOCK_DGRAM, etc.)
//		     int ai_protocol;        // Protocole associé à l'adresse
//		     size_t ai_addrlen;      // Taille de l'adresse en octets
//		     struct sockaddr *ai_addr; // Pointeur vers une structure sockaddr contenant l'adresse
//		     char *ai_canonname;     // Nom canonique du nœud
//		     struct addrinfo *ai_next; // Pointeur vers l'élément suivant dans la liste
//		 };
//
//	sockaddr_storage : stocke n'importe quel type d'adresse reseau
//	

void	multiplexer(void)
{
	struct sigaction	sa;
	int					yes=1;
	char				s[INET6_ADDRSTRLEN];


	addrinfo	hints;
	addrinfo	*serv_info;

	// initialisation de la structure hints pour getaddrinfo
	memset(&hints, 0, sizeof(hints)); // ft_bzero
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	// recupere des infos sur l'adresse
	if (getaddrinfo(NULL, PORT, &hints, &serv_info))
	{
		std::cerr << "webserv : getaddrinfo failed" << std::endl;
		return ;
	}

    // boucle pour trouver le meilleur resultat approprie et effectuer la liaison
	addrinfo	*p;
	int			listen_socket;
	for (p = serv_info;  p; p = p->ai_next)
	{
		// creation du socket
		listen_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listen_socket < 0)
		{
			std::cerr << "webserv : socket creation failed" << std::endl;
			continue;
		}

		// configuration du socket
		if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
		{
			std::cerr << "webserv : setsockopt failed" << std::endl;
			exit(1);
		}

		// liaison du socket a une adresse
		if (bind(listen_socket, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(listen_socket);
			std::cerr << "webserv : bind failed" << std::endl;
			continue;
        }

		// ici la liaison a reussi, on sort de la boucle
		break;
	}

    // on libere la structure serv_info qui n'est plus utile
    freeaddrinfo(serv_info);

	// si la liaison a echoue, on affiche un msg d'erreur et on quitte
    if (p == NULL) 
	{
		std::cerr << "webserv : connection failed" << std::endl;
        exit(1);
    }

    // Mise a l'ecoute du socket
    if (listen(listen_socket, BACKLOG) < 0)
	{
        std::cerr << "webserv : listen error" << std::endl;
        exit(1);
    }

    // configuration du gestionaire de signal
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		std::cerr << "webserv : srigaction error" << std::endl;
        exit(1);
    }
	getConnexionLoop(listen_socket);
}
