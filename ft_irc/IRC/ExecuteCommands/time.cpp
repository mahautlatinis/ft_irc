/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TIME.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:07:18 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 19:08:44 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::time(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	time_t	now(::time(NULL));
	string	timeStr(ctime(&now));

	timeStr.erase(timeStr.size() - 1, 1);

	User	*user(cmd._user);
	string	resp(getResponseFromCode(user, RPL_TIME, (string[]){ timeStr }));
	pushToQueue(user->_fd, resp, responseQueue);
	return ;
}
