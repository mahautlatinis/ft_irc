/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   die.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:27:11 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 09:31:12 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::die(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User	*user(cmd._user);

	if (!user->_oper)
	{
		std::string	resp(getResponseFromCode(user, ERR_NOPRIVILEGES, NULL));
		pushToQueue(user->_fd, resp, responseQueue);
		return;
	}
	std::cout << RED << user->_nick << " killed the server" << std::endl << NC;
	exit(0);
}
