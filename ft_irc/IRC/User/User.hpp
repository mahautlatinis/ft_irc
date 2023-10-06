/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:11:25 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:17:58 by mahautlatin      ###   ########.fr       */
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
	public:
		

		User(int fd);
		virtual ~User(void);

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

	private:
		int					_fd;			// fd of user's network client, used for unique identifier
		bool				_passwordOK;	// Has user entered good password ?
		bool				_registered;	// Is user registered ?
		string				_nick;			// Nickname
		string				_uname;			// Username
		string				_rname;			// Real name
		string				_prefix;		// User's prefix in network messages
		std::set<Channel *>	_joined;		// List of channels user has joined

		string				_awayMsg;		// (a) Away message
		bool				_invisible;		// (i) Is invisible
		bool				_oper;			// (o) Is server operator
		bool				_bot;			// (B) Is a bot

	protected:
		User(int fd, string const &botNick);
		void				registrationOK();
};