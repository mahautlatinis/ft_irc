#include "Command.hpp"

static std::set<string>	_cmdIgnored;
static std::set<string>	_cmdAvail;

void	Command::InitCommandList()
{
	if (!_cmdAvail.empty())
		return;
	const char *ignored[] =
	{
		"CAP",
		"CNOTICE",
		"CPRIVMSG",
		"CONNECT",
		"ENCAP",
		"ERROR",
		"HELP",
		"INFO",
		"ISON",
		"KNOCK",
		"LINKS",
		"LUSERS",
		"NAMESX",
		"PONG",
		"REHASH",
		"RULES",
		"SERVER",
		"SERVICE",
		"SERVLIST",
		"SQUERY",
		"SQUIT",
		"SETNAME",
		"SILENCE",
		"STATS",
		"SUMMON",
		"TRACE",
		"UHNAMES",
		"USERHOST",
		"USERIP",
		"USERS",
		"WALLOPS",
		"WATCH",
		"WHOIS",
		"WHOWAS",
		NULL
	};
	const char *avail[] =
	{
		"ADMIN",
		"AWAY",
		"DIE",
		"INVITE",
		"JOIN",
		"KICK",
		"KILL",
		"LIST",
		"MODE",
		"MOTD",
		"NAMES",
		"NICK",
		"NOTICE",
		"OPER",
		"PART",
		"PASS",
		"PING",
		"PRIVMSG",
		"QUIT",
		"TIME",
		"TOPIC",
		"USER",
		"VERSION",
		"WHO",
		NULL
	};
	for (int i(0); ignored[i]; ++i)	
		_cmdIgnored.insert(ignored[i]);
	for (int i(0); avail[i]; ++i)	
		_cmdAvail.insert(avail[i]);
}

Command::Command(User *user, string const &cmd) :
	_user(user)
{
	string	cmdCopy(cmd);
	deduceType(cmdCopy);
	if (IsExecutable())
	{
		buildParams(cmdCopy);
		if (_type == "TOPIC")
			treatTOPIC(cmd);
		else if (_type == "MODE")
			treatMODE();
		else if (_type == "PRIVMSG" || _type == "NOTICE")
			treatPRIVMSG_NOTICE();
	}
}

Command::~Command()
{}

// Determine the type of command from its string
void	Command::deduceType(string &cmd)
{
	size_t	i(cmd.find(' ', 0));
	if (i == string::npos)
	{
		_type = cmd;
		cmd = "";
	}
	else
	{
		_type = cmd.substr(0, i);
		cmd = cmd.substr(i, cmd.size() - i);
	}
	std::transform(_type.begin(), _type.end(), _type.begin(), ::toupper);
}

// Build the command's parameters from command string
void	Command::buildParams(string const &strParams)
{
	size_t	i(strParams.find(" :", 0));
	if (i == string::npos)
		i = strParams.size();
	
	string beforeLast(strParams.substr(0, i));
	::StrSplit(_params, beforeLast, " ");
	i += 2;
	if (i < strParams.size())
		_params.push_back(strParams.substr(i, strParams.size() - i));
}

// Special treatment for TOPIC
void	Command::treatTOPIC(string const &rawCmd)
{
	if (rawCmd.find(" :", 0) != string::npos)
	{
		if (_params.empty())
			_params.push_back("*");
		else if (_params.size() == 1)
			_params.push_back("");
	}
}

// Special treatment for MODE
void	Command::treatMODE()
{
	if (_params.size() < 3)
		return;
	string res(_params[2]);
	for (size_t i(3); i < _params.size(); ++i)
		res += " " + _params[i];
	_params.erase(_params.begin() + 2, _params.end());
	_params.push_back(res);
}

// Special treatment for PRIVMSG and NOTICE
void	Command::treatPRIVMSG_NOTICE()
{
	if (_params.size() < 2)
		return;
	string res(_params[1]);
	for (size_t i(2); i < _params.size(); ++i)
		res += " " + _params[i];
	_params.erase(_params.begin() + 1, _params.end());
	_params.push_back(res);
}

// Check if command is supported by server (including ignored)
bool	Command::IsValid() const
{
	if (_type.empty())
		return false;
	return (_cmdIgnored.find(_type) != _cmdIgnored.end())
		|| (_cmdAvail.find(_type) != _cmdAvail.end());
}

// Check if command is executed by server
bool	Command::IsExecutable() const
{
	if (_type.empty())
		return false;
	return _cmdAvail.find(_type) != _cmdAvail.end();
}

void	Command::Print() const
{
	std::cout	<< "Type: " << _type << '\n'
				<< "Params:\n";
	for (std::vector<string>::const_iterator it(_params.begin());
		it != _params.end(); ++it)
		std::cout << "- " << *it << '\n';
}
