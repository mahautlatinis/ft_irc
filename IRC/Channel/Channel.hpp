#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "../../includes/Headers.hpp"
#include "../User/User.hpp"

#define CHAN_PREFIX	"#"

#define CHAN_VALID_MODES	"ikot"
#define CHAN_ALL_MODES		"biklmnopstv"

#define CHAN_ILLEGAL_CHARS	"\a,: "

class	Channel
{

private:
	string const		_name;		// Channel's name
	std::set<User *>	_users;		// List of users joined
	string	_topic;					// Channel's topic

	// Channel's modes

	bool	_invitationOnly;		// (i) Only invited user can join
	string	_key;					// (k) Channel's key
	std::set<User *>	_operators;	// (o) List of operators
	bool	_anyoneCanSetTopic;		// (t) Anyone can set topic

	std::set<User *>	_invitations;	// List of users being invited

public:
	static bool	IsPrefix(char c);
	static bool	NameLegal(string const &name);
	static bool	ModeNeedsParam(char mode, string &errorName);

	Channel(string const &name, User *creator);
	virtual ~Channel();

	int	TryAddUser(User *user, string const &key);
	int	RemoveUser(User *user);
	int	TrySetMode(IRC *irc, bool plus, char mode, string const &param);

	bool	IsOperator(User *user) const;
	bool	HasJoined(User *user) const;
	bool	HasKey() const;
	bool	IsInvited(User *user) const;
	string	GetModes() const;
	int		GetVisibleUsers() const;

	friend class IRC;
};

#endif
