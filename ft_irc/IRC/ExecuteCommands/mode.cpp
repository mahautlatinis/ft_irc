/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:39:52 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 09:38:57 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::userMode
	(User *user, 
		std::string const &nick, 
		std::string const &modes, 
		std::vector<t_clientCmd> &responseQueue)
{
	std::string	resp;
	std::string	modeChanges;

	if (nick != user->_nick)
		resp = getResponseFromCode(user, ERR_USERSDONTMATCH, NULL);
	else if (modes.empty())
		resp = getResponseFromCode(user, 
			RPL_UMODEIS, (std::string[]){ user->getModes() });

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
			resp += getResponseFromCode(user, res, 
				(std::string[]){ modes.substr(i,1) });
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
	(User *user, std::string const &chanName, std::string const &modes,
		std::string const &params, std::vector<t_clientCmd> &responseQueue)
{
	Channel						*chan(getChannelByName(chanName));
	size_t						paramIdx(0);
	std::string					modeChanges;
	std::string					param, errorName;
	std::string					resp;
	std::vector<std::string>	paramChanges;
	std::vector<std::string>	paramsList;

	if (!chan)
		resp = getResponseFromCode(user, ERR_NOSUCHCHANNEL,
			(std::string[]){ chanName });
	else if (modes.empty())
	{
		std::string	chanKey;
		if (chan->hasKey())
			chanKey = (chan->hasJoined(user))
					? chan->_key
					: "<key>";
		resp = getResponseFromCode(
			user, RPL_CHANNELMODEIS,
			(std::string[]){ chanName, chan->getModes(), chanKey }
		);
	}
	else if (!chan->isOperator(user))
		resp = getResponseFromCode(user,
			ERR_CHANOPRIVSNEEDED, (std::string[]){ chanName });

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
				(std::string[]){ chanName, modes.substr(i, 1), errorName }
			);
			continue ;
		}

		int	res(chan->trySetMode(this, plus, modes[i], param));

		if (res > 0)
			resp += getResponseFromCode(
				user, res,
				(std::string[]){ param, modes.substr(i, 1), chanName, errorName }
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
		std::string	msgToAll;
		if (paramChanges.empty())
			msgToAll += ":" + modeChanges;
		else
		{
			msgToAll += modeChanges;
			paramChanges.back() = ":" + paramChanges.back();
			for (std::vector<std::string>::iterator it(paramChanges.begin());
				it != paramChanges.end(); ++it)
				msgToAll += " " + (*it);
		}
		appendUserNotif(
			user,
			(std::string[]){ "MODE", chanName, msgToAll, "" },
			chan->_users, responseQueue
		);
	}
	return ;
}

void	IRC::mode(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User				*user(cmd._user);
	std::string const	&name(cmd._params[0]);
	std::string const	&modes = (cmd._params.size() < 2)
						   ? "" : cmd._params[1];
	std::string const	&params = (cmd._params.size() < 3)
						   ? "" : cmd._params[2];

	if (cmd._params.empty())
	{
		std::string resp(getResponseFromCode(user, 
			ERR_NEEDMOREPARAMS, (std::string[]){ cmd._type }));
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}

	if (Channel::isPrefix(name[0]))
		chanMode(user, name, modes, params, responseQueue);
	else
		userMode(user, name, modes, responseQueue);
	return ;
}
