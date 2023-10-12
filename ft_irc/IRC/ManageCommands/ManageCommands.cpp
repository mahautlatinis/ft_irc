/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:19:48 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 11:00:16 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::unknownCmd(Command const &cmd,
	std::vector<t_clientCmd> &responseQueue) const
{
	std::string	resp(
		getResponseFromCode(cmd._user, ERR_UNKNOWNCOMMAND,
			(std::string[]) { cmd._type })
	);
	pushToQueue(cmd._user->_fd, resp, responseQueue);
	return ;
}

bool	IRC::passwordNotOK(User *user,
	std::vector<t_clientCmd> &responseQueue)
{
	std::string	resp(getErrorResponse(user, 
		"Access denied by configuration"));
	pushToQueue(user->_fd, resp, responseQueue);
	clientDisconnect(user->_fd);
	return true;
}

std::string	IRC::getNoticeMsg
	(std::string const &senderPrefix,
		User *user,
		std::string const &msg) const
{
	return std::string(
		senderPrefix + " NOTICE "
		+ user->_nick + " :" + msg
		+ CMD_DELIM
	); 
}

std::string	IRC::getErrorResponse(User *user,
	std::string const &msg) const
{
	return std::string(
		"ERROR :Closing link: ("
		+ user->_uname + "@" + USR_HOST + ") ["
		+ msg + "]"
		+ CMD_DELIM
	);
}

std::string	IRC::appendUserNotif
	(User *user, std::string		params[], std::set<User *> const &dest,
		std::vector<t_clientCmd>	&responseQueue, bool excludeUser) const
{
	std::string						msg(user->_prefix);
	std::set<User *>::iterator		it;

	for (int i = 0; !params[i].empty(); ++i)
		msg += " " + params[i];
	msg += CMD_DELIM;
	if (user->_prefix.empty())
		msg = msg.substr(1);
	
	for (it = dest.begin(); it != dest.end(); ++it)
		if (*it != user || !excludeUser)
			pushToQueue((*it)->_fd, msg, responseQueue);
	return msg;
}

void	IRC::pushToQueue(int fd, std::string const &msg,
	std::vector<t_clientCmd> &responseQueue) const
{
	std::stringstream	ss;

	if (fd == BOT_FD)
		return;
	
	ss << msg << CMD_DELIM;

	// For debug purposes
	// std::cout << ss.str() << std::endl;
	responseQueue.push_back(std::make_pair(fd, msg));
	return ;
}
