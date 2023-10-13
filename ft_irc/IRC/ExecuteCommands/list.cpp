/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:57:05 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/13 19:34:16 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IRC.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	IRC::list(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	std::map<std::string, Channel *>		chans;
	std::string    							resp;
	std::vector<std::string>				names;
	std::stringstream						ss;
	User    								*user(cmd._user);

	if (cmd._params.empty())
		chans = _channels;
	else
	{
		::strSplit(names, cmd._params[0], ",");
		for (std::vector<std::string>::iterator it(names.begin());
			it != names.end(); ++it)
		{
			Channel	*chan(getChannelByName(*it));
			if (chan)
				chans[*it] = chan;
		}
	}

	for (std::map<std::string, Channel *>::iterator it(chans.begin());
		it != chans.end(); ++it)
	{
		int	numVisible(it->second->getVisibleUsers());
		if (numVisible)
		{
			ss << numVisible;
			resp += getResponseFromCode(
				user, RPL_LIST,
				(std::string[]) {
					it->first,
					ss.str(),
					it->second->_topic
				}
			);
		}
	}
	resp += getResponseFromCode(user, RPL_LISTEND, NULL);
	pushToQueue(user->_fd, resp, responseQueue);
	return ;
}
