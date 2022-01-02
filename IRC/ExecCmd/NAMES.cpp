#include "../IRC.hpp"

void	IRC::execNAMES(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	std::vector<string>	chanNames;
	string	name;
	
	// If no param, name of channel is *, else take the first splitted string as name
	if (cmd._params.empty())
		name = "*";
	else
	{
		::StrSplit(chanNames, cmd._params[0], ",");
		name = chanNames.front();
	}

	User	*user(cmd._user);
	string	resp;
	Channel	*chan(getChannelByName(name));
	if (chan)
	{
		bool	joined(chan->HasJoined(user));
		string	names;
		std::set<User *>::iterator it;
		for (it = chan->_users.begin(); it != chan->_users.end(); ++it)
		{
			if (!joined && (*it)->_invisible)	// not showing invisible users
				continue;
			if (chan->IsOperator(*it))
				names += "@";
			names += (*it)->_nick + " ";
		}
		if (!names.empty())
		{
			if (names[names.size() - 1] == ' ')
				// Remove last character if it's a space
				names.erase(names.size() - 1, 1);
			resp = getResponseFromCode(
				user,
				RPL_NAMREPLY,
				(string[]){ "= " + name, names }
			);
		}
	}
	resp += getResponseFromCode(user, RPL_ENDOFNAMES, (string[]){ name });
	pushToQueue(user->_fd, resp, responseQueue);
}
