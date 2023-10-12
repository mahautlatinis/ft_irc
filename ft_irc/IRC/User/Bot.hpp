/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:08:20 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 10:40:16 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../includes/Headers.hpp"
#include "User.hpp"

class Bot: public User
{
	private:
		std::string	cmdCALC(std::string const &expr);
		std::string	cmdHELP(void);
		std::string	cmdQUOTE(void);
		std::string	getResponseFromPython(std::string const &botRequest);
		std::string	processUserMsg(std::string const &msg);

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

		std::string	getWelcomeMsg(std::string const &nick);

		friend class IRC;
};