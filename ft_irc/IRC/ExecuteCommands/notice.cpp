/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:57:37 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 09:41:01 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::notice(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User		*user(cmd._user);
	int			fd(user->_fd);
	std::string	resp;

	if (cmd._params.empty())
		resp = getResponseFromCode(user, 
			ERR_NORECIPIENT, (std::string[]){ cmd._type });
	else if (cmd._params.size() == 1)
		resp = getResponseFromCode(user, ERR_NOTEXTTOSEND, NULL);

	if (!resp.empty())
	{
		pushToQueue(fd, resp, responseQueue);
		return;
	}

	std::string const	&name(cmd._params[0]);
	std::string const	&msg(cmd._params[1]);

	if (Channel::isPrefix(name[0]))
	{
		Channel	*chan(getChannelByName(name));
		if (!chan)
			resp = getResponseFromCode(user, 
				ERR_CANNOTSENDTOCHAN, (std::string[]){ name });
		else
			appendUserNotif(
				user,
				(std::string[]){ "NOTICE", name, ":" + msg, "" },
				chan->_users, responseQueue, true
			);
	}
	else
	{
		User	*target(getUserByNick(name));
		if (!(target = getUserByNick(name)))
			resp = getResponseFromCode(user, ERR_NOSUCHNICK, (std::string[]){ name });
		else
		{
			resp = getNoticeMsg(user->_prefix, target, msg);
			fd = target->_fd;
		}
	}
	if (!resp.empty())
		pushToQueue(fd, resp, responseQueue);
	return ;
}
