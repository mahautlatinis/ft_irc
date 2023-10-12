/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:00:59 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 09:44:01 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::part(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User						*user(cmd._user);
	std::string					resp;
	std::vector<std::string>	names;

	if (cmd._params.empty())
	{
		resp = getResponseFromCode(user, 
			ERR_NEEDMOREPARAMS, (std::string[]){ cmd._type });
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}
	
	::strSplit(names, cmd._params[0], ",");

	for (std::vector<std::string>::iterator it(names.begin()); it != names.end(); ++it)
	{
		std::string const	&chanName(*it);
		Channel				*chan(getChannelByName(chanName));
		if (!chan)
		{
			resp = getResponseFromCode(user, 
				ERR_NOSUCHCHANNEL, (std::string[]){ chanName });
			pushToQueue(user->_fd, resp, responseQueue);
			continue ;
		}
		else if (!chan->hasJoined(user))
		{
			resp = getResponseFromCode(user, 
				ERR_NOTONCHANNEL, (std::string[]){ chanName });
			pushToQueue(user->_fd, resp, responseQueue);
			continue ;
		}
		else if (cmd._params.size() == 1)
			appendUserNotif(
				user,
				(std::string[]){ "PART", ":" + chanName, "" },
				chan->_users, responseQueue
			);
		else
			appendUserNotif(
				user,
				(std::string[]){ "PART", chanName, ":\"" + cmd._params[1] + "\"", "" },
				chan->_users, responseQueue
			);
		userLeaveChannel(user, chan);
	}
	return ;
}
