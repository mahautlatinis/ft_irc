/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:07:18 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/13 19:35:35 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IRC.hpp>

void	IRC::time(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	time_t	now(::time(NULL));
	std::string	timeStr(ctime(&now));

	timeStr.erase(timeStr.size() - 1, 1);

	User	*user(cmd._user);
	std::string	resp(getResponseFromCode(user, 
		RPL_TIME, (std::string[]){ timeStr }));
	pushToQueue(user->_fd, resp, responseQueue);
	return ;
}
