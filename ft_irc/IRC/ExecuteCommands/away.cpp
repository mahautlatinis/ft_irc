/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   away.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:26:34 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/13 19:33:41 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IRC.hpp>

void	IRC::away(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User		*user(cmd._user);
	std::string	resp;

	if (cmd._params.empty() || cmd._params[0].empty())
	{
		user->_awayMsg = "";
		resp = getResponseFromCode(user, RPL_UNAWAY, NULL);
	}
	else
	{
		user->_awayMsg = cmd._params[0];
		resp = getResponseFromCode(user, RPL_NOWAWAY, NULL);
	}
	pushToQueue(user->_fd, resp, responseQueue);
	return ;
}
