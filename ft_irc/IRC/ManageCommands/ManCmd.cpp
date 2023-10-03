#include "../IRC.hpp"

// Handle unknown command by user
void	IRC::unknownCmd(Command const &cmd, std::vector<t_clientCmd> &responseQueue) const
{
	string	resp(
		getResponseFromCode(cmd._user, ERR_UNKNOWNCOMMAND, (string[]) { cmd._type })
	);
	pushToQueue(cmd._user->_fd, resp, responseQueue);
}

// Handle password not valid. Always return true
bool	IRC::passwordNotOK(User *user, std::vector<t_clientCmd> &responseQueue)
{
	string	resp(getErrorResponse(user, "Access denied by configuration"));
	pushToQueue(user->_fd, resp, responseQueue);
	ClientDisconnect(user->_fd);
	return true;
}

// Get a full NOTICE message to be sent to user
string	IRC::getNoticeMsg
	(string const &senderPrefix, User *user, string const &msg) const
{
	return string(
		senderPrefix + " NOTICE "
		+ user->_nick + " :" + msg
		+ CMD_DELIM
	); 
}

// Return a response with ERROR and a message
string	IRC::getErrorResponse(User *user, string const &msg) const
{
	return string(
		"ERROR :Closing link: ("
		+ user->_uname + "@" + USR_HOST + ") ["
		+ msg + "]"
		+ CMD_DELIM
	);
}

// Build a notification message from empty-terminated string array params,
// prefixed with user's prefix, then append the message to the set of
// recipients in dest to response queue. Return the built message
string	IRC::appendUserNotif
	(User *user, string params[], std::set<User *> const &dest, std::vector<t_clientCmd> &responseQueue, bool excludeUser) const
{
	string	msg(user->_prefix);
	for (int i = 0; !params[i].empty(); ++i)
		msg += " " + params[i];
	msg += CMD_DELIM;
	if (user->_prefix.empty())
		msg = msg.substr(1);

	std::set<User *>::iterator	it;
	for (it = dest.begin(); it != dest.end(); ++it)
		if (*it != user || !excludeUser)
			pushToQueue((*it)->_fd, msg, responseQueue);
	return msg;
}

void	IRC::pushToQueue(int fd, string const &msg, std::vector<t_clientCmd> &responseQueue) const
{
	if (fd == BOT_FD)
		return;
	responseQueue.push_back(std::make_pair(fd, msg));
}
