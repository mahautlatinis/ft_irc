/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AWAY.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:27:39 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/03 18:27:39 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::execAWAY(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User	*user(cmd._user);
	string	resp;

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
}
