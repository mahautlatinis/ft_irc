/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   names.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:50:42 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 10:58:35 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::names(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	std::set<User *>::iterator	it;
	std::string					name;
	std::string					names;
	std::string					resp;
	std::vector<std::string>	chanNames;
	
	if (cmd._params.empty())
		name = "*";
	else
	{
		::strSplit(chanNames, cmd._params[0], ",");
		name = chanNames.front();
	}

	Channel	*chan(getChannelByName(name));
	User	*user(cmd._user);
	
	if (chan)
	{
		bool	joined(chan->hasJoined(user));
		for (it = chan->_users.begin(); it != chan->_users.end(); ++it)
		{
			if (!joined && (*it)->_invisible)
				continue ;
			if (chan->isOperator(*it))
				names += "@";
			names += (*it)->_nick + " ";
		}

		if (!names.empty())
		{;
			if (names[names.size() - 1] == ' ')
				names.erase(names.size() - 1, 1);
			resp = getResponseFromCode(
				user,
				RPL_NAMREPLY,
				(std::string[]){ "= " + name, names }
			);
		}
	}
	resp += getResponseFromCode(user,
		RPL_ENDOFNAMES, (std::string[]){ name });
	pushToQueue(user->_fd, resp, responseQueue);
	return ;
}