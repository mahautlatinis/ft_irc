/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:09:16 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/13 19:32:40 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <User.hpp>
#include <Channel.hpp>

User::User(int fd):
	_bot(false),
	_invisible(false),
	_oper(false),
	_passwordOK(false),
	_registered(false),
	_fd(fd),
	_awayMsg(""),
	_nick(DEFAULT_NAME),
	_prefix(std::string(":") + DEFAULT_NAME + '!' + DEFAULT_NAME + '@' + USR_HOST),
	_rname(DEFAULT_NAME),
	_uname(DEFAULT_NAME)
{
	return ;
}

User::User(int fd, std::string const &botNick):
	_bot(true),
	_invisible(false),
	_oper(false),
	_passwordOK(true),
	_registered(true),
	_fd(fd),
	_awayMsg(""),
	_nick(botNick),
	_rname(botNick),
	_uname(botNick)
{
	return ;
}

User::~User(void)
{
	return ;
}

bool	User::checkNickValidChars(std::string const &nick)
{
	static std::string const	validChars(VALID_CHARS);
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

void	User::registrationOK(void)
{
	_registered = true;
	_prefix = std::string(":") + _nick + '!' + _uname + '@' + USR_HOST;
	return ;
}


bool	User::isUsernameDefault(void) const
{
	return (_uname == DEFAULT_NAME);
}

bool	User::isAway(void) const
{
	return (!_awayMsg.empty());
}

bool	User::isVisible(void) const
{
	return !_invisible;
}

void	User::setNick(std::string const &nick)
{
	_nick = nick;
	if (_uname != DEFAULT_NAME)
		registrationOK();
	return ;
}

void	User::setUsername(std::string const &uname)
{
	_uname = uname;
	if (_nick != DEFAULT_NAME)
		registrationOK();
	return ;
}

std::string	User::getModes() const
{
	std::string	mode("+");
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

int	User::tryJoin(Channel *chan, std::string const &key)
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
	static std::string	allModes(USR_ALL_MODES);
	static std::string	validModes(USR_VALID_MODES);

	if (allModes.find(mode) == std::string::npos)
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
