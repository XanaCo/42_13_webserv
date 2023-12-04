# WEBSERV 

## RESSOURCES

- [ HTTP DEV ](https://http.dev/)
- [ Asyncronous I/O ](https://youtu.be/tAGF0T2cXRA)
- [ Jacob Sorber - How to build a Web Client ](https://youtu.be/bdIiTxtMaKA)
- [ DataTracker ](https://datatracker.ietf.org/doc/html/rfc2616)
- [ Bej's Guide to Network Programming ](https://beej.us/guide/bgnet/html/split/)
- [ Medium Guide ](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
- [ Authorised Functions ](https://free-mimosa-2dd.notion.site/Fonctions-autorisees-41b6c35f8c854d75992ee6bc7edf3001?pvs=4)
- [ Maxence's Webserv ](https://github.com/mbocquel/42_cursus/tree/main/52_webserv)
- [ Webserv Guide ](https://github.com/Kaydooo/Webserv_42)
- [ Webserv Ressources ](https://github.com/cclaude42/webserv)

## GIT 

#### Verifier que notre branche est a jour

- ```git checkout / switch``` vstable

- ```git pull``` origin vstable

- ```git checkout / switch``` **perso-branchezoneavenue**

#### Commit les nouveaux changements

- ```git add``` *

- ```git commit -m``` ***J'ai fini le projet**

#### Push notre branche sur le repo

- ```git push``` origin **perso-branchezoneavenue**

#### Creer une Pull Request

- Aller sur la main page du repo sur github
- Cliquer sur **Create pull request** ou **Compare and pull request**

#### Comparer les Changements

- S'assurer que ce soit bien la vstable qui soit set en tant que branche dans laquelle on veut merge.
- S'assurer que ce soit notre branche *perso* qui soit set dans la branche a comparer

#### Creer la Pull Request

- Verifier les changements qui seront merge
- Donner un titre aux changements et donner une description des changements
- Cliquer sur **create pull request**

#### Etape des reviews

On peut soit specifier les membres de la team qui devront review ou bien mettre tout le monde. Ces membres recevront une notif et peuvent commenter ou approuver les changements.

#### Corriger les feedback

- L'auteur de la pull request doit corriger les feedback faits, si il en est.
- Il doit a nouveau commit et push ces changements (sa branche)

#### Merge

Une fois que la pull request est approuvee (par tous les membres, ou selon un poucentage fix par l'auteur du repo). On peut faire le merge dans la main branch

## Project 

### Prerequis

- [x] Arguments : Config file or Config file path
- [x] Pas exécuter un autre serveur web
- [ ] Le serveur ne doit jamais bloquer et le client doit être correctement renvoyé si nécessaire
- [ ] Il doit être non bloquant et n’utiliser qu’un seul ```poll()``` (ou équivalent) pour toutes les opérations entrées/sorties entre le client et le serveur (listen inclus)
- [ ] ```poll()``` (ou équivalent) doit vérifier la lecture et l’écriture en même temps
- [ ] Vous ne devriez jamais faire une opération de lecture ou une opération d’écriture sans passer par ```poll()``` (ou équivalent)
- [ ] La vérification de la valeur de errno est strictement interdite après une opération de lecture ou d’écriture
- [ ] Vous n’avez pas besoin d’utiliser ```poll()``` (ou équivalent) avant de lire notre fichier de configuration
- [ ] PAS AUTHORISE D'UTILISER ```read/recv``` ou ```write/send``` avec n’importe quel FD sans utiliser ```poll()``` (ou équivalent)
- [ ] Authorise d'utiliser chaque macro et définir comme ```FD_SET, FD_CLR, FD_ISSET, FD_ZERO```
- [ ] Une requête à notre serveur ne devrait jamais se bloquer pour indéfiniment
- [ ] Compatibilite avec le navigateur web de notre choix
- [ ] ```NGINX``` est conforme à ```HTTP 1.1``` et peut être utilisé pour comparer les en-têtes et les comportements de réponse
- [ ] Codes d’état de réponse **HTTP** exacts
- [ ] Pages d’erreur par défaut
- [ ] Pas de fork pour autre chose que **CGI**
- [ ] Site web entierement statique
- [ ] On peut telecharger des fichiers
- [ ] Metodes ```GET, POST, DELETE```
- [ ] Stress Tests
- [ ] Le serveur doit ecouter sur plusieurs ports
- [ ] Sur MACOS, on peut utiliser ```fcntl()``` seulement : ```fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC)```. Toute autre flag est interdit

### Mandatory

Read **RFC** and do some test with telnet and **NGINX**

- [ ] Choisir le port et l’host de chaque *serveur*
- [ ] Setup ```server_names``` ou pas
- [ ] Premier serveur pour un ```host:port``` sera le serveur par défaut pour cet ```host:port```
- [ ] Setup des pages d’erreur par défaut
- [ ] Limiter la taille du body des clients
- Setup des routes avec une ou plusieurs des règles/configurations suivantes (pas de regexp): 
	- [ ] Définir une liste de méthodes **HTTP** acceptées pour la route
	- [ ] Définir une redirection **HTTP**
	- [ ] Définir un répertoire ou un fichier à partir duquel le fichier doit être recherché (par exemple si l’url /kapouet est rootée sur /tmp/www, l’url /kapouet/pouic/toto/pouet est /tmp/www/pouic/toto/pouet).
	- [ ] Activer ou désactiver le listing des répertoires
	- [ ] Set un fichier par défaut comme réponse si la requête est un répertoire
	- [ ] Exécuter **CGI** en fonction de certaines extensions de fichier
	- [ ] Faites-le fonctionner avec les méthodes ```POST``` et ```GET```
	- [ ] Rendre la route capable d’accepter les fichiers téléchargés et configurer où cela doit être enregistré.
	- [ ] Le **CGI** utilise directement le chemin complet comme ```PATH_INFO```
	- [ ] Pour les requêtes fragmentées, notre serveur doit la dé-fragmenter et le **CGI** attendra ```EOF``` comme fin du body
	- [ ] Si aucun ```content_length``` n’est renvoyé par le **CGI**, ```EOF``` signifiera la fin des données renvoyées
	- [ ] Webserv doit appeler le **CGI** avec le fichier demandé comme premier argument
	- [ ] Le **CGI** doit être exécuté dans le bon répertoire pour l’accès au fichier de chemin relatif
	- [ ] Notre serveur devrait fonctionner avec un seul **CGI** (php-CGI, Python, etc.)
	- [ ] Fournir les fichiers de config et des fichiers de base  par défaut pour tester et démontrer que chaque fonctionnalité fonctionne
	- [ ] Comparer le comportement de notre programme avec celui de NGINX (verifier le fonctionement de ```server_name```)
	- [ ] Le serveur ne mort jamais

### Bonus

- [ ] Support cookies et gestion de session
- [ ] Gèrer plusieurs **CGI**


## TO-DO

- [ ] Everything

