#include "../IRC.hpp"

void	IRC::execWHO(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	string	resp;
	User	*user(cmd._user);

	if (cmd._params.empty())
	{
		resp = getResponseFromCode(user, ERR_NEEDMOREPARAMS, (string[]){ cmd._type });
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}
	
	string const	&mask(cmd._params[0]);
	bool	o(cmd._params.size() > 1 && cmd._params[1].find('o') != string::npos);
	if (Channel::IsPrefix(mask[0]))
		chanWHO(user, mask, o, responseQueue);
	else
		userWHO(user, mask, o, responseQueue);
	resp = getResponseFromCode(user, RPL_ENDOFWHO, (string[]){ mask });
	pushToQueue(user->_fd, resp, responseQueue);
}

void	IRC::chanWHO(User *user, string const &mask, bool o, std::vector<t_clientCmd> &responseQueue) const
{
	Channel	*chan(getChannelByName(mask));
	if (!chan)
		return;
	
	string	resp;
	User	*u;
	for (std::set<User *>::const_iterator it(chan->_users.begin());
		it != chan->_users.end(); ++it)
	{
		u = (*it);
		if ((!chan->HasJoined(user) && user != u && u->_invisible)
			|| (o && !u->_oper))
			// Do not include non-self invisible users or non-op in 'o' request
			continue;
		string	flag = (u->IsAway()) ? "G" : "H";
		if (u->_oper)
			flag += "*";
		if (chan->IsOperator(u))
			flag += "@";
		resp += getResponseFromCode(
			user, RPL_WHOREPLY,
			(string[]){ mask, u->_uname, u->_nick, flag, u->_rname }
		);
	}
	pushToQueue(user->_fd, resp, responseQueue);
}

void	IRC::userWHO(User *user, string mask, bool o, std::vector<t_clientCmd> &responseQueue) const
{
	// Search in : hostname, server name, nickname, real name.
	
	// Transfrom mask by reducing multiple consecutive stars to only one
	size_t	multiStarIdx;
	while ((multiStarIdx = mask.find("**")) != string::npos)
		mask.replace(multiStarIdx, 2, "*");

	string	resp;
	User	*u;
	for (std::map<int, User *>::const_iterator it(_users.begin());
		it != _users.end(); ++it)
	{
		bool	showUser(false);
		u = it->second;
		if (o && !u->_oper)
			// Do not include non-op in 'o' request
			continue;
		if (u->_nick == mask || u->_rname == mask)
			// Show invisible users if exact match in realname or nickname
			showUser = true;
		else if (u == user || !u->_invisible)
			// Show user if one of the patterns matches (for themselves
			// or another non-invisible user)
			showUser = (::StrMatch(mask.c_str(), USR_HOST)
				|| ::StrMatch(mask.c_str(), IRC_HOST)
				|| ::StrMatch(mask.c_str(), u->_nick.c_str())
				|| ::StrMatch(mask.c_str(), u->_rname.c_str()));
		if (showUser)
		{
			string	flag = (u->IsAway()) ? "G" : "H";
			if (u->_oper)
				flag += "*";
			resp += getResponseFromCode(
				user, RPL_WHOREPLY,
				(string[]){ "*", u->_uname, u->_nick, flag, u->_rname }
			);
		}
	}
	if (!resp.empty())
		pushToQueue(user->_fd, resp, responseQueue);
}
