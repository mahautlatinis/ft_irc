#include "../IRC.hpp"
#include <fstream>

struct	AdminInfo
{
	string	Name;
	string	Location;
	string	Email;

	AdminInfo() :
		Name(ADMIN_NAME),
		Location(ADMIN_LOC),
		Email(ADMIN_EMAIL)
	{}
};

static AdminInfo	admin;

void	IRC::execADMIN(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User	*user(cmd._user);
	string	resp(
		  getResponseFromCode(user, RPL_ADMINME, NULL)
		+ getResponseFromCode(user, RPL_ADMINLOC1, (string[]){ admin.Name })
		+ getResponseFromCode(user, RPL_ADMINLOC2, (string[]){ admin.Location })
		+ getResponseFromCode(user, RPL_ADMINEMAIL, (string[]){ admin.Email })
	);
	pushToQueue(user->_fd, resp, responseQueue);
}
