#include "../IRC.hpp"

void	IRC::execJOIN(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User	*user(cmd._user);
	string	resp;

	if (cmd._params.empty())
	{
		resp = getResponseFromCode(user, ERR_NEEDMOREPARAMS, (string[]){ cmd._type });
		pushToQueue(user->_fd, resp, responseQueue);
		return;
	}

	std::vector<string>	chanNames, chanKeys;
	::StrSplit(chanNames, cmd._params[0], ",");
	if (cmd._params.size() > 1)
		::StrSplit(chanKeys, cmd._params[1], ",");

	for (size_t i(0); i < chanNames.size(); ++i)
	{
		string const	&name(chanNames[i]);
		string const	&key = (i < chanKeys.size())
							 ? chanKeys[i]
							 : "";
		if (!Channel::NameLegal(name))
		{
			resp = getResponseFromCode(user, ERR_BADCHANMASK, (string[]){ name });
			pushToQueue(user->_fd, resp, responseQueue);
			continue;
		}
		
		Channel	*chan(getChannelByName(name));
		int	res(0);
		if (!chan)
			chan = newChannel(name, user);
		else
			res = user->TryJoin(chan, key);
		if (res > 0)
		{
			resp = getResponseFromCode(user, res, (string[]){ name });
			pushToQueue(user->_fd, resp, responseQueue);
		}
		else if (!res)
		{
			// Inform everyone in the channel that user has just joined
			resp = appendUserNotif(
				user,
				(string[]){ "JOIN", ":" + name, "" },
				chan->_users,
				responseQueue
			);
			// Create and execute TOPIC and NAMES
			if (!chan->_topic.empty())
				execTOPIC(Command(user, "TOPIC " + name), responseQueue);
			execNAMES(Command(user, "NAMES " + name), responseQueue);
		}
	}
}
