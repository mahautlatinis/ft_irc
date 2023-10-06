/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageCommands.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:19:48 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:56:36 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::unknownCmd(Command const &cmd,
	std::vector<t_clientCmd> &responseQueue) const
{
	string	resp(
		getResponseFromCode(cmd._user, ERR_UNKNOWNCOMMAND,
			(string[]) { cmd._type })
	);
	pushToQueue(cmd._user->_fd, resp, responseQueue);
	return ;
}

bool	IRC::passwordNotOK(User *user,
	std::vector<t_clientCmd> &responseQueue)
{
	string	resp(getErrorResponse(user, "Access denied by configuration"));
	pushToQueue(user->_fd, resp, responseQueue);
	ClientDisconnect(user->_fd);
	return true;
}

string	IRC::getNoticeMsg
	(string const &senderPrefix, User *user, string const &msg) const
{
	return string(
		senderPrefix + " NOTICE "
		+ user->_nick + " :" + msg
		+ CMD_DELIM
	); 
}

string	IRC::getErrorResponse(User *user, string const &msg) const
{
	return string(
		"ERROR :Closing link: ("
		+ user->_uname + "@" + USR_HOST + ") ["
		+ msg + "]"
		+ CMD_DELIM
	);
}

string	IRC::appendUserNotif
	(User *user, string params[], std::set<User *> const &dest,
		std::vector<t_clientCmd> &responseQueue, bool excludeUser) const
{
	string						msg(user->_prefix);
	std::set<User *>::iterator	it;

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

void	IRC::pushToQueue(int fd, string const &msg,
	std::vector<t_clientCmd> &responseQueue) const
{
	if (fd == BOT_FD)
		return;
	responseQueue.push_back(std::make_pair(fd, msg));
	return ;
}
