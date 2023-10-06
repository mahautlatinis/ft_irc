/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:39:52 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 21:02:28 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::userMode
	(User *user, string const &nick, string const &modes, std::vector<t_clientCmd> &responseQueue)
{
	string	resp;
	string	modeChanges;

	if (nick != user->_nick)
		resp = getResponseFromCode(user, ERR_USERSDONTMATCH, NULL);
	else if (modes.empty())
		resp = getResponseFromCode(user, RPL_UMODEIS, (string[]){ user->getModes() });

	if (!resp.empty())
	{
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}

	bool	plus(modes[0] != '-');
	size_t	i((plus && modes[0] == '+') || !plus);

	for (; i < modes.size(); ++i)
	{
		int	res(user->trySetMode(plus, modes[i]));
		if (!res)
			modeChanges += modes[i];
		else if (res > 0)
			resp += getResponseFromCode(user, res, (string[]){ modes.substr(i,1) });
	}

	if (!modeChanges.empty())
	{
		modeChanges = plus
					? "+" + modeChanges
					: "-" + modeChanges;
		resp += user->_prefix + " MODE " + nick + " :" + modeChanges + CMD_DELIM;
		pushToQueue(user->_fd, resp, responseQueue);
	}
	return ;
}

void	IRC::chanMode
	(User *user, string const &chanName, string const &modes,
		string const &params, std::vector<t_clientCmd> &responseQueue)
{
	string				resp;
	Channel				*chan(getChannelByName(chanName));
	std::vector<string>	paramsList;
	string				modeChanges;
	std::vector<string>	paramChanges;
	size_t				paramIdx(0);
	string				param, errorName;

	if (!chan)
		resp = getResponseFromCode(user, ERR_NOSUCHCHANNEL, (string[]){ chanName });
	else if (modes.empty())
	{
		string	chanKey;
		if (chan->hasKey())
			chanKey = (chan->hasJoined(user))
					? chan->_key
					: "<key>";
		resp = getResponseFromCode(
			user, RPL_CHANNELMODEIS,
			(string[]){ chanName, chan->getModes(), chanKey }
		);
	}
	else if (!chan->isOperator(user))
		resp = getResponseFromCode(user, ERR_CHANOPRIVSNEEDED, (string[]){ chanName });

	if (!resp.empty())
	{
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}

	::strSplit(paramsList, params, " ");

	bool	plus(modes[0] != '-');
	size_t	i((plus && modes[0] == '+') || !plus);
	
	for (; i < modes.size(); ++i)
	{
		if (Channel::modeNeedsParam(modes[i], errorName) && paramIdx < paramsList.size())
			param = paramsList[paramIdx++];

		if (param.empty() && !errorName.empty())
		{
			resp += getResponseFromCode(
				user, ERR_CUST_CMODEPARAM,
				(string[]){ chanName, modes.substr(i, 1), errorName }
			);
			continue ;
		}

		int	res(chan->trySetMode(this, plus, modes[i], param));

		if (res > 0)
			resp += getResponseFromCode(
				user, res,
				(string[]){ param, modes.substr(i, 1), chanName, errorName }
			);
		else if(!res)
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
		modeChanges = plus
					? "+" + modeChanges
					: "-" + modeChanges;
		string	msgToAll;
		if (paramChanges.empty())
			msgToAll += ":" + modeChanges;
		else
		{
			msgToAll += modeChanges;
			paramChanges.back() = ":" + paramChanges.back();
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
	return ;
}

void	IRC::mode(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User			*user(cmd._user);
	string const	&name(cmd._params[0]);
	string const	&modes = (cmd._params.size() < 2)
						   ? "" : cmd._params[1];
	string const	&params = (cmd._params.size() < 3)
						   ? "" : cmd._params[2];

	if (cmd._params.empty())
	{
		string resp(getResponseFromCode(user, ERR_NEEDMOREPARAMS, (string[]){ cmd._type }));
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}

	if (Channel::isPrefix(name[0]))
		chanMode(user, name, modes, params, responseQueue);
	else
		userMode(user, name, modes, responseQueue);
	return ;
}
