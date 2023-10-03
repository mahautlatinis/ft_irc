/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:29:16 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/03 18:29:17 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include "../../includes/Headers.hpp"
#include "User.hpp"

class	Bot : public User
{

private:
	string	processUserMsg(string const &msg);
	string	cmdHELP();
	string	cmdCALC(string const &expr);
	string	cmdQUOTE();

	string	getResponseFromPython(string const &botRequest);

public:
	Bot();
	~Bot();

	string	GetWelcomeMsg(string const &nick);

	friend class IRC;
};

#endif
