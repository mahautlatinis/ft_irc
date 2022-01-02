#include "Server.hpp"

Server::Server(int port, string const &password, IRC &irc) :
	_port(port),
	_password(password),
	_irc(irc),
	_fd(-1)
{}

Server::~Server()
{
	std::map<int, Client *>::iterator	clientIter;
	for (clientIter = _clients.begin(); clientIter != _clients.end(); ++clientIter)
		delete clientIter->second;
	close(_fd);
}

void	Server::SetUp()
{
	// Create server socket
	protoent	*prot(getprotobyname("tcp"));
#ifdef __APPLE__
	if ((_fd = socket(AF_INET, SOCK_STREAM, prot->p_proto)) == -1)
#else
	if ((_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, prot->p_proto)) == -1)
#endif
		SERVER_ERR("socket");

#ifdef __APPLE__
	fcntl(_fd, F_SETFL, O_NONBLOCK);
#endif

	// Set options for socket
	int	opt(1);
#ifdef __APPLE__
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
#else
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(int)) == -1)
#endif
		SERVER_ERR("setsockopt");

	// Bind socket to port
	sockaddr_in	sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(_port);
	if (bind(_fd, (sockaddr *)&sin, sizeof(sin)) == -1)
		SERVER_ERR("bind");

	// Listen for connections
	if (listen(_fd, MAX_LISTEN) == -1)
		SERVER_ERR("listen");

	std::cout	<< GREEN
				<< "IRC Server now active on "
				<< inet_ntoa(sin.sin_addr)
				<< ":" << _port
				<< NC << '\n';
}

void	Server::acceptClient()
{
	sockaddr_in	sin;
	socklen_t	sin_len = 0;

	int	clientFD(accept(_fd, (sockaddr *)&sin, &sin_len));
	if (clientFD == -1)
	{
		std::cerr << "Failed to accept a new connection\n";
		return;
	}
	std::cout << "New client on socket #" << clientFD << '\n';
	_clients.insert(std::make_pair(clientFD, new Client(clientFD)));
}

void	Server::removeClient(int fd)
{
	if (_clients.find(fd) != _clients.end())
	{
		delete _clients[fd];
		_clients.erase(fd);
	}
}

void	Server::Run()
{
	int	totalFD;
	std::vector<t_clientCmd>	responseQueue;
	std::set<int>				disconnectList;
	std::vector<t_clientCmd>::iterator	rIt;
	std::set<int>::iterator				dIt;

	while (true)
	{
		responseQueue.clear();
		disconnectList.clear();
		totalFD = setFDForReading();
		recvProcessCommand(totalFD, responseQueue, disconnectList);

		// Send server's response to clients
		for (rIt = responseQueue.begin(); rIt != responseQueue.end(); ++rIt)
		{
			int	clientFD = rIt->first;
			if (_clients.find(clientFD) != _clients.end())
				_clients[clientFD]->sendResponse(rIt->second);
		}
		// Disconnect FDs in list
		for (dIt = disconnectList.begin(); dIt != disconnectList.end(); ++dIt)
			removeClient(*dIt);
	}
}

int	Server::setFDForReading()
{
	_maxFD = _fd;
	FD_ZERO(&_fdReader);
	FD_SET(_fd, &_fdReader);

	std::map<int, Client *>::iterator	clientIter;
	for (clientIter = _clients.begin(); clientIter != _clients.end(); ++clientIter)
	{
		int	clientFD(clientIter->first);
		FD_SET(clientFD, &_fdReader);
		if (clientFD > _maxFD)
			_maxFD = clientFD;
	}

	int	r(select(_maxFD + 1, &_fdReader, NULL, NULL, NULL));
	if (r == -1)
		SERVER_ERR("select");
	return r;
}

void	Server::recvProcessCommand
	(int totalFD, std::vector<t_clientCmd> &responseQueue, std::set<int> &disconnectList)
{
	string	cmd;

	// Checking each socket for reading, starting from FD 3 because there should be nothing
	// to read from 0 (stdin), 1 (stdout) and 2 (stderr)
	for (int fd = 3; fd <= _maxFD && totalFD; ++fd)
		if (FD_ISSET(fd, &_fdReader))
		{
			if (fd == _fd)
				acceptClient();
			else if (disconnectList.find(fd) == disconnectList.end())
			{
				cmd.clear();
				// Receive a full command, with delimiter, then send it to program to process,
				// then grab program's response(s)
				if (!_clients[fd]->receiveCommand(cmd))
				{
					_irc.ClientDisconnect(fd);	// Tell the program that client is disconnected
					removeClient(fd);
				}
				else if (!cmd.empty() && _irc.ProcessClientCommand(std::make_pair(fd, cmd), responseQueue))
					disconnectList.insert(fd);
				// Victim is the one being killed by an operator
				int	victimFD = _irc.GetVictim();
				if (victimFD != -1)
					disconnectList.insert(victimFD);
			}
			--totalFD;
		}
}
