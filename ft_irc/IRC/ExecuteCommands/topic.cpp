/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:08:52 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 09:47:38 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::topic(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User		*user(cmd._user);
	std::string	resp;

	if (cmd._params.empty())
	{
		resp = getResponseFromCode(user,
			ERR_NEEDMOREPARAMS, (std::string[]){ cmd._type });
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}

	std::string const	&chanName(cmd._params[0]);
	Channel			*chan(getChannelByName(chanName));

	if (!chan)
		resp = getResponseFromCode(user, 
				ERR_NOSUCHCHANNEL, (std::string[]){ chanName });
	else if (cmd._params.size() == 1)
		resp = (chan->_topic.empty())
			 ? getResponseFromCode(user, 
			 	RPL_NOTOPIC, (std::string[]){ chanName })
			 : getResponseFromCode(user, 
			 	RPL_TOPIC, (std::string[]){ chanName, chan->_topic });
	else if (!chan->hasJoined(user))
		resp = getResponseFromCode(user, 
				ERR_NOTONCHANNEL, (std::string[]){ chanName });
	else if (!chan->isOperator(user) && !chan->_anyoneCanSetTopic)
		resp = getResponseFromCode(user,
			ERR_CHANOPRIVSNEEDED, (std::string[]){ chanName });
	else
	{
		chan->_topic = cmd._params[1];
		resp = appendUserNotif(
			user,
			(std::string[]){ "TOPIC", chanName, ":" + chan->_topic, "" },
			chan->_users, responseQueue, true
		);
	}
	pushToQueue(user->_fd, resp, responseQueue);
	return ;
}
