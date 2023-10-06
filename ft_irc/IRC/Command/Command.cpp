/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:55:35 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:30:12 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

static std::set<string>	_cmdIgnored;
static std::set<string>	_cmdAvail;

Command::Command(User *user, string const &cmd): _user(user)
{
	string	cmdCopy(cmd);

	deduceType(cmdCopy);

	if (isExecutable())
	{
		buildParams(cmdCopy);
		if (_type == "TOPIC")
			treatTOPIC(cmd);

		else if (_type == "MODE")
			treatMODE();

		else if (_type == "PRIVMSG" || _type == "NOTICE")
			treatPRIVMSG_NOTICE();
	}
	return ;
}

Command::~Command(void)
{
	return ;
}

void	Command::initCommandList()
{
	if (!_cmdAvail.empty())
		return ;

	const char *ignored[] =
	{
		"CAP",
		"CNOTICE",
		"CPRIVMSG",
		"CONNECT",
		"ENCAP",
		"ERROR",
		"HELP",
		"INFO",
		"ISON",
		"KNOCK",
		"LINKS",
		"LUSERS",
		"NAMESX",
		"PONG",
		"REHASH",
		"RULES",
		"SERVER",
		"SERVICE",
		"SERVLIST",
		"SQUERY",
		"SQUIT",
		"SETNAME",
		"SILENCE",
		"STATS",
		"SUMMON",
		"TRACE",
		"UHNAMES",
		"USERHOST",
		"USERIP",
		"USERS",
		"WALLOPS",
		"WATCH",
		"WHOIS",
		"WHOWAS",
		NULL
	};

	const char *avail[] =
	{
		"ADMIN",
		"AWAY",
		"DIE",
		"INVITE",
		"JOIN",
		"KICK",
		"KILL",
		"LIST",
		"MODE",
		"MOTD",
		"NAMES",
		"NICK",
		"NOTICE",
		"OPER",
		"PART",
		"PASS",
		"PING",
		"PRIVMSG",
		"QUIT",
		"TIME",
		"TOPIC",
		"USER",
		"VERSION",
		"WHO",
		NULL
	};

	for (int i(0); ignored[i]; ++i)
		_cmdIgnored.insert(ignored[i]);

	for (int i(0); avail[i]; ++i)
		_cmdAvail.insert(avail[i]);

	return ;
}

void	Command::deduceType(string &cmd)
{
	size_t	i(cmd.find(' ', 0));

	if (i == string::npos)
	{
		_type = cmd;
		cmd = "";
	}
	else
	{
		_type = cmd.substr(0, i);
		cmd = cmd.substr(i, cmd.size() - i);
	}
	std::transform(_type.begin(), _type.end(), _type.begin(), ::toupper);
	return ;
}

void	Command::buildParams(string const &strParams)
{
	size_t	i(strParams.find(" :", 0));

	if (i == string::npos)
		i = strParams.size();

	string beforeLast(strParams.substr(0, i));
	::strSplit(_params, beforeLast, " ");

	i += 2;
	if (i < strParams.size())
		_params.push_back(strParams.substr(i, strParams.size() - i));
	return ;
}


void	Command::treatTOPIC(string const &rawCmd)
{
	if (rawCmd.find(" :", 0) != string::npos)
	{
		if (_params.empty())
			_params.push_back("*");
		else if (_params.size() == 1)
			_params.push_back("");
	}
	return ;
}

void	Command::treatMODE(void)
{
	if (_params.size() < 3)
		return;

	string res(_params[2]);

	for (size_t i(3); i < _params.size(); ++i)
		res += " " + _params[i];

	_params.erase(_params.begin() + 2, _params.end());
	_params.push_back(res);
	return ;
}

void	Command::treatPRIVMSG_NOTICE(void)
{
	if (_params.size() < 2)
		return ;

	string res(_params[1]);

	for (size_t i(2); i < _params.size(); ++i)
		res += " " + _params[i];

	_params.erase(_params.begin() + 1, _params.end());
	_params.push_back(res);
	return ;
}

bool	Command::isValid(void) const
{
	if (_type.empty())
		return false;

	return (_cmdIgnored.find(_type) != _cmdIgnored.end())
		|| (_cmdAvail.find(_type) != _cmdAvail.end());
}

bool	Command::isExecutable(void) const
{
	if (_type.empty())
		return false;

	return _cmdAvail.find(_type) != _cmdAvail.end();
}
