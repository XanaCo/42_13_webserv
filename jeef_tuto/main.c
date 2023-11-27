
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

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

int	main()
{
    int                     sockfd; // listen on sockfd
    int                     new_fd; // new connection on new_fd
    struct addrinfo         hints;
    struct addrinfo         *servinfo;
    struct addrinfo         *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t               sin_size;
    struct sigaction        sa;
    int                     yes=1;
    char                    s[INET6_ADDRSTRLEN];
    int                     rv;

    // initialisation de la structure hints pour getaddrinfo
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    // recupere des infos sur l'adresse
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
	{
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // boucle pour trouver le meilleur resultat approprie et effectuer la liaison
    for(p = servinfo; p != NULL; p = p->ai_next)
	{
        // creation du socket
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1)
		{
            perror("server: socket");
            continue;
        }

        // configuration du socket
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1)
		{
            perror("setsockopt");
            exit(1);
        }

        // liaison du socket a une adresse
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
            close(sockfd);
            perror("server: bind");
            continue;
        }

        // ici la liaison a reussi, on sort de la boucle
        break;
    }

    // on libere la structure servinfo qui n'est plus utile
    freeaddrinfo(servinfo);

    // si la liaison a echoue, on affiche un msg d'erreur et on quitte
    if (p == NULL) 
	{
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    // Mise a l'ecoute du socket
    if (listen(sockfd, BACKLOG) == -1)
	{
        perror("listen");
        exit(1);
    }

    // configuration du gestionaire de signal
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

	// boucle principale pour accepter des connexions
    while(1)
	{
        sin_size = sizeof their_addr;

        // accepte une nouvelle connexion et cree un nouveau socket pour la communication
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
		{
            perror("accept");
            continue;
        }

        // convertit l'adresse IP du client en format texte
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

        // cree un processus enfant pour gerer la communication avec le client
        if (!fork())
		{
            close(sockfd);
            if (send(new_fd, "Hello, world!", 13, 0) == -1)
                perror("send");
            close(new_fd);
            exit(0);
        }
        close(new_fd);
    }
	return (0);
}