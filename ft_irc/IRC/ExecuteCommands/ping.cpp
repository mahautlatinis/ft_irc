/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:02:24 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 10:59:07 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::ping(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	std::stringstream	ss;
	std::string			param = cmd._params.empty()
							? ""
							: cmd._params[0];

	ss << _prefix << " PONG " << IRC_HOST << " :" << param << CMD_DELIM;
	pushToQueue(cmd._user->_fd, ss.str(), responseQueue);
	return ;
}
