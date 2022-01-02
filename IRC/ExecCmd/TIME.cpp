#include "../IRC.hpp"

void	IRC::execTIME(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	time_t	now(time(NULL));
	string	timeStr(ctime(&now));
	timeStr.erase(timeStr.size() - 1, 1);

	User	*user(cmd._user);
	string	resp(getResponseFromCode(user, RPL_TIME, (string[]){ timeStr }));
	pushToQueue(user->_fd, resp, responseQueue);
}
