/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:18:14 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 09:27:45 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "../IRC.hpp"

Channel::Channel(std::string const &name, User *creator):
	_anyoneCanSetTopic(false),
	_invitationOnly(false),
	_name(name)
{
	_operators.insert(creator);
	_users.insert(creator);
}

Channel::~Channel(void)
{
	return ;
}

bool	Channel::isPrefix(char c)
{
	static std::string const	prefixSet(CHAN_PREFIX);
	return (prefixSet.find(c) != std::string::npos);
}

bool	Channel::isNameLegal(std::string const &name)
{
	if (!isPrefix(name[0]))
		return false;

	static std::string const	illegalChars(CHAN_ILLEGAL_CHARS);
	static std::set<char> const illegalCharSet(illegalChars.begin(),
		illegalChars.end());

	std::set<char>		nameCharSet(name.begin() + 1, name.end());
	std::vector<char>	inter(illegalChars.size());

	std::vector<char>::iterator	it(
		std::set_intersection(
			illegalCharSet.begin(), illegalCharSet.end(),
			nameCharSet.begin(), nameCharSet.end(),
			inter.begin()
		)
	);	
	return std::distance(inter.begin(), it) == 0;
}

bool	Channel::modeNeedsParam(char mode, std::string &errorName)
{
	std::string	name;
	if (mode == 'k')
		name = "key";
	else if (mode == 'o')
		name = "op";
	if (!name.empty())
	{
		errorName = name;
		return true;
	}
	return false;
}

int	Channel::tryAddUser(User *user, std::string const &key)
{
	if (hasJoined(user))
		return -1;

	if (_invitationOnly && !isInvited(user))
		return ERR_INVITEONLYCHAN;

	if (hasKey() && key != _key)
		return ERR_BADCHANNELKEY;

	_users.insert(user);
	_invitations.erase(user);
	return 0;
}

int	Channel::removeUser(User *user)
{
	_users.erase(user);
	_operators.erase(user);
	return _users.size();
}

int	Channel::trySetMode(IRC *irc, bool plus, char mode, std::string const &param)
{
	static std::string	allModes(CHAN_ALL_MODES);
	static std::string	validModes(CHAN_VALID_MODES);

	if (allModes.find(mode) == std::string::npos)
		return ERR_UNKNOWNMODE;

	if (mode == 'i' && plus != _invitationOnly)
	{
		_invitationOnly = !_invitationOnly;
		return 0;
	}
	
	if (mode == 'k' && hasKey() && !plus)
	{
		if (param == _key)
			_key = "";
		return (param != _key) * ERR_KEYSET;
	}

	if (mode == 'k' && !hasKey() && plus)
	{
		_key = param;
		return 0;
	}

	if (mode == 'o')
	{
		User	*target(irc->getUserByNick(param));
	
		if (!target)
			return ERR_NOSUCHNICK;

		else if (plus && hasJoined(target) && !isOperator(target))
		{
			_operators.insert(target);
			return 0;
		}
		else if (!plus && isOperator(target))
		{
			_operators.erase(target);
			return 0;
		}
	}
	if (mode == 't' && plus == _anyoneCanSetTopic)
	{
		_anyoneCanSetTopic = !_anyoneCanSetTopic;
		return 0;
	}
	return -1;
}

bool	Channel::hasJoined(User *user) const
{
	return (_users.find(user) != _users.end());
}


bool	Channel::isOperator(User *user)	const
{
	return (_operators.find(user) != _operators.end());
}

bool	Channel::hasKey(void) const
{ 
	return !_key.empty();
}

bool	Channel::isInvited(User *user) const
{
	return (_invitations.find(user) != _invitations.end());
}

std::string	Channel::getModes(void) const
{
	std::string	mode("+");

	if (_invitationOnly)
		mode += 'i';
	if (!_key.empty())
		mode += 'k';
	if (!_anyoneCanSetTopic)
		mode += 't';
	return mode;
}

int		Channel::getVisibleUsers(void) const
{
	int	count(0);

	for (std::set<User *>::const_iterator it(_users.begin());
		it != _users.end(); ++it)
		count += (*it)->isVisible();
	return count;
}
