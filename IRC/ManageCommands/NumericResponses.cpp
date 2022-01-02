#include "../IRC.hpp"

// Return a string rep of n zero-padded whose final length is width
static string	toStrZeroPadded(unsigned int n, int width)
{
	char	buf[16];
	sprintf(buf, "%0*u", width, n);
	string	res(buf);
	return res;
}

// Return a response from a valid code
string	IRC::getResponseFromCode(User *user, int code, string params[]) const
{
	stringstream	ss;
	ss	<<	_prefix
		<< " " << toStrZeroPadded(code, 3)
		<< " " << user->_nick << " ";

	switch (code)
	{
		case RPL_WELCOME:
			ss  << ":Welcome to the FT_IRC Network " << params[0]; break;
		case RPL_YOURHOST:
			ss  << ":Your host is " << IRC_HOST << ", running version " << IRC_VER; break;
		case RPL_CREATED:
			ss  << ":This server was created " << ctime(&_startupTime);
			ss.seekp(-1, std::ios_base::end); break;
		case RPL_MYINFO:
			ss  << IRC_HOST << ' ' << IRC_VER << ' ' << "biklmnopstv :" << CHAN_VALID_MODES; break;
		
		case RPL_UMODEIS:
			ss  << ":" << params[0]; break;
		case RPL_ADMINME:
			ss  << IRC_HOST << " :Administrative info"; break;
		case RPL_ADMINLOC1:
			ss  << ":Name: " << params[0]; break;
		case RPL_ADMINLOC2:
			ss  << ":Location: " << params[0]; break;
		case RPL_ADMINEMAIL:
			ss  << ":Email: " << params[0]; break;
		
		case RPL_AWAY:
			ss  << params[0] << " :" << params[1]; break;
		case RPL_UNAWAY:
			ss  << ":You are no longer marked as being away"; break;
		case RPL_NOWAWAY:
			ss  << ":You have been marked as being away"; break;
		case RPL_ENDOFWHO:
			ss  << params[0] << " :End of WHO list"; break;
		case RPL_LIST:
			ss  << params[0] << " " << params[1] << " :" << params[2]; break;
		case RPL_LISTEND:
			ss  << ":End of LIST"; break;
		case RPL_CHANNELMODEIS:
			ss  << params[0] << " ";
			if (params[2].empty())
				ss  << ":" << params[1];
			else
				ss  << params[1] << " :" << params[2];
			break;
		case RPL_NOTOPIC:
			ss  << params[0] << " :No topic is set"; break;
		case RPL_TOPIC:
			ss  << params[0] << " :" << params[1]; break;
		case RPL_INVITING:
			ss  << params[0] << " :" << params[1]; break;
		case RPL_VERSION:
			ss  << IRC_VER << ". " << IRC_HOST << " :"; break;
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
			ss  << ": " << params[0]; break;
		case RPL_ENDOFMOTD:
			ss  << ":End of message of the day."; break;
		case RPL_YOUREOPER:
			ss  << ":You are now an IRC operator"; break;
		case RPL_TIME:
			ss  << IRC_HOST << " :" << params[0]; break;
		
		case ERR_NOSUCHNICK:
			ss  << params[0] << " :No such nick"; break;
		case ERR_NOSUCHCHANNEL:
			ss  << params[0] << " :No such channel"; break;
		case ERR_CANNOTSENDTOCHAN:
			ss  << params[0] << " :Cannot send to channel"; break;
		case ERR_NORECIPIENT:
			ss  << ":No recipient given (" << params[0] << ")"; break;
		case ERR_NOTEXTTOSEND:
			ss  << ":No text to send"; break;
		case ERR_UNKNOWNCOMMAND:
			ss  << params[0] << " :Unknown command"; break;
		case ERR_NOMOTD:
			ss  << ":MOTD File is missing"; break;
		case ERR_NONICKNAMEGIVEN:
			ss  << ":No nickname given"; break;
		case ERR_ERRONEUSNICKNAME:
			ss  << params[0] << " :Erroneous nickname"; break;
		case ERR_NICKNAMEINUSE:
			ss  << params[0] << " :Nickname is already in use"; break;
		case ERR_USERNOTINCHANNEL:
			ss  << params[0] << " " << params[1] << " :They aren't on that channel"; break;
		case ERR_NOTONCHANNEL:
			ss  << params[0] << " :You're not on that channel"; break;
		case ERR_USERONCHANNEL:
			ss  << params[0] << " " << params[1] << " :is already on channel"; break;
		case ERR_NEEDMOREPARAMS:
			ss  << params[0] << " :Not enough parameters"; break;
		case ERR_ALREADYREGISTRED:
			ss  << ":You may not reregister"; break;
		case ERR_PASSWDMISMATCH:
			ss  << ":Password incorrect"; break;
		case ERR_KEYSET:
			ss  << params[2] << " :Channel key already set"; break;
		case ERR_UNKNOWNMODE:
			ss  << params[1] << " :is unknown mode char to me for " << params[2]; break;
		case ERR_INVITEONLYCHAN:
			ss  << params[0] << " :Cannot join channel (+i)"; break;
		case ERR_BADCHANNELKEY:
			ss  << params[0] << " :Cannot join channel (+k)"; break;
		case ERR_BADCHANMASK:
			ss  << params[0] << " :Invalid channel name"; break;
		case ERR_NOPRIVILEGES:
			ss  << ":Permission Denied - You're not an IRC operator"; break;
		case ERR_CHANOPRIVSNEEDED:
			ss  << params[0] << " :You're not channel operator"; break;
		case ERR_CANTKILLSERVER:
			ss  << ":You can't kill the server!"; break;

		case ERR_UMODEUNKNOWNFLAG:
			ss  << params[0] << " :Unknown MODE flag"; break;
		case ERR_USERSDONTMATCH:
			ss  << ":Cannot change mode for other users"; break;

		case ERR_CUST_CMODEPARAM:
			ss  << params[0] << " " << params[1]
				<< " * :You must specify a parameter for the "
				<< params[2] << " mode. Syntax: <"
				<< params[2] << ">.";
			break;

		default: break;
	}
	ss	<< CMD_DELIM;
	return ss.str();
}
