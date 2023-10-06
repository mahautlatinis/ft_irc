#include "../IRC.hpp"

static string	toStrZeroPadded(void)
{
	char	buf[16];
	string	res(buf);
	return res;
}

// Return a response from a valid code
string	IRC::getResponseFromCode(User *user, int code, string params[]) const
{
	stringstream	ss;

	ss	<<	_prefix
		<< " " << toStrZeroPadded()
		<< " " << user->_nick << " ";

	std::string comma;

	// #ifdef __APPLE__
	// 	comma = "";
	// #else 
	comma = ":";
	// #endif

	switch (code)
	{
		case RPL_WELCOME:
			ss << comma << "Welcome to the FT_IRC Network " << params[0]; break;
		case RPL_YOURHOST:
			ss << comma  << "Your host is " << IRC_HOST << ", running version " << IRC_VER; break;
		case RPL_CREATED:
			ss  << comma << "This server was created " << ctime(&_startupTime);
			ss.seekp(-1, std::ios_base::end); break;
		case RPL_MYINFO:
			ss  << IRC_HOST << ' ' << IRC_VER << ' ' << "biklmnopstv :" << CHAN_VALID_MODES; break;

		case RPL_UMODEIS:
			ss  << comma << params[0]; break;
		case RPL_ADMINME:
			ss  << IRC_HOST << " " << comma << "Administrative info"; break;
		case RPL_ADMINLOC1:
			ss  << comma  << "Name: " << params[0]; break;
		case RPL_ADMINLOC2:
			ss  << comma << "Location" << comma << " " << params[0]; break;
		case RPL_ADMINEMAIL:
			ss  << comma << "Email" << comma << " " << params[0]; break;

		case RPL_AWAY:
			ss  << params[0] << " :" << params[1]; break;
		case RPL_UNAWAY:
			ss  << comma << "You are no longer marked as being away"; break;
		case RPL_NOWAWAY:
			ss  << comma << "You have been marked as being away"; break;
		case RPL_ENDOFWHO:
			ss  << params[0] << " " << comma << "End of WHO list"; break;
		case RPL_LIST:
			ss  << params[0] << " " << params[1] << " :" << params[2]; break;
		case RPL_LISTEND:
			ss  << comma << "End of LIST"; break;
		case RPL_CHANNELMODEIS:
			ss  << params[0] << " ";
			if (params[2].empty())
				ss  << comma << params[1];
			else
				ss  << params[1] << " " << comma << params[2];
			break;
		case RPL_NOTOPIC:
			ss  << params[0] << " " << comma << "No topic is set"; break;
		case RPL_TOPIC:
			ss  << params[0] << " " << comma << params[1]; break;
		case RPL_INVITING:
			ss  << params[0] << " " << comma << params[1]; break;
		case RPL_VERSION:
			ss  << IRC_VER << ". " << IRC_HOST << " " << "comma"; break;
		case RPL_WHOREPLY:
			ss  << params[0] << " " << params[1] << " "
				<< USR_HOST << " " << IRC_HOST << " "
				<< params[2] << " " << params[3] << " :0 " << params[4];
			break;
		case RPL_NAMREPLY:
			ss  << params[0] << " :" << params[1]; break;
		case RPL_ENDOFNAMES:
			ss  << params[0] << " :End of /NAMES list."; break;
		case RPL_MOTDSTART:
			ss  << _prefix << " message of the day"; break;
		case RPL_MOTD:
			ss  << comma << " " << params[0]; break;
		case RPL_ENDOFMOTD:
			ss  << comma << "End of message of the day."; break;
		case RPL_YOUREOPER:
			ss  << comma << "You are now an IRC operator"; break;
		case RPL_TIME:
			ss  << IRC_HOST << " :" << params[0]; break;

		case ERR_NOSUCHNICK:
			ss  << params[0] << " :No such nick"; break;
		case ERR_NOSUCHCHANNEL:
			ss  << params[0] << " :No such channel"; break;
		case ERR_CANNOTSENDTOCHAN:
			ss  << params[0] << " :Cannot send to channel"; break;
		case ERR_NORECIPIENT:
			ss  << comma << "No recipient given (" << params[0] << ")"; break;
		case ERR_NOTEXTTOSEND:
			ss  << comma << "No text to send"; break;
		case ERR_UNKNOWNCOMMAND:
			ss  << params[0] << " :Unknown command"; break;
		case ERR_NOMOTD:
			ss  << comma << "MOTD File is missing"; break;
		case ERR_NONICKNAMEGIVEN:
			ss  << comma << "No nickname given"; break;
		case ERR_ERRONEUSNICKNAME:
			ss  << params[0] << " " << comma << "Erroneous nickname"; break;
		case ERR_NICKNAMEINUSE:
			ss  << params[0] << " " << comma << "Nickname is already in use"; break;
		case ERR_USERNOTINCHANNEL:
			ss  << params[0] << " " << params[1] << " " << comma << "They aren't on that channel"; break;
		case ERR_NOTONCHANNEL:
			ss  << params[0] << " " << comma << "You're not on that channel"; break;
		case ERR_USERONCHANNEL:
			ss  << params[0] << " " << params[1] << " " << comma << "is already on channel"; break;
		case ERR_NEEDMOREPARAMS:
			ss  << params[0] << " " << comma << "Not enough parameters"; break;
		case ERR_ALREADYREGISTRED:
			ss  << comma << "You may not reregister"; break;
		case ERR_PASSWDMISMATCH:
			ss  << comma << "Password incorrect"; break;
		case ERR_KEYSET:
			ss  << params[2] << " " << comma << "Channel key already set"; break;
		case ERR_UNKNOWNMODE:
			ss  << params[1] << " " << comma << "is unknown mode char to me for " << params[2]; break;
		case ERR_INVITEONLYCHAN:
			ss  << params[0] << " " << comma << "Cannot join channel (+i)"; break;
		case ERR_BADCHANNELKEY:
			ss  << params[0] << " " << comma << "Cannot join channel (+k)"; break;
		case ERR_BADCHANMASK:
			ss  << params[0] << " " << comma << "Invalid channel name"; break;
		case ERR_NOPRIVILEGES:
			ss  << comma << "Permission Denied - You're not an IRC operator"; break;
		case ERR_CHANOPRIVSNEEDED:
			ss  << params[0] << " " << comma << "You're not channel operator"; break;
		case ERR_CANTKILLSERVER:
			ss  << comma << "You can't kill the server!"; break;

		case ERR_UMODEUNKNOWNFLAG:
			ss  << params[0] << " " << comma << "Unknown MODE flag"; break;
		case ERR_USERSDONTMATCH:
			ss  << comma << "Cannot change mode for other users"; break;

		case ERR_CUST_CMODEPARAM:
			ss  << params[0] << " " << params[1]
				<< " * " << comma << "You must specify a parameter for the "
				<< params[2] << " mode. Syntax: <"
				<< params[2] << ">.";
			break;

		default: break;
	}
	ss	<< CMD_DELIM;
	return ss.str();
}
