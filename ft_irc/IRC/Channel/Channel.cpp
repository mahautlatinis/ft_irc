/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:18:14 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:36:22 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "../IRC.hpp"

Channel::Channel(string const &name, User *creator) :
	_name(name),
	_invitationOnly(false),
	_anyoneCanSetTopic(false)
{
	_operators.insert(creator);
	_users.insert(creator);
}

Channel::~Channel()
{
	return ;
}

bool	Channel::isPrefix(char c)
{
	static string const	prefixSet(CHAN_PREFIX);
	return (prefixSet.find(c) != string::npos);
}

bool	Channel::isNameLegal(string const &name)
{
	if (!isPrefix(name[0]))
		return false;

	static string const	illegalChars(CHAN_ILLEGAL_CHARS);
	static std::set<char> const	illegalCharSet(illegalChars.begin(),
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

bool	Channel::modeNeedsParam(char mode, string &errorName)
{
	string	name;
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

int	Channel::tryAddUser(User *user, string const &key)
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

int	Channel::trySetMode(IRC *irc, bool plus, char mode, string const &param)
{
	static string	allModes(CHAN_ALL_MODES);
	static string	validModes(CHAN_VALID_MODES);

	if (allModes.find(mode) == string::npos)
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

bool	Channel::hasKey() const
{ 
	return !_key.empty();
}

bool	Channel::isInvited(User *user) const
{
	return (_invitations.find(user) != _invitations.end());
}

string	Channel::getModes() const
{
	string	mode("+");

	if (_invitationOnly)
		mode += 'i';
	if (!_key.empty())
		mode += 'k';
	if (!_anyoneCanSetTopic)
		mode += 't';
	return mode;
}

int		Channel::getVisibleUsers() const
{
	int	count(0);

	for (std::set<User *>::const_iterator it(_users.begin());
		it != _users.end(); ++it)
		count += (*it)->isVisible();
	return count;
}
