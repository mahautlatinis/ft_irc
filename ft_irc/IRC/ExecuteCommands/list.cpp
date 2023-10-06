/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LIST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:57:05 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 18:57:08 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	IRC::list(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User    					*user(cmd._user);
	string    					resp;
	std::map<string, Channel *>	chans;
	std::vector<string>			names;
	stringstream				ss;

	if (cmd._params.empty())
		chans = _channels;
	else
	{
		::strSplit(names, cmd._params[0], ",");
		for (std::vector<string>::iterator it(names.begin());
			it != names.end(); ++it)
		{
			Channel	*chan(getChannelByName(*it));
			if (chan)
				chans[*it] = chan;
		}
	}

	for (std::map<string, Channel *>::iterator it(chans.begin());
		it != chans.end(); ++it)
	{
		int	numVisible(it->second->getVisibleUsers());
		if (numVisible)
		{
			ss << numVisible;
			resp += getResponseFromCode(
				user, RPL_LIST,
				(string[]) {
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
