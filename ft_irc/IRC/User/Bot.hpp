/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:08:20 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/11 18:13:12 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../includes/Headers.hpp"
#include "User.hpp"

class Bot: public User
{
	private:
		string	processUserMsg(string const &msg);
		string	cmdHELP();
		string	cmdCALC(string const &expr);
		string	cmdQUOTE();
		string	getResponseFromPython(string const &botRequest);

	public:
		// Functions implemented here were added just to respect the canonical convention
		Bot(void);
		Bot(Bot const &src)
		{
			*this = src;
			return ;
		};
		virtual ~Bot(void);
		Bot &operator=(Bot const &rhs)
		{
			(void)rhs;
			return *this;
		}

		string	GetWelcomeMsg(string const &nick);

		friend class IRC;
};