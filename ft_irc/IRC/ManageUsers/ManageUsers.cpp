/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManUser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:50:09 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 18:50:17 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

User	*IRC::getUserByNick(string const &nick) const
{
	for (std::map<int, User *>::const_iterator it(_users.begin());
		it != _users.end(); ++it)
		if (it->second->_nick == nick)
			return it->second;
	return NULL;
}

void	IRC::userLeaveChannel(User *user, Channel *chan)
{
	user->_joined.erase(chan);
	if (chan->removeUser(user) == 0)
	{
		_channels.erase(chan->_name);
		delete chan;
	}
	return ;
}

void	IRC::removeFromAllChannel(User *user)
{
	std::map<string, Channel *>	chansCopy(_channels);
	Channel						*chan;

	for (std::map<string, Channel *>::iterator it(chansCopy.begin());
		it != chansCopy.end(); ++it)
	{
		chan = it->second;
		if (chan->hasJoined(user))
			userLeaveChannel(user, chan);
		else if (chan->isInvited(user))
			chan->_invitations.erase(user);
	}
	return ;
}

void	IRC::sendWelcomeMessage(User *user, std::vector<t_clientCmd> &responseQueue)
{
	string	resp(
		  getResponseFromCode(user, RPL_WELCOME, (string[]){ user->_prefix })
		+ getResponseFromCode(user, RPL_YOURHOST, NULL)
		+ getResponseFromCode(user, RPL_CREATED, NULL)
		+ getResponseFromCode(user, RPL_MYINFO, NULL)
	);
	pushToQueue(user->_fd, resp, responseQueue);
	Command	cmdMOTD(user, "MOTD");
	motd(cmdMOTD, responseQueue);
	return ;
}
