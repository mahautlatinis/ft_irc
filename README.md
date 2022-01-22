# ft_irc

- Status: validated
- Grade: 125%

## Suggestion for you
- Use your proxy.py program with your server (or a real server) to check how messages look like.
- Download Hexchat on your 42 VM or find another UX friendly client for your environment.
- Check DCC send and DCC get for the FTP bonus.
- Split the work into tree part : 1. The socket programming and network part, 2. Client registration/connection and associated commands + Sending messages and notices, 3. Channel operations commands.  

→ **Socket programming**

Pour simplifier la correction (connexion avec netcat)

```bash
nc -C 127.0.0.1 3333 
CAP LS 302
PASS pass
NICK nickname
USER nickname username 127.0.0.1 :realname
```

Page principale (voir celle de Baudoin), de jolis schemas avaient ete fait sur notre “command process”: 

→ [https://www.notion.so/IRC-8de3850fa05745ceaeb089e2faef3b00](https://www.notion.so/IRC-8de3850fa05745ceaeb089e2faef3b00)

Meilleur groupe 42:

→ [https://ircgod.com/](https://ircgod.com/)

**Vidéo tutoriel** 

[https://www.youtube.com/watch?v=3QiPPX-KeSc](https://www.youtube.com/watch?v=3QiPPX-KeSc)

Cette vidéo explique les mêmes concepts (mais en python).

# Rappels

## Internet protocol (look RFC for more info)

IP (internet protocol) → network protocol form a stack to handle (**Network layer**, transport layer...)

Headers are like envelop → each protocol adds one to attach its own metadata

A message (datagram) is sent by a sender app. The transport layer (TCP/UDP) receive this datagram and attach an header (envelop) to insert the information that it needs + maps/attachs the port numbers (each application has its own port(s) number(s)).

---

le Internet protocole qui a ajoute ses headers (IP address) en dernier sur le datagram qui se retrouve au dessus des headers de la transport layer (port number) et le datagram en dernier.

(voir la video de Rhymu’s Video) qui est super bien expliquee). 

---

IP sender splits the large datagram into smaller fragments to fit the network, then transmis the fragments as a smaller datagram which the routers pass along to the receiver.

Then the receiver reassembles to reconstruct the original datagram

## TCP - makes sure every packet is received

TCP is **“bidirectionnal”**. 

The server are passing open and the clients are actively tring to make a connection. 

Plusieurs signaux sont envoyes : syn and fin signals. 

Syn signal (synchronization byte - server has got to sent back its own signal if it is accepting its connection). 

## Sockets

[https://github.com/rhymu8354/SocketTutorial](https://github.com/rhymu8354/SocketTutorial)

Enabling you to create an endpoint and it could be using different protocols. 

Functions normally used with **connection-oriented sockets** (TCP)

→ connect (client) vs listen then accept (server) 

→ accept (as a server you will have as much sockets as connected clients).

→send (client) vs receive (server)

// → shutdown 

close → give up the connextion entirely

## Input/Outpout

Blocking and non-blocking modes :

If it is not blocking it enables you to not wait for the function call to end

fnctl → file control function (POSIX), special mode flag O_NONBLOCK

# Client / Server model

Pour comprendre le socket programming il faut avoir une compréhension du network et pour comprendre et debugger un projet de socket programming il faut pouvoir comprendre ce qu’il se passe dans son réseau local.

Les sockets se basent sur le “client/server” model.

Le serveur est le process principal (central location) (dans notre cas il n’y en a qu’un). Ensuite, on peut retrouver différents clients qui vont être (potentiellement) éparpillés.

Les jeux sont un bon exemple de socket programming (d’ailleurs Ubisfot propose un Graduate Program de “Online Developer” et attend comme compétences le C++ et des connaissances reseau + server).

Dans les jeux, le “client” est l’instance de notre jeu (exemple : fortnite).

Les clients ne peuvent pas se connecter entre eux, ils se connectent uniquement **au(x) serveur(s)**. Une des raisons pour cela est notamment que cela pourrait poser des problèmes de sécurité. De plus, le fait d’avoir une “central location” optimise la rapidité (du jeu - si chaque client devait se connecter a tous les autres clients ce serait très contraignant).

Un client envoie donc une information/message au serveur et le serveur va la retransmettre a **tous les clients**.

## Local network

modem - wired in to your internet connection.

Router - allows devices in your home to connect (wirelessly)

Notre modem possède une adresse IP publique. Mais chaque device possède aussi une adresse IP privée (nécessaire pour que le router puisse communiquer avec chaque device).

Dans ce type de projets il arrive souvent de travailler avec plusieurs threads (multithreading), mais ce n’était pas demandé dans le cadre du sujet ft_irc.

Les threads peuvent être utiles pour qu’un client n’ait pas à attendre en raison d’un autre client qui “impacterait” le serveur.

Un port qui est souvent inutilisé est le port 5050 (tcp / trojans / used to be used by Yahoo). 

**ipconfig** → donne des local network informations. Ces données ne doivent pas être partagées.

Notre IP privée va être utilisée pour pouvoir faire tourner notre programme sur le réseau local.

Il ne faut pas hardcoder l’adresse IP puisque sinon le projet risque de ne pas marcher sur une autre machine.

On peut creer différent types de socket en fonction du protocole avec lequel on va travailler (type d’adresse).

Ensuite on va “binder” (lier) la socket sur l’adresse et le port.

Ensuite la socket va devoir être configurée pour être en écoute puis accepter des connexions.

Ensuite, on pourrait creer un thread a chaque connexion. (ou faire en sorte que le serveur n’attende pas de message et ne bloque pas tant que ce n’est pas le cas).

Ensuite, tant que le client est connecte, on va chercher à recevoir les messages qu’il nous envoie. Attention, les messages envoyés peuvent ne pas contenir le même nombre de bytes.

Comme on ne sait pas la taille que va faire notre message, on va potentiellement devoir envoyer dans un premier temps au serveur comme message préliminaire / header la taille que va faire notre message. Le message va potentiellement etre encodé puis “décodé” (from byte format to string).

Il faudrait deconnecter le client s’il envoie un message de deconnexion et closer la socket si tel est le cas.

### Questions and answers

- What is the difference between nickname / username / realname ?
    
    The **nickname** supplied with `NICK` is the name that's used to address you on IRC. The nickname must be unique across the network, so you can't use a nickname that's already in use at the time.
    
    The **username** supplied with `USER` is simply the `user` part in your `user@host` hostmask that appears to others on IRC, showing where your connection originates from (if you've never seen these, then your client may be hiding them). In the early days of IRC it was typical for people to run their IRC client on multi-user machines, and the username corresponded to their local username on that machine. Some people do still use IRC from multi-user shell servers, but for the most part the username is vestigal.
    
    The **real name** supplied with `USER` is used to populate the real name field that appears when someone uses the `WHOIS` command on your nick. Unlike the other two fields, this field can be fairly long and contain most characters (including spaces). Some people do put their real name here, but many do not.
    
- What is the difference between IPPROTO_TCP & 0 in socket() ?
    
    [https://stackoverflow.com/questions/7122102/sockets-ippproto-tcp-vs-0](https://stackoverflow.com/questions/7122102/sockets-ippproto-tcp-vs-0)
    
- Does the serv concatenate replies or send them individuallly ?
    

```bash
#Rejoindre un serveur qui existe deja pour test
nc irc.devgungeon.com 6667
```

# Notes

"**Internet Relay Chat**"→ collection of networks (collection of channels).

Hexchat seems to be the most popular chat on Linux (freenode). 

→ Freenode est utilise par des utilisateurs de distribution Linux qui posent des questions support

Irssi (sur terminal, assez sympa aussi). 

Will a confirme qu'il fallait choisir un client et adapter notre code pour qu'il marche avec celui-ci

Important → lecture de la RFC2812.

"text based conferencing".

**Documentation :**

- [https://datatracker.ietf.org/doc/html/](https://datatracker.ietf.org/doc/html/rfc1459)rfc2810
- [http://chi.cs.uchicago.edu/chirc/irc.html](http://chi.cs.uchicago.edu/chirc/irc.html)
- best : [https://beej.us/guide/bgnet/html/](https://beej.us/guide/bgnet/html/)
- exemple : [https://github.com/AdrienCN/ft_irc](https://github.com/AdrienCN/ft_irc)

### What is a socket

→ Les sockets sont un moyen de "parler a d'autres programmes en utilisant les "standard Unix file descriptors".

→ Pour rappel, les file descriptors sont des entiers associes a des fichiers ouverts. 

→ On dit souvent qu'en Unix, "tout est un fichier". Quand les programmes font des I/O, ils le font en lisant ou ecrivant sur un file descriptor. 

→ Or, ce fichier peut etre " a network connection, a FIFO, a pipe, a terminal etc....

C'est l'appel socket() (system routine) qui permet de recuperer un file descriptor. Ensuite, on va communiquer en utilisant send() et recv() (socket calls). 

Il serait techniquement possible d'utiliser read et write mais send et recv permettent beaucoup plus de controle sur "la data transmission". 

### Raw sockets

Stream socket (TCP)

Datagram socket (UDP) → use IP for routing 

→ connection less → we do not have to maintain an open connection as you do with stream sockets.

We have to build a packet, slap an IP header on int with destination information, and send it out. 

SOCK_DGRAM applications

### Low level and network theory

### Data encapsulation :

Ethernet → IP → UDP → TFTP → Data

Packet is wrapped (encapsulated) in header (and rarely a footer) by the first protocol (TFTP protocol) then the whole thing (TDFTP header included_ is encapsulated again by the next protocol (say, UDP), them agaim by the next (IP) then again by the final protocol on the hardware (physical layer) (say, Ethernet). 

Layered network model (ISO). 

- Application Layer (*telnet, ftp, etc.*)
- Host-to-Host Transport Layer (*TCP, UDP*)
- Internet Layer (*IP and routing*)
- Network Access Layer (*Ethernet, wi-fi, or whatever*)

```c
struct addrinfo {
        int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
        int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
        int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
        int              ai_protocol;  // use 0 for "any"
        size_t           ai_addrlen;   // size of ai_addr in bytes
        struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
        char            *ai_canonname; // full canonical hostname
    
        struct addrinfo *ai_next;      // linked list, next node
    };
```

Modes par defaut conventionnels 

- +C → Blocks users from sending CTCP request to channel
- +n → No external messages ? Ceux qui n ont pas join le channel ne peuvent pas envoyer de message
- +s → Make the channel secret (ne pas gerer)
- +t → Only ops and half ops can change the topic (a faire celui-ci)

