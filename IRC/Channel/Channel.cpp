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
{}

// Check if a character is a valid channel name prefix
bool	Channel::IsPrefix(char c)
{
	static string const	prefixSet(CHAN_PREFIX);
	return (prefixSet.find(c) != string::npos);
}

// Check if name is legal to be used as channel's name
bool	Channel::NameLegal(string const &name)
{
	static string const	illegalChars(CHAN_ILLEGAL_CHARS);
	static std::set<char> const	illegalCharSet(illegalChars.begin(), illegalChars.end());

	if (!IsPrefix(name[0]))
		return false;
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

// If changing mode requires parameter, set error key as string and return true
bool	Channel::ModeNeedsParam(char mode, string &errorName)
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

// Attempt to add user to the channel using key. Return 0 if user
// is added successfully, or a numeric representation of error if fails
int	Channel::TryAddUser(User *user, string const &key)
{
	if (HasJoined(user))
		return -1;
	if (_invitationOnly && !IsInvited(user))
		return ERR_INVITEONLYCHAN;
	if (HasKey() && key != _key)
		return ERR_BADCHANNELKEY;
	_users.insert(user);
	_invitations.erase(user);
	return 0;
}

// Remove a user from channel. Return number of user left in the channel
int	Channel::RemoveUser(User *user)
{
	_users.erase(user);
	_operators.erase(user);
	return _users.size();
}

// Try setting channel's mode. Return 0 on success, -1 if ignored, else a
// positive error number
int	Channel::TrySetMode(IRC *irc, bool plus, char mode, string const &param)
{
	static string	allModes(CHAN_ALL_MODES);
	static string	validModes(CHAN_VALID_MODES);

	if (allModes.find(mode) == string::npos)
		// Mode not found
		return ERR_UNKNOWNMODE;

	if (mode == 'i' && plus != _invitationOnly)
	{
		_invitationOnly = !_invitationOnly;
		return 0;
	}
	if (mode == 'k' && HasKey() && !plus)	// -k <param>
	{
		// If param matches key, remove current key, return 0.
		// Otherwise return error
		if (param == _key)
			_key = "";
		return (param != _key) * ERR_KEYSET;	// 0 or ERR_KEYSET
	}
	if (mode == 'k' && !HasKey() && plus)	// +k <param>
	{
		// Set channel's new key
		_key = param;
		return 0;
	}
	if (mode == 'o')
	{
		User	*target(irc->getUserByNick(param));
		if (!target)
			return ERR_NOSUCHNICK;
		else if (plus && HasJoined(target) && !IsOperator(target))
		{
			_operators.insert(target);
			return 0;
		}
		else if (!plus && IsOperator(target))
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

// Check if user has joined channel
bool	Channel::HasJoined(User *user) const
{ return (_users.find(user) != _users.end()); }

// Check if user is operator of channel
bool	Channel::IsOperator(User *user)	const
{ return (_operators.find(user) != _operators.end()); }

// Check if channel key is set
bool	Channel::HasKey() const
{ return !_key.empty(); }

// Check if a user is in the invitation list
bool	Channel::IsInvited(User *user) const
{ return (_invitations.find(user) != _invitations.end()); }

// Get string representation of channel's modes
string	Channel::GetModes() const
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

// Get number of visible users in channel
int		Channel::GetVisibleUsers() const
{
	int	count(0);
	for (std::set<User *>::const_iterator it(_users.begin());
		it != _users.end(); ++it)
		count += (*it)->IsVisible();
	return count;
}
