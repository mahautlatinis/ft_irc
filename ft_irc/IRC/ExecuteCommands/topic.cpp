/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:08:52 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 19:19:11 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::topic(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User	*user(cmd._user);
	string	resp;

	if (cmd._params.empty())
	{
		resp = getResponseFromCode(user, ERR_NEEDMOREPARAMS, (string[]){ cmd._type });
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}

	string const	&chanName(cmd._params[0]);
	Channel			*chan(getChannelByName(chanName));

	if (!chan)
		resp = getResponseFromCode(user, ERR_NOSUCHCHANNEL, (string[]){ chanName });
	else if (cmd._params.size() == 1)
		resp = (chan->_topic.empty())
			 ? getResponseFromCode(user, RPL_NOTOPIC, (string[]){ chanName })
			 : getResponseFromCode(user, RPL_TOPIC, (string[]){ chanName, chan->_topic });
	else if (!chan->hasJoined(user))
		resp = getResponseFromCode(user, ERR_NOTONCHANNEL, (string[]){ chanName });
	else if (!chan->isOperator(user) && !chan->_anyoneCanSetTopic)
		resp = getResponseFromCode(user, ERR_CHANOPRIVSNEEDED, (string[]){ chanName });
	else
	{
		chan->_topic = cmd._params[1];
		resp = appendUserNotif(
			user,
			(string[]){ "TOPIC", chanName, ":" + chan->_topic, "" },
			chan->_users, responseQueue, true
		);
	}
	pushToQueue(user->_fd, resp, responseQueue);
	return ;
}
