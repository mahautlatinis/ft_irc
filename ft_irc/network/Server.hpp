/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:02:45 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:03:45 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "../includes/Headers.hpp"
#include "../IRC/IRC.hpp"
#include "Client.hpp"

#define MAX_LISTEN	42
#define SERVER_ERR(err)	do { std::cerr << RED << err << ": " << NC << strerror(errno) << std::endl; exit(1); } while (0)

class	Server
{
	private:
		int const				_port;		// Server's port
		string	const			_password;	// Server's password
		IRC						&_irc;		// IRC program
		int						_fd;		// Server's socket fd

		std::map<int, Client *>	_clients;	// List of clients, fd as key, Client object as value
		int						_maxFD;		// Current highest client FD
		fd_set					_fdReader;	// Structure to select client FD for reading

		void					acceptClient();
		void					removeClient(int fd);
		int						setFDForReading();
		void					recvProcessCommand(int totalFD, std::vector<t_clientCmd> &responseQueue, std::set<int> &disconnectList);

	public:
		Server(int port, string const &password, IRC &irc);
		virtual ~Server();
		void					setUp();
		void					run();
};

#endif
