/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:58:48 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 09:41:18 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::oper(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User		*user(cmd._user);
	std::string	resp;

	if (cmd._params.size() < 2)
		resp = getResponseFromCode(user, 
			ERR_NEEDMOREPARAMS, (std::string[]){ cmd._type });
	else if (cmd._params[0] != OPER_USERNAME || cmd._params[1] != OPER_PASSWORD)
		resp = getResponseFromCode(user, ERR_PASSWDMISMATCH, NULL);
	else
	{
		user->_oper = true;
		resp = getResponseFromCode(user, RPL_YOUREOPER, NULL);
	}
	pushToQueue(user->_fd, resp, responseQueue);
	return ;
}
