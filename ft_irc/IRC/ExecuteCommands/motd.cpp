/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MOTD.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:54:56 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:51:33 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"
#include <fstream>

static std::vector<string>	MOTDLines;
static bool					motdInit(false);

static void	initMotd()
{
	std::ifstream	motdFile(MOTD_FILE);
	char			*cwd(getcwd(NULL, 0));
	string			line;

	motdInit = true;
	if (!motdFile.is_open())
	{
		std::cout	<< RED << "Cannot read message of the day from "
					<< cwd << '/' << MOTD_FILE << std::endl << NC;

		free(cwd);
		return ;
	}
	while (std::getline(motdFile, line))
		MOTDLines.push_back(line);
	motdFile.close();
	return ;
}

void	IRC::motd(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	string	motd;
	User	*user(cmd._user);

	if (!motdInit)
		initMotd();
	if (MOTDLines.empty())
		motd = getResponseFromCode(user, ERR_NOMOTD, NULL);
	else
	{
		motd = getResponseFromCode(user, RPL_MOTDSTART, NULL);
		for (std::vector<string>::iterator it(MOTDLines.begin());
			it != MOTDLines.end(); ++it)
		{
			motd += getResponseFromCode(
				user, RPL_MOTD,
				(string[]){ *it }
			);
		}
		motd += getResponseFromCode(user, RPL_ENDOFMOTD, NULL);
	}
	pushToQueue(user->_fd, motd, responseQueue);
	return ;
}
