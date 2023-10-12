/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageChannels.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:22:08 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 09:49:28 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

Channel	*IRC::newChannel(std::string const &name, User *creator)
{
	Channel	*chan(new Channel(name, creator));

	_channels[name] = chan;
	creator->_joined.insert(chan);
	return chan;
}

Channel	*IRC::getChannelByName(std::string const &name) const
{
	if (_channels.find(name) != _channels.end())
		return _channels.at(name);
	return NULL;
}

std::set<User *>	IRC::getCommonUsers(User *user) const
{
	std::set<User *>				results;
	std::set<Channel *>::iterator	it;

	for (it = user->_joined.begin(); it != user->_joined.end(); ++it)
		results.insert((*it)->_users.begin(), (*it)->_users.end());
	results.erase(user);
	return results;
}
