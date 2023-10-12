/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:11:25 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 10:22:53 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../includes/Headers.hpp"

#define USR_VALID_MODES	"aioB"
#define USR_ALL_MODES	"aiorwsOB"

#define DEFAULT_NAME	"*"
#define VALID_CHARS		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-^_[]{}\\|"

class	User
{
	private:
		bool						_bot;
		bool						_invisible;
		bool						_oper;
		bool						_passwordOK;
		bool						_registered;
		int							_fd;
		std::set<Channel *>			_joined;
		std::string					_awayMsg;
		std::string					_nick;
		std::string					_prefix;
		std::string					_rname;
		std::string					_uname;
		
	public:
		// Functions implemented here were added just to respect the canonical convention
		User(void);
		User(User const &src)
		{
			*this = src;
			return ;
		};

		User(int fd);
		virtual ~User(void);

		User &operator=(User const &rhs)
		{
			(void)rhs;
			return *this;
		}

		static bool						checkNickValidChars(std::string const &nick);

		bool							isUsernameDefault(void) const;
		bool							isAway(void) const;
		bool							isVisible(void) const;

		void							setNick(std::string const &nick);
		void							setUsername(std::string const &uname);

		std::string						getModes(void) const;

		int								tryJoin(Channel *chan, std::string const &key);
		int								trySetMode(bool plus, char mode);

		friend class					IRC;

	protected:
		User(int fd, std::string const	&botNick);
		void							registrationOK();
};