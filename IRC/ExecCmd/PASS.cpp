#include "../IRC.hpp"

void	IRC::execPASS(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User	*user(cmd._user);
	string	resp;

	if (user->_passwordOK)
		resp = getResponseFromCode(user, ERR_ALREADYREGISTRED, (string[]){ user->_nick });
	if (cmd._params.empty())
		resp = getResponseFromCode(user, ERR_NEEDMOREPARAMS, (string[]){ cmd._type });
	else if (cmd._params[0] == _svPassword)
		user->_passwordOK = true;
	if (!resp.empty())
		pushToQueue(user->_fd, resp, responseQueue);
}
