/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:06:36 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/13 19:32:16 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Bot.hpp>

#define	PYTHON_PORT	33333
#define	BUFFER_SIZE	512

Bot::Bot(void): User(BOT_FD, BOT_NICKNAME)
{
	registrationOK();
	return ;
}

Bot::~Bot(void)
{
	return ;
}

std::string	Bot::cmdHELP(void)
{
	return std::string("CALC(expr): to calculate a simple math expression (only digits and +-*/^ operators). ")
			+ "QUOTE: to get a random quote from the Internet.";
}

std::string	Bot::cmdCALC(std::string const &expr)
{
	static std::string const	validChar("0123456789+-*/^(). ");
	static std::set<char> const	validCharSet(validChar.begin(), validChar.end());

	if (expr.empty())
		return "There is nothing to calculate";

	for (std::string::const_iterator it(expr.begin()); it != expr.end(); ++it)
		if (validCharSet.find(*it) == validCharSet.end())
			return "ERROR: expression contains illegal characters";
	return getResponseFromPython("CALC " + expr);
}

std::string	Bot::cmdQUOTE(void)
{
	return getResponseFromPython("QUOTE");
}

std::string	Bot::getResponseFromPython(std::string const &botRequest)
{
	std::string const	errorMsg("ERROR ");

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
	
	std::string	resp;
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
			&& resp.find(CMD_DELIM, resp.size() - LEN_DELIM) != std::string::npos)
			break;
	}
	close(sv);
	return resp;
}

std::string	Bot::processUserMsg(std::string const &msg)
{
	std::string	cmdType, cmdContent;

	size_t	firstSpace(msg.find(' '));
	if (firstSpace == std::string::npos)
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

std::string	Bot::getWelcomeMsg(const std::string &nick)
{
	return	std::string(
		"Hello " + nick
		+ ", I'm your personnal assistant. "
		+ "Type HELP to see what I can do for you."
	);
}

