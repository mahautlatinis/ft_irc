#include "IRC.hpp"

IRC::IRC(string const &password) :
	_startupTime(time(NULL)),
	_svPassword(password),
	_prefix(string(":") + IRC_HOST),
	_killing(-1)
{
	Command::InitCommandList();
	_bot = new Bot();
	_users[BOT_FD] = _bot;
}

IRC::~IRC()
{
	std::map<int, User *>::iterator	iuit;
	std::map<string, Channel *>::iterator	scit;

	for (iuit = _users.begin(); iuit != _users.end(); ++iuit)
		delete iuit->second;
	for (scit = _channels.begin(); scit != _channels.end(); ++scit)
		delete scit->second;
}

// Called by the server. Process a command from a client, then queue responses
// into responseQueue. Return true if client is disconnecting, otherwise false.
bool	IRC::ProcessClientCommand(t_clientCmd const &command, std::vector<t_clientCmd> &responseQueue)
{
	User	*user;
	int		fd(command.first);
	if (_users.find(fd) == _users.end())
	{
		user = (_users[fd] = new User(fd));
		if (_svPassword.empty())
			user->_passwordOK = true;
		string	msg = getNoticeMsg(
			_prefix, user,
			string("*** Your hostname is set to ") + USR_HOST + " like everybody else."
		);
		pushToQueue(fd, getNoticeMsg(_prefix, user, msg), responseQueue);
	}
	else
		user = _users[fd];

	std::vector<string> subCmds;
	::StrSplit(subCmds, command.second, CMD_DELIM);
	for (std::vector<string>::iterator it(subCmds.begin());
		it != subCmds.end(); ++it)
	{
		Command	cmd(user, *it);
		if (!cmd.IsValid())
			unknownCmd(cmd, responseQueue);
		else if (cmd.IsExecutable())
		{
			bool	reg(user->_registered);
			execCmd(cmd, responseQueue);
			if (cmd._type == "QUIT")
				return true;
			// First executable command should be PASS with correct password
			if (!user->_passwordOK)
				return passwordNotOK(user, responseQueue);
			if (!reg && user->_registered)	// send welcome msg when user has just been registered
			{
				sendWelcomeMessage(user, responseQueue);
				// Bot send welcome PRIVMSG
				Command	botPRIVMSG(
					_bot,
					"PRIVMSG " + user->_nick + " :" + _bot->GetWelcomeMsg(user->_nick)
				);
				execPRIVMSG(botPRIVMSG, responseQueue);
			}
		}
	}
	return false;
}

// Callable by the server to indicate that a client has disconnected
void	IRC::ClientDisconnect(int fd)
{
	if (_users.find(fd) != _users.end())
	{
		User	*user(_users[fd]);
		if (user->_registered)
			removeFromAllChannel(user);
		delete user;
		_users.erase(fd);
	}
}

// Get the fd of user being killed by a server operator
int	IRC::GetVictim()
{
	int	res = _killing;
	_killing = -1;
	return res;
}

// Execute a valid command by user
void	IRC::execCmd(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
		 if (cmd._type == "ADMIN")		execADMIN		(cmd, responseQueue);
	else if (cmd._type == "AWAY")		execAWAY		(cmd, responseQueue);
	else if (cmd._type == "DIE")		execDIE			(cmd, responseQueue);
	else if (cmd._type == "INVITE")		execINVITE		(cmd, responseQueue);
	else if (cmd._type == "JOIN")		execJOIN		(cmd, responseQueue);
	else if (cmd._type == "KICK")		execKICK		(cmd, responseQueue);
	else if (cmd._type == "KILL")		execKILL		(cmd, responseQueue);
	else if (cmd._type == "LIST")		execLIST		(cmd, responseQueue);
	else if (cmd._type == "MODE")		execMODE		(cmd, responseQueue);
	else if (cmd._type == "MOTD")		execMOTD		(cmd, responseQueue);
	else if (cmd._type == "NAMES")		execNAMES		(cmd, responseQueue);
	else if (cmd._type == "NICK")		execNICK		(cmd, responseQueue);
	else if (cmd._type == "NOTICE")		execNOTICE		(cmd, responseQueue);
	else if (cmd._type == "OPER")		execOPER		(cmd, responseQueue);
	else if (cmd._type == "PART")		execPART		(cmd, responseQueue);
	else if (cmd._type == "PASS")		execPASS		(cmd, responseQueue);
	else if (cmd._type == "PING")		execPING		(cmd, responseQueue);
	else if (cmd._type == "PRIVMSG")	execPRIVMSG		(cmd, responseQueue);
	else if (cmd._type == "QUIT")		execQUIT		(cmd, responseQueue);
	else if (cmd._type == "TIME")		execTIME		(cmd, responseQueue);
	else if (cmd._type == "TOPIC")		execTOPIC		(cmd, responseQueue);
	else if (cmd._type == "USER")		execUSER		(cmd, responseQueue);
	else if (cmd._type == "VERSION")	execVERSION		(cmd, responseQueue);
	else if (cmd._type == "WHO")		execWHO			(cmd, responseQueue);
}
