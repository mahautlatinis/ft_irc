#ifndef IRC_HPP
#define IRC_HPP

#include "../includes/Headers.hpp"
#include "User/User.hpp"
#include "User/Bot.hpp"
#include "Channel/Channel.hpp"
#include "Command/Command.hpp"

class	IRC
{

private:
	time_t const	_startupTime;			// Start-up time
	string const	_svPassword;			// Server's password
	string const	_prefix;				// Server's prefix in responses
	std::map<int, User *>		_users;		// Users list
	std::map<string, Channel *>	_channels;	// Channels list
	int	_killing;							// FD of user being killed by an operator
	Bot	*_bot;								// Server's bot

	//// Manage users ////

	void	userLeaveChannel(User *user, Channel *chan);
	void	removeFromAllChannel(User *user);
	void	sendWelcomeMessage(User *user, std::vector<t_clientCmd> &responseQueue);

	//// Manage channels ////

	Channel	*newChannel(string const &name, User *creator);
	std::set<User *>	getCommonUsers(User *user) const;

	//// Manage commands ////

	void	unknownCmd(Command const &cmd, std::vector<t_clientCmd> &responseQueue) const;
	bool	passwordNotOK(User *user, std::vector<t_clientCmd> &responseQueue);
	string	getNoticeMsg(string const &senderPrefix, User *user, string const &msg) const;
	string	getErrorResponse(User *user, string const &msg) const;
	string	getResponseFromCode(User *user, int code, string params[]) const;
	string	appendUserNotif(User *user, string params[], std::set<User *> const &dest, std::vector<t_clientCmd> &responseQueue, bool excludeUser=false) const;
	void	pushToQueue(int fd, string const &msg, std::vector<t_clientCmd> &responseQueue) const;

	//// Command executor ////

	void	execCmd			(Command const &cmd, std::vector<t_clientCmd> &responseQueue);

	void	execADMIN		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execAWAY		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execDIE			(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execINVITE		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execJOIN		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execKICK		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execKILL		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execLIST		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execMODE		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execMOTD		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execNAMES		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execNICK		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execNOTICE		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execOPER		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execPART		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execPASS		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execPING		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execPRIVMSG		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execQUIT		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execTIME		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execTOPIC		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execUSER		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execVERSION		(Command const &cmd, std::vector<t_clientCmd> &responseQueue);
	void	execWHO			(Command const &cmd, std::vector<t_clientCmd> &responseQueue);

	// Methods specific to some commands

	void	chanPRIVMSG(User *user, string const &name, string const &msg, std::vector<t_clientCmd> &responseQueue);
	void	userPRIVMSG(User *user, string const &name, string const &msg, std::vector<t_clientCmd> &responseQueue);

	void	chanMODE(User *user, string const &chanName, string const &modes, string const &params, std::vector<t_clientCmd> &responseQueue);
	void	userMODE(User *user, string const &nick, string const &modes, std::vector<t_clientCmd> &responseQueue);

	string	kickTarget(User *user, string const &nick, Channel *chan, string const &comment, std::vector<t_clientCmd> &responseQueue);

	void	chanWHO(User *user, string const &mask, bool o, std::vector<t_clientCmd> &responseQueue) const;
	void	userWHO(User *user, string mask, bool o, std::vector<t_clientCmd> &responseQueue) const;

public:
	IRC(string const &password);
	virtual ~IRC();

	User	*getUserByNick(string const &nick) const;
	Channel	*getChannelByName(string const &name) const;

	bool	ProcessClientCommand(t_clientCmd const &command, std::vector<t_clientCmd> &responseQueue);
	void	ClientDisconnect(int fd);
	int		GetVictim();

};

#endif
