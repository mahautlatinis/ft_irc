/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:53:19 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:29:55 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include "../../includes/Headers.hpp"
#include "../User/User.hpp"

#define CHAN_PREFIX			"#"
#define CHAN_VALID_MODES	"ikot"
#define CHAN_ALL_MODES		"biklmnopstv"
#define CHAN_ILLEGAL_CHARS	"\a,: "

class	Channel
{
	private:
		string const		_name;				// Channel's name
		std::set<User *>	_users;				// List of users joined
		string				_topic;				// Channel's topic
		bool				_invitationOnly;	// (i) Only invited user can join
		string				_key;				// (k) Channel's key
		std::set<User *>	_operators;			// (o) List of operators
		bool				_anyoneCanSetTopic;	// (t) Anyone can set topic
		std::set<User *>	_invitations;		// List of users being invited

	public:
		Channel(string const &name, User *creator);
		virtual ~Channel(void);

		static bool			isPrefix(char c);
		static bool			isNameLegal(string const &name);
		static bool			modeNeedsParam(char mode, string &errorName);

		int					tryAddUser(User *user, string const &key);
		int					removeUser(User *user);
		int					trySetMode(IRC *irc, bool plus, char mode, string const &param);
		bool				isOperator(User *user) const;
		bool				hasJoined(User *user) const;
		bool				hasKey() const;
		bool				isInvited(User *user) const;
		string				getModes() const;
		int					getVisibleUsers() const;

		friend class IRC;
};