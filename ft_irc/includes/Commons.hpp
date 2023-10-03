#ifndef COMMONS_HPP
#define COMMONS_HPP

#define CMD_DELIM	"\r\n"
#define LEN_DELIM	2

#define IRC_HOST	"ft-irc.42.fr"		// Server's hostname
#define IRC_VER		"ft_irc v1.0"		// IRC server version
#define USR_HOST	"user.ft-irc.42.fr"	// User's default hostname

// Default admin's info for ADMIN query

#define ADMIN_NAME	"Damien Dam"
#define ADMIN_LOC	"Paris FR"
#define ADMIN_EMAIL	"qdam@student.42.fr"

#define MOTD_FILE	"motd"	// Path to message of the day

// Username and password for OPER command

#define OPER_USERNAME	"admin"
#define OPER_PASSWORD	"ftirc"

// Bot-related stuffs

#define BOT_FD			-1
#define BOT_NICKNAME	"MrBot"

using	std::string;
using	std::stringstream;
typedef std::pair<int, string>	t_clientCmd;

// Internal classes

class	Server;
class	Client;
class	IRC;
class	Channel;
class	User;
class	Bot;
class	Command;

// No-scope utility functions

void	StrSplit(std::vector<string> &results, string const &str, string const &delim);
bool	StrMatch(char const *mask, char const *str);

#endif
