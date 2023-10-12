/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:02:45 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 10:10:29 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Headers.hpp"
#include "../IRC/IRC.hpp"
#include "Client.hpp"

#define MAX_LISTEN	42
#define SERVER_ERR(err)	do { std::cerr << RED << err << ": " << NC << strerror(errno) << std::endl; exit(1); } while (0)

class	Server
{
	private:
		fd_set					_fdReader;
		int						_fd;
		int						_maxFD;
		int const				_port;
		IRC						&_irc;
		std::map<int, Client *>	_clients;
		std::string	const		_password;

		void					acceptClient();
		void					removeClient(int fd);
		int						setFDForReading();
		void					recvProcessCommand(int totalFD, std::vector<t_clientCmd> &responseQueue, std::set<int> &disconnectList);

	public:
		// Canonical form can't be respected here because of the reference to IRC
		Server(int port, std::string const &password, IRC &irc);
		virtual ~Server(void);
		Server(Server const &src):
			_fdReader(src._fdReader),
			_fd(src._fd),
			_maxFD(src._maxFD),
			_port(src._port),
			_irc(src._irc),
			_clients(src._clients),
			_password(src._password)
		{
			*this = src;
			return ;
		};
		Server &operator=(Server const &rhs)
		{
			(void)rhs;
			return *this;
		}
		
		void					setUp(void);
		void					run(void);
};