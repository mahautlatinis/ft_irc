/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:55:49 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 09:57:20 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Headers.hpp"

#define BUFFER_SIZE	512

class	Client
{
	public:
		virtual ~Client(void);
		friend class Server;

	private:
		Client(void);
		// Functions implemented here were added just to respect the canonical convention
		Client(Client const &src): _fd(src._fd)
		{
			*this = src;
			return ;
		};

		Client(int fd);
		Client &operator=(Client const &rhs)
		{
			(void)rhs;
			return *this;
		}

		char		_buffer[BUFFER_SIZE + 1];
		int const	_fd;
		std::string	_cmdBuilder;
		bool		receiveCommand(std::string &cmd);
		void		sendResponse(std::string const &resp);
};