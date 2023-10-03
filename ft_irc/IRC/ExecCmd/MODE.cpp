#include "../IRC.hpp"

void	IRC::execMODE(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User	*user(cmd._user);

	if (cmd._params.empty())
	{
		string resp(getResponseFromCode(user, ERR_NEEDMOREPARAMS, (string[]){ cmd._type }));
		pushToQueue(user->_fd, resp, responseQueue);
		return;
	}

	string const	&name(cmd._params[0]);
	string const	&modes = (cmd._params.size() < 2)
						   ? "" : cmd._params[1];
	string const	&params = (cmd._params.size() < 3)
						   ? "" : cmd._params[2];
	if (Channel::IsPrefix(name[0]))
		chanMODE(user, name, modes, params, responseQueue);
	else
		userMODE(user, name, modes, responseQueue);
}

// Execute MODE targeting a channel
void	IRC::chanMODE
	(User *user, string const &chanName, string const &modes, string const &params, std::vector<t_clientCmd> &responseQueue)
{
	string	resp;
	Channel	*chan(getChannelByName(chanName));

	if (!chan)
		resp = getResponseFromCode(user, ERR_NOSUCHCHANNEL, (string[]){ chanName });
	else if (modes.empty())
	{
		// Querying channel's mode
		string	chanKey;
		if (chan->HasKey())
			chanKey = (chan->HasJoined(user))
					? chan->_key
					: "<key>";
		resp = getResponseFromCode(
			user, RPL_CHANNELMODEIS,
			(string[]){ chanName, chan->GetModes(), chanKey }
		);
	}
	else if (!chan->IsOperator(user))
		resp = getResponseFromCode(user, ERR_CHANOPRIVSNEEDED, (string[]){ chanName });
	if (!resp.empty())
	{
		pushToQueue(user->_fd, resp, responseQueue);
		return;
	}
	
	std::vector<string>	paramsList;
	::StrSplit(paramsList, params, " ");
	size_t	paramIdx(0);

	bool	plus(modes[0] != '-');
	size_t	i((plus && modes[0] == '+') || !plus);	// if first char is +/-, start at second char
	string	modeChanges;
	std::vector<string>	paramChanges;

	for (; i < modes.size(); ++i)
	{
		// Looping over each character
		string param, errorName;
		if (Channel::ModeNeedsParam(modes[i], errorName) && paramIdx < paramsList.size())
			param = paramsList[paramIdx++];
		if (param.empty() && !errorName.empty())
		{
			// If mode requires parameter and param is empty, then error
			resp += getResponseFromCode(
				user, ERR_CUST_CMODEPARAM,
				(string[]){ chanName, modes.substr(i, 1), errorName }
			);
			continue;
		}

		int	res(chan->TrySetMode(this, plus, modes[i], param));
		if (res > 0)
			resp += getResponseFromCode(
				user, res,
				(string[]){ param, modes.substr(i, 1), chanName, errorName }
			);
		else if (!res)
		{
			modeChanges += modes[i];
			if (!param.empty())
				paramChanges.push_back(param);
		}
	}
	if (!resp.empty())
		pushToQueue(user->_fd, resp, responseQueue);

	if (!modeChanges.empty())
	{
		// If there is some valid changes, notify everyone in channel
		modeChanges = plus
					? "+" + modeChanges
					: "-" + modeChanges;
		string	msgToAll;
		if (paramChanges.empty())
			msgToAll += ":" + modeChanges;
		else
		{
			msgToAll += modeChanges;
			paramChanges.back() = ":" + paramChanges.back();	// Add ':' to the beginning of the last string
			for (std::vector<string>::iterator it(paramChanges.begin());
				it != paramChanges.end(); ++it)
				msgToAll += " " + (*it);
		}
		appendUserNotif(
			user,
			(string[]){ "MODE", chanName, msgToAll, "" },
			chan->_users, responseQueue
		);
	}
}

// Execute MODE targeting a user
void	IRC::userMODE
	(User *user, string const &nick, string const &modes, std::vector<t_clientCmd> &responseQueue)
{
	string	resp;

	if (nick != user->_nick)
		// User should only be able to set their own mode
		resp = getResponseFromCode(user, ERR_USERSDONTMATCH, NULL);
	else if (modes.empty())
		// Query user's own mode
		resp = getResponseFromCode(user, RPL_UMODEIS, (string[]){ user->GetModes() });
	if (!resp.empty())
	{
		pushToQueue(user->_fd, resp, responseQueue);
		return;
	}

	bool	plus(modes[0] != '-');
	size_t	i((plus && modes[0] == '+') || !plus);	// if first char is +/-, start at second char
	string	modeChanges;

	for (; i < modes.size(); ++i)
	{
		// Try setting each mode by iterating each character
		int	res(user->TrySetMode(plus, modes[i]));
		if (!res)
			modeChanges += modes[i];
		else if (res > 0)
			resp += getResponseFromCode(user, res, (string[]){ modes.substr(i,1) });
	}
	if (!modeChanges.empty())
	{
		// If there is some valid changes, response effective changes to user
		modeChanges = plus
					? "+" + modeChanges
					: "-" + modeChanges;
		resp += user->_prefix + " MODE " + nick + " :" + modeChanges + CMD_DELIM;
		pushToQueue(user->_fd, resp, responseQueue);
	}
}
