/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:16:59 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/13 19:35:46 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IRC.hpp>

void	IRC::user(Command const &cmd,
	std::vector<t_clientCmd> &responseQueue)
{
	User		*user(cmd._user);
	std::string	resp;

	if (!user->isUsernameDefault())
		resp = getResponseFromCode(user, ERR_ALREADYREGISTRED, NULL);
	else if (cmd._params.size() < 4)
		resp = getResponseFromCode(user, ERR_NEEDMOREPARAMS, 
			(std::string[]){ cmd._type });
	else
	{
		user->setUsername(cmd._params[0]);
		user->_rname = cmd._params[3];
	}
	if (!resp.empty())
		pushToQueue(user->_fd, resp, responseQueue);
	return ;
}
