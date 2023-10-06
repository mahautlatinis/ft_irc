/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:58:30 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 22:02:50 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int port, string const &password, IRC &irc) :
	_port(port),
	_password(password),
	_irc(irc),
	_fd(-1)
{
	return ;
}

Server::~Server(void)
{
	std::map<int, Client *>::iterator	clientIter;
	for (clientIter = _clients.begin(); clientIter != _clients.end(); ++clientIter)
		delete clientIter->second;
	close(_fd);
	return ;
}

void	Server::setUp(void)
{
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

	int	opt(1);
#ifdef __APPLE__
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) == -1)
#else
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(int)) == -1)
#endif
		SERVER_ERR("setsockopt");

	sockaddr_in	sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(_port);
	if (bind(_fd, (sockaddr *)&sin, sizeof(sin)) == -1)
		SERVER_ERR("bind");

	if (listen(_fd, MAX_LISTEN) == -1)
		SERVER_ERR("listen");

	std::cout	<< GREEN
				<< "IRC Server now active on "
				<< inet_ntoa(sin.sin_addr)
				<< ":" << _port
				<< NC << std::endl;
	return ;
}

void	Server::acceptClient(void)
{
	sockaddr_in	sin;
	socklen_t	sin_len = 0;

	int	clientFD(accept(_fd, (sockaddr *)&sin, &sin_len));
	if (clientFD == -1)
	{
		std::cerr << "Failed to accept a new connection" << std::endl;
		return;
	}
	std::cout << "New client on socket #" << clientFD << std::endl;
	_clients.insert(std::make_pair(clientFD, new Client(clientFD)));
}

void	Server::removeClient(int fd)
{
	if (_clients.find(fd) != _clients.end())
	{
		delete _clients[fd];
		_clients.erase(fd);
	}
	return ;
}

void	Server::run(void)
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

		for (rIt = responseQueue.begin(); rIt != responseQueue.end(); ++rIt)
		{
			int	clientFD = rIt->first;
			if (_clients.find(clientFD) != _clients.end())
				_clients[clientFD]->sendResponse(rIt->second);
		}
		for (dIt = disconnectList.begin(); dIt != disconnectList.end(); ++dIt)
			removeClient(*dIt);
	}
	return ;
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

	for (int fd = 3; fd <= _maxFD && totalFD; ++fd)
		if (FD_ISSET(fd, &_fdReader))
		{
			if (fd == _fd)
				acceptClient();
			else if (disconnectList.find(fd) == disconnectList.end())
			{
				cmd.clear();
				if (!_clients[fd]->receiveCommand(cmd))
				{
					_irc.clientDisconnect(fd);
					removeClient(fd);
				}
				else if (!cmd.empty() && _irc.processClientCommand(std::make_pair(fd, cmd), responseQueue))
					disconnectList.insert(fd);
				int	victimFD = _irc.getVictim();
				if (victimFD != -1)
					disconnectList.insert(victimFD);
			}
			--totalFD;
		}
	return ;
}
