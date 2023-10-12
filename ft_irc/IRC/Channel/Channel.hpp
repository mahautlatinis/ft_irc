/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:53:19 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 09:28:51 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include "../../includes/Headers.hpp"
#include "../User/User.hpp"


#define CHAN_ALL_MODES		"biklmnopstv"
#define CHAN_ILLEGAL_CHARS	"\a,: "
#define CHAN_PREFIX			"#"
#define CHAN_VALID_MODES	"ikot"

class	Channel
{
	private:
		bool				_anyoneCanSetTopic;	// (t) Anyone can set topic
		bool				_invitationOnly;	// (i) Only invited user can join
		std::set<User *>	_invitations;		// List of users being invited
		std::set<User *>	_operators;			// (o) List of operators
		std::set<User *>	_users;				// List of users joined
		std::string			_key;				// (k) Channel's key
		std::string			_topic;				// Channel's topic
		std::string const	_name;				// Channel's name

	public:
		// Functions implemented here were added just to respect the canonical convention
		Channel(void): _anyoneCanSetTopic(false), _invitationOnly(false), _name("default")
		{
			return ; 
		};

		Channel(Channel const &src)
		{
			*this = src;
		};

		virtual ~Channel(void);
		Channel(std::string const &name, User *creator);

		Channel &operator=(Channel const &rhs)
		{
			(void)rhs;
			return (*this); 
		};

		int					getVisibleUsers(void) const;
		int					removeUser(User *user);
		int					tryAddUser(User *user, std::string const &key);
		int					trySetMode(IRC *irc, bool plus, char mode, std::string const &param);
		bool				hasJoined(User *user) const;
		bool				hasKey(void) const;
		bool				isInvited(User *user) const;
		bool				isOperator(User *user) const;

		static bool			isNameLegal(std::string const &name);
		static bool			isPrefix(char c);
		static bool			modeNeedsParam(char mode, std::string &errorName);

		std::string			getModes(void) const;

		friend class IRC;
};