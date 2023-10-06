/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:08:20 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:09:04 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../includes/Headers.hpp"
#include "User.hpp"

class Bot: public User
{
	public:
		Bot(void);
		virtual ~Bot(void);
		string	GetWelcomeMsg(string const &nick);
		friend class IRC;

	private:
		string	processUserMsg(string const &msg);
		string	cmdHELP();
		string	cmdCALC(string const &expr);
		string	cmdQUOTE();
		string	getResponseFromPython(string const &botRequest);
};