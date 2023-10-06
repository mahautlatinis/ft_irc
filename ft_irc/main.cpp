/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:24:27 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:03:50 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"

IRC		*gIRC = NULL;
Server	*gServer = NULL;

static void	cleanup()
{
	if (gIRC)
		delete gIRC;
	if (gServer)
		delete gServer;
}

static void	handleSignal(int signum)
{
	if (signum == SIGINT || signum == SIGQUIT)
		std::cout << GREEN << "\b\bServer stopped. Good bye!" << std::endl << NC;
	exit(0);
}

static bool	checkArgs(int ac, char **av, int &port, string &password)
{
	int	iPort, iPassword;

	if (ac == 3)
		iPort = 1, iPassword = 2;
	else if (ac == 4)
	{
		std::cout << "Multi-server is not implemented, second argument is ignored" << std::endl;
		iPort = 2, iPassword = 3;
	}
	else
	{
		std::cerr << "\033[0;32mInvalid number of arguments\033[0m" << std::endl;
		return false;
	}

	port = std::atoi(av[iPort]);
	if (port <= 0 || port > 0xffff)
	{
		std::cerr << "Invalid port number" << std::endl;
		return false;
	}
	password = string(av[iPassword]);
	return true;
}

int	main(int ac, char **av)
{
	atexit(cleanup);

	signal(SIGINT, handleSignal);
	signal(SIGQUIT, handleSignal);

	int		port;
	string	password;
	if (!checkArgs(ac, av, port, password))
		exit(0);

	gIRC = new IRC(password);
	gServer = new Server(port, password, *gIRC);
	gServer->setUp();
	gServer->run();
	return (0);
}
