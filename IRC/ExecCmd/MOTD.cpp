#include "../IRC.hpp"
#include <fstream>

static std::vector<string>	MOTDLines;
static bool		motdInit(false);

// Initialize message of the day from file
static void	initMOTD()
{
	motdInit = true;
	std::ifstream	motdFile(MOTD_FILE);
	if (!motdFile.is_open())
	{
		char	*cwd(getcwd(NULL, 0));
		std::cout	<< RED << "Cannot read message of the day from "
					<< cwd << '/' << MOTD_FILE << '\n' << NC;
		free(cwd);
		return;
	}

	string		line;
	while (std::getline(motdFile, line))
		MOTDLines.push_back(line);
	motdFile.close();
}

void	IRC::execMOTD(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	string	motd;
	User	*user(cmd._user);

	if (!motdInit)
		initMOTD();
	if (MOTDLines.empty())
		motd = getResponseFromCode(user, ERR_NOMOTD, NULL);
	else
	{
		motd = getResponseFromCode(user, RPL_MOTDSTART, NULL);
		for (std::vector<string>::iterator it(MOTDLines.begin());
			it != MOTDLines.end(); ++it)
		{
			motd += getResponseFromCode(
				user, RPL_MOTD,
				(string[]){ *it }	
			);
		}
		motd += getResponseFromCode(user, RPL_ENDOFMOTD, NULL);
	}
	pushToQueue(user->_fd, motd, responseQueue);
}
