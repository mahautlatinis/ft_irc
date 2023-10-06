/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:09:48 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 21:04:00 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Headers.hpp"
#include "User/User.hpp"
#include "User/Bot.hpp"
#include "Channel/Channel.hpp"
#include "Command/Command.hpp"

class	IRC
{
	public:
		IRC(string const &password);
		virtual ~IRC(void);

		User						*getUserByNick(string const &nick) const;
		Channel						*getChannelByName(string const &name) const;
		bool						ProcessClientCommand(t_clientCmd const &command, std::vector<t_clientCmd> &responseQueue);
		void						ClientDisconnect(int fd);
		int							GetVictim();

	private:
		time_t const				_startupTime;	// Start-up time
		string const				_svPassword;	// Server's password
		string const				_prefix;		// Server's prefix in responses
		std::map<int, User *>		_users;			// Users list
		std::map<string, Channel *>	_channels;		// Channels list
		int							_killing;		// FD of user being killed by an operator
		Bot							*_bot;			// Server's bot

		void						userLeaveChannel(User *user, Channel *chan);
		void						removeFromAllChannel(User *user);
		void						sendWelcomeMessage(User *user, std::vector<t_clientCmd> &responseQueue);

		Channel						*newChannel(string const &name, User *creator);
		std::set<User *>			getCommonUsers(User *user) const;

		void						unknownCmd(Command const &cmd, std::vector<t_clientCmd> &responseQueue) const;
		bool						passwordNotOK(User *user, std::vector<t_clientCmd> &responseQueue);
		string						getNoticeMsg(string const &senderPrefix, User *user, string const &msg) const;
		string						getErrorResponse(User *user, string const &msg) const;
		string						getResponseFromCode(User *user, int code, string params[]) const;
		string						appendUserNotif(User *user, string params[], std::set<User *> const &dest, std::vector<t_clientCmd> &responseQueue, bool excludeUser=false) const;
		void						pushToQueue(int fd, string const &msg, std::vector<t_clientCmd> &responseQueue) const;


		void						execCmd(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						admin(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						away(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						die(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						invite(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						join(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						kick(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						kill(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						list(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						mode(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						motd(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						names(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						nick(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						notice(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						oper(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						part(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						pass(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						ping(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						privmsg(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						quit(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						time(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						topic(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						user(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						version(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
		void						who(Command const &cmd, std::vector<t_clientCmd> &responseQueue);

		void						chanPrivmsg(User *user, string const &name, string const &msg, std::vector<t_clientCmd> &responseQueue);
		void						userPrivmsg(User *user, string const &name, string const &msg, std::vector<t_clientCmd> &responseQueue);

		void						chanMode(User *user, string const &chanName, string const &modes, string const &params, std::vector<t_clientCmd> &responseQueue);
		void						userMode(User *user, string const &nick, string const &modes, std::vector<t_clientCmd> &responseQueue);

		string						kickTarget(User *user, string const &nick, Channel *chan, string const &comment, std::vector<t_clientCmd> &responseQueue);

		void						chanWho(User *user, string const &mask, bool o, std::vector<t_clientCmd> &responseQueue) const;
		void						userWho(User *user, string mask, bool o, std::vector<t_clientCmd> &responseQueue) const;
};