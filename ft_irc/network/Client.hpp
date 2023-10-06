/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:55:49 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 19:57:57 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../includes/Headers.hpp"

#define BUFFER_SIZE	512

class	Client
{
	public:
		virtual ~Client();
		friend class Server;

	private:
		Client(void);
		Client(int fd);

		char		_buffer[BUFFER_SIZE + 1];
		int const	_fd;
		string		_cmdBuilder;
		bool		receiveCommand(string &cmd);
		void		sendResponse(string const &resp);
};

#endif
