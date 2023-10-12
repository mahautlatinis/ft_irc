/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:55:42 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 09:57:02 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd): _fd(fd)
{
	fcntl(fd, F_SETFL, O_NONBLOCK);
	return ;
}

Client::~Client(void)
{
	close(_fd);
	std::cout << "Client on socket #" << _fd << " disconnected." << std::endl;
	return ;
}

bool	Client::receiveCommand(std::string &cmd)
{
	ssize_t	r(recv(_fd, _buffer, BUFFER_SIZE, 0));
	if (r <= 0)
		return false;

	_buffer[r] = '\0';

	_cmdBuilder += _buffer;

	if (_cmdBuilder.find(CMD_DELIM, _cmdBuilder.size() - LEN_DELIM) != std::string::npos)
	{
		cmd = _cmdBuilder;
		_cmdBuilder.clear();
	}
	return true;
}

void	Client::sendResponse(std::string const &resp)
{
	send(_fd, resp.c_str(), resp.size(), 0);
	return ;
}
