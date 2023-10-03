#include "../IRC.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void	IRC::execLIST(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User    *user(cmd._user);
	string    resp;
	std::map<string, Channel *>	chans;

	// Build list of channels being queried
	if (cmd._params.empty())
		chans = _channels;
	else
	{
		std::vector<string>	names;
		::StrSplit(names, cmd._params[0], ",");
		for (std::vector<string>::iterator it(names.begin());
			it != names.end(); ++it)
		{
			Channel	*chan(getChannelByName(*it));
			if (chan)
				chans[*it] = chan;
		}
	}

	for (std::map<string, Channel *>::iterator it(chans.begin());
		it != chans.end(); ++it)
	{
		int	numVisible(it->second->GetVisibleUsers());
		if (numVisible)
		{
			stringstream	ss; ss << numVisible;
			// Only showing channels with non-zero visible users
			resp += getResponseFromCode(
				user, RPL_LIST,
				(string[]) {
					it->first,
					ss.str(),
					it->second->_topic
				}
			);
		}
	}
	resp += getResponseFromCode(user, RPL_LISTEND, NULL);
	pushToQueue(user->_fd, resp, responseQueue);
}
