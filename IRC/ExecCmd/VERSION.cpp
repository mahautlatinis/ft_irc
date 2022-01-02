#include "../IRC.hpp"

void	IRC::execVERSION(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User	*user(cmd._user);
	string	resp(getResponseFromCode(user, RPL_VERSION, NULL));
	pushToQueue(user->_fd, resp, responseQueue);
}
