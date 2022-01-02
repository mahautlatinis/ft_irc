#include "../IRC.hpp"

void	IRC::execNOTICE(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User	*user(cmd._user);
	int		fd(user->_fd);
	string	resp;

	if (cmd._params.empty())
		resp = getResponseFromCode(user, ERR_NORECIPIENT, (string[]){ cmd._type });
	else if (cmd._params.size() == 1)
		resp = getResponseFromCode(user, ERR_NOTEXTTOSEND, NULL);
	if (!resp.empty())
	{
		pushToQueue(fd, resp, responseQueue);
		return;
	}

	string const	&name(cmd._params[0]);
	string const	&msg(cmd._params[1]);
	if (Channel::IsPrefix(name[0]))
	{
		// Send notice to a channel
		Channel	*chan(getChannelByName(name));
		if (!chan)
			resp = getResponseFromCode(user, ERR_CANNOTSENDTOCHAN, (string[]){ name });
		else
			appendUserNotif(
				user,
				(string[]){ "NOTICE", name, ":" + msg, "" },
				chan->_users, responseQueue, true
			);
	}
	else
	{
		// Send notice to a user
		User	*target(getUserByNick(name));
		if (!(target = getUserByNick(name)))
			resp = getResponseFromCode(user, ERR_NOSUCHNICK, (string[]){ name });
		else
		{
			resp = getNoticeMsg(user->_prefix, target, msg);
			fd = target->_fd;
		}
	}
	if (!resp.empty())
		pushToQueue(fd, resp, responseQueue);
}
