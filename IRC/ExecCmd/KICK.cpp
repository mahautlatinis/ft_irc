#include "../IRC.hpp"

// User kicks target having nickname nick from chan with acomment. Return the
// individual kick message to be added to response queue
string	IRC::kickTarget
	(User *user, string const &nick, Channel *chan, string const &comment, std::vector<t_clientCmd> &responseQueue)
{
	User	*target(getUserByNick(nick));
	string	resp;

	if (!target)
		resp = getResponseFromCode(user, ERR_NOSUCHNICK, (string[]){ nick });
	else if (!chan->HasJoined(target))
		resp = getResponseFromCode(user, ERR_USERNOTINCHANNEL, (string[]){ nick, chan->_name });
	if (!resp.empty())
	{
		pushToQueue(user->_fd, resp, responseQueue);
		return "";
	}

	userLeaveChannel(target, chan);
	resp = user->_prefix
		 + " KICK "
		 + chan->_name + " "
		 + nick + " :"
		 + comment + CMD_DELIM;
	pushToQueue(target->_fd, resp, responseQueue);
	return resp;
}

void	IRC::execKICK(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	string	resp;
	User	*user(cmd._user);

	if (cmd._params.size() < 2)
	{
		resp = getResponseFromCode(user, ERR_NEEDMOREPARAMS, (string[]){ cmd._type });
		pushToQueue(user->_fd, resp, responseQueue);
		return;
	}
	
	string const	&chanName(cmd._params[0]);
	Channel			*chan(getChannelByName(chanName));
	if (!chan)
		resp = getResponseFromCode(user, ERR_NOSUCHCHANNEL, (string[]){ chanName });
	else if (!chan->HasJoined(user))
		resp = getResponseFromCode(user, ERR_NOTONCHANNEL, (string[]){ chanName });
	else if (!chan->IsOperator(user))
		resp = getResponseFromCode(user, ERR_CHANOPRIVSNEEDED, (string[]){ chanName });
	if (!resp.empty())
	{
		pushToQueue(user->_fd, resp, responseQueue);
		return;
	}

	std::vector<string>	nicks;
	::StrSplit(nicks, cmd._params[1], ",");
	string	comment = (cmd._params.size() == 2)
					? user->_nick
					: cmd._params[2];
	string	msgToAll;
	for (std::vector<string>::iterator it(nicks.begin()); it != nicks.end(); ++it)
		msgToAll += kickTarget(user, *it, chan, comment, responseQueue);
	
	for (std::set<User *>::iterator it(chan->_users.begin());
		it != chan->_users.end(); ++it)
		pushToQueue((*it)->_fd, msgToAll, responseQueue);
}
