/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:14:57 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 10:59:46 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"
#include "includes/Commons.hpp"

void	IRC::chanWho(User *user, std::string const &mask, bool o,
	std::vector<t_clientCmd> &responseQueue) const
{
	Channel		*chan(getChannelByName(mask));
	std::string	resp;
	User		*u;

	if (!chan)
		return;

	for (std::set<User *>::const_iterator it(chan->_users.begin());
		it != chan->_users.end(); ++it)
	{
		u = (*it);
		if ((!chan->hasJoined(user) && user != u && u->_invisible)
			|| (o && !u->_oper))
			continue;
		std::string	flag = (u->isAway()) ? "G" : "H";
		if (u->_oper)
			flag += "*";
		if (chan->isOperator(u))
			flag += "@";
		resp += getResponseFromCode(
			user, RPL_WHOREPLY,
			(std::string[]){ mask, u->_uname, u->_nick, flag, u->_rname }
		);
	}
	pushToQueue(user->_fd, resp, responseQueue);
	return ;
}

void	IRC::userWho(User *user, std::string mask, bool o,
	std::vector<t_clientCmd> &responseQueue) const
{
	size_t		multiStarIdx;
	std::string	resp;
	User		*u;
	bool		showUser(false);

	while ((multiStarIdx = mask.find("**")) != std::string::npos)
		mask.replace(multiStarIdx, 2, "*");

	for (std::map<int, User *>::const_iterator it(_users.begin());
		it != _users.end(); ++it)
	{
		u = it->second;
		if (o && !u->_oper)
			continue ;

		if (u->_nick == mask || u->_rname == mask)
			showUser = true;
		else if (u == user || !u->_invisible)
			showUser = (strMatch(mask.c_str(), USR_HOST)
				|| strMatch(mask.c_str(), IRC_HOST)
				|| strMatch(mask.c_str(), u->_nick.c_str())
				|| strMatch(mask.c_str(), u->_rname.c_str()));

		if (showUser)
		{
			std::string flag = (u->isAway()) ? "G" : "H";

			if (u->_oper)
				flag += "*";
			resp += getResponseFromCode(
				user, RPL_WHOREPLY,
				(std::string[]){ "*", u->_uname, u->_nick, flag, u->_rname }
			);
		}
	}
	if (!resp.empty())
		pushToQueue(user->_fd, resp, responseQueue);
	return ;
}

void	IRC::who(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	std::string	resp;
	User	*user(cmd._user);

	if (cmd._params.empty())
	{
		resp = getResponseFromCode(user,
			ERR_NEEDMOREPARAMS, (std::string[]){ cmd._type });
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}
	
	std::string const	&mask(cmd._params[0]);
	bool	o(cmd._params.size() > 1 && cmd._params[1].find('o') != std::string::npos);

	if (Channel::isPrefix(mask[0]))
		chanWho(user, mask, o, responseQueue);
	else
		userWho(user, mask, o, responseQueue);
	resp = getResponseFromCode(user, RPL_ENDOFWHO, (std::string[]){ mask });

	pushToQueue(user->_fd, resp, responseQueue);
	return ;
}