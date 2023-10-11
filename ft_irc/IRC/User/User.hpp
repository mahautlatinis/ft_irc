/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:11:25 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/11 18:28:36 by mahautlatin      ###   ########.fr       */
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
		int					_fd;			// fd of user's network client, used for unique identifier

		bool				_passwordOK;	// Has user entered good password ?
		bool				_registered;	// Is user registered ?
		bool				_invisible;		// (i) Is invisible
		bool				_oper;			// (o) Is server operator
		bool				_bot;			// (B) Is a bot

		string				_nick;			// Nickname
		string				_uname;			// Username
		string				_rname;			// Real name
		string				_prefix;		// User's prefix in network messages
		string				_awayMsg;		// (a) Away message

		std::set<Channel *>	_joined;		// List of channels user has joined
		
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

		static bool			checkNickValidChars(string const &nick);

		bool				isUsernameDefault() const;
		bool				isAway() const;
		bool				isVisible() const;

		void				setNick(string const &nick);
		void				setUsername(string const &uname);

		string				getModes() const;

		int					tryJoin(Channel *chan, string const &key);
		int					trySetMode(bool plus, char mode);

		friend class		IRC;

	protected:
		User(int fd, string const &botNick);
		void				registrationOK();
};