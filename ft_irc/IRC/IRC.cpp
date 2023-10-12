/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:56:21 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 11:04:13 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

IRC::IRC(std::string const &password):
	_startupTime(::time(NULL)),
	_svPassword(password),
	_prefix(std::string(":") + IRC_HOST),
	_killing(-1)
{
	Command::initCommandList();

	_bot = new Bot();
	_users[BOT_FD] = _bot;
	return ; 
}

IRC::~IRC(void)
{
	std::map<int, User *>::iterator				iuit;
	std::map<std::string, Channel *>::iterator	scit;

	for (iuit = _users.begin(); iuit != _users.end(); ++iuit)
		delete iuit->second;

	for (scit = _channels.begin(); scit != _channels.end(); ++scit)
		delete scit->second;

	return ;
}

bool	IRC::processClientCommand(t_clientCmd const &command,
	std::vector<t_clientCmd> &responseQueue)
{
	User						*user;
	int							fd(command.first);
	std::vector<std::string>	subCmds;
	std::string					msg;

	if (_users.find(fd) == _users.end())
	{
		user = (_users[fd] = new User(fd));
		if (_svPassword.empty())
			user->_passwordOK = true;

		msg = getNoticeMsg(
			_prefix, user,
			std::string("*** Your hostname is set to ") + USR_HOST
				+ " like everybody else."
		);
		pushToQueue(fd, getNoticeMsg(_prefix, user, msg), responseQueue);
	}
	else
		user = _users[fd];

	::strSplit(subCmds, command.second, CMD_DELIM);

	for (std::vector<std::string>::iterator it(subCmds.begin());
		it != subCmds.end(); ++it)
	{
		Command	cmd(user, *it);
		bool	reg(user->_registered);

		if (!cmd.isValid())
			unknownCmd(cmd, responseQueue);

		else if (cmd.isExecutable())
		{
			execCmd(cmd, responseQueue);

			if (cmd._type == "QUIT")
				return true;

			if (!user->_passwordOK)
				return passwordNotOK(user, responseQueue);

			if (!reg && user->_registered)
			{
				sendWelcomeMessage(user, responseQueue);
				Command	botPRIVMSG(
					_bot,
					"PRIVMSG " + user->_nick + " :"
						+ _bot->getWelcomeMsg(user->_nick)
				);
				privmsg(botPRIVMSG, responseQueue);
			}

		}
	}
	return false;
}

void	IRC::clientDisconnect(int fd)
{
	User	*user(_users[fd]);
	
	if (_users.find(fd) != _users.end())
	{
		if (user->_registered)
			removeFromAllChannel(user);
		delete user;
		_users.erase(fd);
	}
	return ; 
}

int	IRC::getVictim(void)
{
	int	res = _killing;
	_killing = -1;
	return res;
}

void	IRC::execCmd(Command const &cmd,
	std::vector<t_clientCmd> &responseQueue)
{
	if (cmd._type == "ADMIN")
		admin(cmd, responseQueue);
	else if (cmd._type == "AWAY")
		away(cmd, responseQueue);
	else if (cmd._type == "DIE")
		die(cmd, responseQueue);
	else if (cmd._type == "INVITE")
		invite(cmd, responseQueue);
	else if (cmd._type == "JOIN")
		join(cmd, responseQueue);
	else if (cmd._type == "KICK")
		kick(cmd, responseQueue);
	else if (cmd._type == "KILL")
		kill(cmd, responseQueue);
	else if (cmd._type == "LIST")
		list(cmd, responseQueue);
	else if (cmd._type == "MODE")
		mode(cmd, responseQueue);
	else if (cmd._type == "MOTD")
		motd(cmd, responseQueue);
	else if (cmd._type == "NAMES")
		names(cmd, responseQueue);
	else if (cmd._type == "NICK")
		nick(cmd, responseQueue);
	else if (cmd._type == "NOTICE")
		notice(cmd, responseQueue);
	else if (cmd._type == "OPER")
		oper(cmd, responseQueue);
	else if (cmd._type == "PART")
		part(cmd, responseQueue);
	else if (cmd._type == "PASS")
		pass(cmd, responseQueue);
	else if (cmd._type == "PING")
		ping(cmd, responseQueue);
	else if (cmd._type == "PRIVMSG")
		privmsg(cmd, responseQueue);
	else if (cmd._type == "QUIT")
		quit(cmd, responseQueue);
	else if (cmd._type == "TIME")
		time(cmd, responseQueue);
	else if (cmd._type == "TOPIC")
		topic(cmd, responseQueue);
	else if (cmd._type == "USER")
		user(cmd, responseQueue);
	else if (cmd._type == "VERSION")
		version(cmd, responseQueue);
	else if (cmd._type == "WHO")
		who(cmd, responseQueue);
	return ;
}
