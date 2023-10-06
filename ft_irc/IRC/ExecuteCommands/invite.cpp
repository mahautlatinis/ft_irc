/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:30:28 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:49:37 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::invite(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User			*user(cmd._user);
	string			resp;
	string const	&nick(cmd._params[0]);
	string const	&chanName(cmd._params[1]);
	User			*target(getUserByNick(nick));
	Channel			*chan(getChannelByName(chanName));

	if (cmd._params.size() < 2)
	{
		resp = getResponseFromCode(user, ERR_NEEDMOREPARAMS, (string[]){ cmd._type });
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}

	if (!target)
		resp = getResponseFromCode(user, ERR_NOSUCHNICK, (string[]){ nick });
	else if (!chan)
		resp = getResponseFromCode(user, ERR_NOSUCHCHANNEL, (string[]){ chanName });
	else if (!chan->hasJoined(user))
		resp = getResponseFromCode(user, ERR_NOTONCHANNEL, (string[]){ chanName });
	else if (chan->hasJoined(target))
		resp = getResponseFromCode(user, ERR_USERONCHANNEL, (string[]){ nick, chanName });
	else if (chan->_invitationOnly && !chan->isOperator(user))
		resp = getResponseFromCode(user, ERR_CHANOPRIVSNEEDED, (string[]){ chanName });
	else
	{
		if (target->isAway())
			resp = getResponseFromCode(user, RPL_AWAY, (string[]){ nick, target->_awayMsg });

		resp += getResponseFromCode(user, RPL_INVITING, (string[]){ nick, chanName });
		string invitation(
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
