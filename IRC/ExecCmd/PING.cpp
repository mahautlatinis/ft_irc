#include "../IRC.hpp"

void	IRC::execPING(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	stringstream	ss;
	string	param	= cmd._params.empty()
						? ""
						: cmd._params[0];
	ss << _prefix << " PONG " << IRC_HOST << " :" << param << CMD_DELIM;
	pushToQueue(cmd._user->_fd, ss.str(), responseQueue);
}
