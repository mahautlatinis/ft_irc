#include "Bot.hpp"

#define	PYTHON_PORT	33333
#define	BUFFER_SIZE	512

Bot::Bot() : User(BOT_FD, BOT_NICKNAME)
{
	registrationOK();
}

Bot::~Bot()
{}

string	Bot::cmdHELP()
{
	return string("'00,01CALC01,00 (expr)01' Calculate a simple math expression (only digits and +-*/^ operators). ")
			+ "'00,01QUOTE01' Get a random quote from the Internet.";
}

string	Bot::cmdCALC(string const &expr)
{
	static string const	validChar("0123456789+-*/^(). ");
	static std::set<char> const	validCharSet(validChar.begin(), validChar.end());

	if (expr.empty())
		return "There is nothing to calculate";

	for (string::const_iterator it(expr.begin()); it != expr.end(); ++it)
		if (validCharSet.find(*it) == validCharSet.end())
			return "04ERROR Expression contains illegal characters";
	return getResponseFromPython("CALC " + expr);
}

string	Bot::cmdQUOTE()
{ return getResponseFromPython("QUOTE"); }

string	Bot::getResponseFromPython(string const &botRequest)
{
	string const	errorMsg("04ERROR ");

	// Setup client socket & error checking
	int	sv(socket(AF_INET, SOCK_STREAM, 0));
	if (sv < 0)
		return errorMsg + "Socket cannot be created";
	sockaddr_in	svAddr;
	svAddr.sin_family = AF_INET;
	svAddr.sin_port = htons(PYTHON_PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &svAddr.sin_addr) <= 0)
	{
		close(sv);
		return errorMsg + "Invalid bot IP address";
	}
	if (connect(sv, (sockaddr *)&svAddr, sizeof(svAddr)) < 0)
	{
		close(sv);
		return errorMsg + "Bot offline";
	}

	send(sv, (botRequest + CMD_DELIM).c_str(), botRequest.size() + LEN_DELIM, 0);
	
	string	resp;
	char	buffer[BUFFER_SIZE + 1];
	ssize_t	bytesRead(0);
	while (true)
	{
		bytesRead = recv(sv, buffer, BUFFER_SIZE, 0);
		if (bytesRead <= 0)
		{
			close(sv);
			return errorMsg + "Bot closed socket";
		}
		buffer[bytesRead] = '\0';
		resp += buffer;
		if (resp.size() >= LEN_DELIM
			&& resp.find(CMD_DELIM, resp.size() - LEN_DELIM) != string::npos)
			break;
	}
	if (resp.find("ERROR ") == 0)
		resp = "04" + resp;
	close(sv);
	return resp;
}

string	Bot::processUserMsg(string const &msg)
{
	string	cmdType, cmdContent;

	// Pre-process command 
	size_t	firstSpace(msg.find(' '));
	if (firstSpace == string::npos)
		cmdType = msg;
	else
	{
		cmdType = msg.substr(0, firstSpace);
		cmdContent = msg.substr(firstSpace + 1);
	}
	std::transform(cmdType.begin(), cmdType.end(), cmdType.begin(), ::toupper);

	if (cmdType == "HELP")
		return cmdHELP();
	else if (cmdType == "CALC")
		return cmdCALC(cmdContent);
	else if (cmdType == "QUOTE")
		return cmdQUOTE();
	else
		return "Sorry man, I don't really know what you want";
}

string	Bot::GetWelcomeMsg(const string &nick)
{
	return	string(
		"Hello " + nick
		+ ", I'm your personnal assistant. "
		+ "Type '00,01HELP01' to see what I can do for you."
	);
}

