#include "../IRC.hpp"

void	IRC::execDIE(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User	*user(cmd._user);

	if (!user->_oper)
	{
		string	resp(getResponseFromCode(user, ERR_NOPRIVILEGES, NULL));
		pushToQueue(user->_fd, resp, responseQueue);
		return;
	}
	std::cout << RED << user->_nick << " killed the server\n" << NC;
	exit(0);
}
