/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:33:23 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:50:21 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"


string	IRC::kickTarget
	(User *user, string const &nick, Channel *chan, string const &comment,
	std::vector<t_clientCmd> &responseQueue)
{
	User	*target(getUserByNick(nick));
	string	resp;

	if (!target)
		resp = getResponseFromCode(user, ERR_NOSUCHNICK, (string[]){ nick });

	else if (!chan->hasJoined(target))
		resp = getResponseFromCode(user, ERR_USERNOTINCHANNEL, (string[]){ nick, chan->_name });

	if (!resp.empty())
	{
		pushToQueue(user->_fd, resp, responseQueue);
		return "";
	}

	userLeaveChannel(target, chan);

	resp = user->_prefix
		 + " KICK "
		 + chan->_name + " "
		 + nick + " :"
		 + comment + CMD_DELIM;

	pushToQueue(target->_fd, resp, responseQueue);
	return resp;
}

void	IRC::kick(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	string				resp;
	User				*user(cmd._user);
	string const		&chanName(cmd._params[0]);
	Channel				*chan(getChannelByName(chanName));
	std::vector<string>	nicks;
	string				msgToAll;

	if (cmd._params.size() < 2)
	{
		resp = getResponseFromCode(user, ERR_NEEDMOREPARAMS, (string[]){ cmd._type });
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}

	if (!chan)
		resp = getResponseFromCode(user, ERR_NOSUCHCHANNEL, (string[]){ chanName });
	else if (!chan->hasJoined(user))
		resp = getResponseFromCode(user, ERR_NOTONCHANNEL, (string[]){ chanName });
	else if (!chan->isOperator(user))
		resp = getResponseFromCode(user, ERR_CHANOPRIVSNEEDED, (string[]){ chanName });
	if (!resp.empty())
	{
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}
	
	::strSplit(nicks, cmd._params[1], ",");

	string	comment = (cmd._params.size() == 2)
					? user->_nick
					: cmd._params[2];

	for (std::vector<string>::iterator it(nicks.begin()); it != nicks.end(); ++it)
		msgToAll += kickTarget(user, *it, chan, comment, responseQueue);
	
	for (std::set<User *>::iterator it(chan->_users.begin());
		it != chan->_users.end(); ++it)
		pushToQueue((*it)->_fd, msgToAll, responseQueue);

	return ;
}
