/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:00:59 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 19:01:00 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::part(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User				*user(cmd._user);
	string				resp;
	std::vector<string>	names;

	if (cmd._params.empty())
	{
		resp = getResponseFromCode(user, ERR_NEEDMOREPARAMS, (string[]){ cmd._type });
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}
	
	::strSplit(names, cmd._params[0], ",");

	for (std::vector<string>::iterator it(names.begin()); it != names.end(); ++it)
	{
		string const	&chanName(*it);
		Channel			*chan(getChannelByName(chanName));
		if (!chan)
		{
			resp = getResponseFromCode(user, ERR_NOSUCHCHANNEL, (string[]){ chanName });
			pushToQueue(user->_fd, resp, responseQueue);
			continue ;
		}
		else if (!chan->hasJoined(user))
		{
			resp = getResponseFromCode(user, ERR_NOTONCHANNEL, (string[]){ chanName });
			pushToQueue(user->_fd, resp, responseQueue);
			continue ;
		}
		else if (cmd._params.size() == 1)
			appendUserNotif(
				user,
				(string[]){ "PART", ":" + chanName, "" },
				chan->_users, responseQueue
			);
		else
			appendUserNotif(
				user,
				(string[]){ "PART", chanName, ":\"" + cmd._params[1] + "\"", "" },
				chan->_users, responseQueue
			);
		userLeaveChannel(user, chan);
	}
	return ;
}
