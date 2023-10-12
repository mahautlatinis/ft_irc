/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:03:01 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 09:46:02 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::userPrivmsg
	(User *user, 
		std::string const &name, 
		std::string const &msg,
		std::vector<t_clientCmd> &responseQueue)
{
	User		*target(getUserByNick(name));
	int			fd(user->_fd);
	std::string	resp;

	if (!target)
		resp = getResponseFromCode(user, 
			ERR_NOSUCHNICK, (std::string[]){ name });
	else if (target == _bot)
	{
		resp = _bot->processUserMsg(msg);
		Command	botResp(_bot, "PRIVMSG " + user->_nick + " :" + resp);
		privmsg(botResp, responseQueue);
		return ;
	}
	else
	{
		if (target->isAway())
		{
			resp = getResponseFromCode(user, 
				RPL_AWAY, (std::string[]){ name, target->_awayMsg });
			pushToQueue(user->_fd, resp, responseQueue);
		}
		resp = user->_prefix + " PRIVMSG " + name + " :" + msg + CMD_DELIM;
		fd = target->_fd;
	}
	pushToQueue(fd, resp, responseQueue);
	return ;
}

void	IRC::chanPrivmsg
	(User *user, 
		std::string const &name, 
		std::string const &msg,
		std::vector<t_clientCmd> &responseQueue)
{
	Channel	*chan(getChannelByName(name));

	if (!chan || !chan->hasJoined(user))
	{
		std::string	resp(getResponseFromCode(user, 
			ERR_CANNOTSENDTOCHAN, (std::string[]){ name }));
		pushToQueue(user->_fd, resp, responseQueue);
	}
	else
		appendUserNotif(
			user,
			(std::string[]){ "PRIVMSG", name, ":" + msg, "" },
			chan->_users, responseQueue, true
		);
	return ;
}

void	IRC::privmsg(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User		*user(cmd._user);
	std::string	resp;

	if (cmd._params.empty())
		resp = getResponseFromCode(user, 
			ERR_NORECIPIENT, (std::string[]){ cmd._type });
	else if (cmd._params.size() == 1)
		resp = getResponseFromCode(user, ERR_NOTEXTTOSEND, NULL);
	if (!resp.empty())
	{
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}

	std::string const	&name(cmd._params[0]);
	std::string const	&msg(cmd._params[1]);

	if (Channel::isPrefix(name[0]))
		chanPrivmsg(user, name, msg, responseQueue);
	else
		userPrivmsg(user, name, msg, responseQueue);
	return ;
}