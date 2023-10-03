#include "../IRC.hpp"

void	IRC::execTOPIC(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User	*user(cmd._user);
	string	resp;

	if (cmd._params.empty())
	{
		resp = getResponseFromCode(user, ERR_NEEDMOREPARAMS, (string[]){ cmd._type });
		pushToQueue(user->_fd, resp, responseQueue);
		return;
	}

	string const	&chanName(cmd._params[0]);
	Channel	*chan(getChannelByName(chanName));
	if (!chan)
		// Channel not exist
		resp = getResponseFromCode(user, ERR_NOSUCHCHANNEL, (string[]){ chanName });
	else if (cmd._params.size() == 1)
		// Querying channel's topic
		resp = (chan->_topic.empty())
			 ? getResponseFromCode(user, RPL_NOTOPIC, (string[]){ chanName })
			 : getResponseFromCode(user, RPL_TOPIC, (string[]){ chanName, chan->_topic });
	else if (!chan->HasJoined(user))
		// User not joined channel
		resp = getResponseFromCode(user, ERR_NOTONCHANNEL, (string[]){ chanName });
	else if (!chan->IsOperator(user) && !chan->_anyoneCanSetTopic)
		// User trying to change topic is not operator of channel with mode +t
		resp = getResponseFromCode(user, ERR_CHANOPRIVSNEEDED, (string[]){ chanName });
	else
	{
		// User changed channel's topic, inform everyone
		chan->_topic = cmd._params[1];
		resp = appendUserNotif(
			user,
			(string[]){ "TOPIC", chanName, ":" + chan->_topic, "" },
			chan->_users, responseQueue, true
		);
	}
	pushToQueue(user->_fd, resp, responseQueue);
}
