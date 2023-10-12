/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:30:28 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 10:56:20 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::invite(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	std::string const	&chanName(cmd._params[1]);
	Channel				*chan(getChannelByName(chanName));
	std::string			resp;
	std::string const	&nick(cmd._params[0]);
	User				*target(getUserByNick(nick));
	User				*user(cmd._user);

	if (cmd._params.size() < 2)
	{
		resp = getResponseFromCode(user,
			ERR_NEEDMOREPARAMS, (std::string[]){ cmd._type });
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}

	if (!target)
		resp = getResponseFromCode(user, 
			ERR_NOSUCHNICK, (std::string[]){ nick });
	else if (!chan)
		resp = getResponseFromCode(user, 
			ERR_NOSUCHCHANNEL, (std::string[]){ chanName });
	else if (!chan->hasJoined(user))
		resp = getResponseFromCode(user, 
			ERR_NOTONCHANNEL, (std::string[]){ chanName });
	else if (chan->hasJoined(target))
		resp = getResponseFromCode(user, 
			ERR_USERONCHANNEL, (std::string[]){ nick, chanName });
	else if (chan->_invitationOnly && !chan->isOperator(user))
		resp = getResponseFromCode(user, 
			ERR_CHANOPRIVSNEEDED, (std::string[]){ chanName });
	else
	{
		if (target->isAway())
			resp = getResponseFromCode(user, 
				RPL_AWAY, (std::string[]){ nick, target->_awayMsg });

		resp += getResponseFromCode(user, 
			RPL_INVITING, (std::string[]){ nick, chanName });
		std::string invitation(
			user->_prefix
			+ " INVITE "
			+ nick + " :"
			+ chanName + CMD_DELIM
		);
		pushToQueue(target->_fd, invitation, responseQueue);
		chan->_invitations.insert(target);
	}
	pushToQueue(user->_fd, resp, responseQueue);
	return ; 
}
