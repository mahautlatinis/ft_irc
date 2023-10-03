#include "../IRC.hpp"

void	IRC::execPART(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User	*user(cmd._user);
	string	resp;

	if (cmd._params.empty())
	{
		resp = getResponseFromCode(user, ERR_NEEDMOREPARAMS, (string[]){ cmd._type });
		pushToQueue(user->_fd, resp, responseQueue);
		return;
	}

	std::vector<string>	names;
	::StrSplit(names, cmd._params[0], ",");
	for (std::vector<string>::iterator it(names.begin()); it != names.end(); ++it)
	{
		string const	&chanName(*it);
		Channel	*chan(getChannelByName(chanName));
		if (!chan)
		{
			// Channel not found
			resp = getResponseFromCode(user, ERR_NOSUCHCHANNEL, (string[]){ chanName });
			pushToQueue(user->_fd, resp, responseQueue);
			continue;
		}
		else if (!chan->HasJoined(user))
		{
			// User not joined channel
			resp = getResponseFromCode(user, ERR_NOTONCHANNEL, (string[]){ chanName });
			pushToQueue(user->_fd, resp, responseQueue);
			continue;
		}
		else if (cmd._params.size() == 1)
			appendUserNotif(
				user,
				(string[]){ "PART", ":" + chanName, "" },
				chan->_users, responseQueue
			);
		else
			appendUserNotif(
				user,
				(string[]){ "PART", chanName, ":\"" + cmd._params[1] + "\"", "" },
				chan->_users, responseQueue
			);
		userLeaveChannel(user, chan);
	}
}
