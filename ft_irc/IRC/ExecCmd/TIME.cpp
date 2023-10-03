/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TIME.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:28:39 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/03 18:28:40 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::execTIME(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	time_t	now(time(NULL));
	string	timeStr(ctime(&now));
	timeStr.erase(timeStr.size() - 1, 1);

	User	*user(cmd._user);
	string	resp(getResponseFromCode(user, RPL_TIME, (string[]){ timeStr }));
	pushToQueue(user->_fd, resp, responseQueue);
}
