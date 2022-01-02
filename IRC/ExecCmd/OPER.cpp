#include "../IRC.hpp"

void	IRC::execOPER(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User	*user(cmd._user);
	string	resp;

	if (cmd._params.size() < 2)
		resp = getResponseFromCode(user, ERR_NEEDMOREPARAMS, (string[]){ cmd._type });
	else if (cmd._params[0] != OPER_USERNAME || cmd._params[1] != OPER_PASSWORD)
		resp = getResponseFromCode(user, ERR_PASSWDMISMATCH, NULL);
	else
	{
		user->_oper = true;
		resp = getResponseFromCode(user, RPL_YOUREOPER, NULL);
	}
	pushToQueue(user->_fd, resp, responseQueue);
}
