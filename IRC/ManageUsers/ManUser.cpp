#include "../IRC.hpp"

// Return the pointer to the user with nickname nick, or NULL if not found
User	*IRC::getUserByNick(string const &nick) const
{
	for (std::map<int, User *>::const_iterator it(_users.begin());
		it != _users.end(); ++it)
		if (it->second->_nick == nick)
			return it->second;
	return NULL;
}

// Delete user from a channel when they leave
void	IRC::userLeaveChannel(User *user, Channel *chan)
{
	user->_joined.erase(chan);
	if (chan->RemoveUser(user) == 0)
	{
		_channels.erase(chan->_name);
		delete chan;
	}
}

// Delete user from every channel he is in
void	IRC::removeFromAllChannel(User *user)
{
	std::map<string, Channel *>	chansCopy(_channels);
	Channel	*chan;
	for (std::map<string, Channel *>::iterator it(chansCopy.begin());
		it != chansCopy.end(); ++it)
	{
		chan = it->second;
		if (chan->HasJoined(user))
			userLeaveChannel(user, chan);
		else if (chan->IsInvited(user))
			chan->_invitations.erase(user);
	}
}

// Send new user welcome messages with useful information about server
void	IRC::sendWelcomeMessage(User *user, std::vector<t_clientCmd> &responseQueue)
{
	string	resp(
		  getResponseFromCode(user, RPL_WELCOME, (string[]){ user->_prefix })
		+ getResponseFromCode(user, RPL_YOURHOST, NULL)
		+ getResponseFromCode(user, RPL_CREATED, NULL)
		+ getResponseFromCode(user, RPL_MYINFO, NULL)
	);
	pushToQueue(user->_fd, resp, responseQueue);
	// Create a MOTD command to be executed right away
	Command	cmdMOTD(user, "MOTD");
	execMOTD(cmdMOTD, responseQueue);
}
