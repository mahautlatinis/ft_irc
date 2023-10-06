/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADMIN.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:26:38 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 18:26:39 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"
#include <fstream>

class	AdminInfo
{
	public: 
		string	name;
		string	location;
		string	email;

		AdminInfo(): name(ADMIN_NAME), location(ADMIN_LOC), email(ADMIN_EMAIL)
		{
			return ;
		}

		virtual ~AdminInfo()
		{
			return ;
		}
};

static AdminInfo	administrator;

void	IRC::admin(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User	*user(cmd._user);

	string	resp(
		  getResponseFromCode(user, RPL_ADMINME, NULL)
		+ getResponseFromCode(user, RPL_ADMINLOC1, (string[])
			{
				administrator.name
			})
		+ getResponseFromCode(user, RPL_ADMINLOC2, (string[])
			{
				administrator.location 
			})
		+ getResponseFromCode(user, RPL_ADMINEMAIL, (string[])
			{ 
				administrator.email 
			})
	);
	pushToQueue(user->_fd, resp, responseQueue);
	return ; 
}
