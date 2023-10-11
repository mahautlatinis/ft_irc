/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:09:16 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/11 18:32:51 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"
#include "../Channel/Channel.hpp"

User::User(int fd):
	_fd(fd),
	_passwordOK(true),
	_registered(true),
	_invisible(false),
	_oper(false),
	_bot(false),
	_nick(DEFAULT_NAME),
	_uname(DEFAULT_NAME),
	_rname(DEFAULT_NAME),
	_prefix(string(":") + DEFAULT_NAME + '!' + DEFAULT_NAME + '@' + USR_HOST),
	_awayMsg("")
{
	return ;
}

User::User(int fd, string const &botNick):
	_fd(fd),
	_passwordOK(true),
	_registered(true),
	_invisible(false),
	_oper(false),
	_bot(true),
	_nick(botNick),
	_uname(botNick),
	_rname(botNick),
	_awayMsg("")
{
	return ;
}

User::~User(void)
{
	return ;
}

bool	User::checkNickValidChars(string const &nick)
{
	static string const			validChars(VALID_CHARS);
	static std::set<char> const	validChatSet(validChars.begin(),
		validChars.end());
	std::set<char>				nickCharSet(nick.begin(), nick.end());
	std::vector<char>			diff(nickCharSet.size());

	std::vector<char>::iterator	it(
		std::set_difference(
			nickCharSet.begin(), nickCharSet.end(),
			validChatSet.begin(), validChatSet.end(),
			diff.begin()
		)
	);
	return std::distance(diff.begin(), it) == 0;
}

void	User::registrationOK()
{
	_registered = true;
	_prefix = string(":") + _nick + '!' + _uname + '@' + USR_HOST;
	return ;
}


bool	User::isUsernameDefault() const
{
	return (_uname == DEFAULT_NAME);
}

bool	User::isAway() const
{
	return (!_awayMsg.empty());
}

bool	User::isVisible() const
{
	return !_invisible;
}

void	User::setNick(string const &nick)
{
	_nick = nick;
	if (_uname != DEFAULT_NAME)
		registrationOK();
	return ;
}

void	User::setUsername(string const &uname)
{
	_uname = uname;
	if (_nick != DEFAULT_NAME)
		registrationOK();
	return ;
}

string	User::getModes() const
{
	string	mode("+");
	if (isAway())
		mode += "a";
	if (_invisible)
		mode += "i";
	if (_oper)
		mode += "o";
	if (_bot)
		mode += "B";
	return mode;
}

int	User::tryJoin(Channel *chan, string const &key)
{
	if (_joined.find(chan) != _joined.end())
		return -1;

	int	ret(chan->tryAddUser(this, key));
	if (!ret)
		_joined.insert(chan);

	return ret;
}

int	User::trySetMode(bool plus, char mode)
{
	static string	allModes(USR_ALL_MODES);
	static string	validModes(USR_VALID_MODES);

	if (allModes.find(mode) == string::npos)
		return ERR_UMODEUNKNOWNFLAG;
	if (mode == 'a' && !plus && isAway())
	{
		_awayMsg = "";
		return 0;
	}
	if (mode == 'i' && plus != _invisible)
	{
		_invisible = !_invisible;
		return 0;
	}
	if (mode == 'o' && !plus && _oper)
	{
		_oper = false;
		return 0;
	}
	return -1;
}
