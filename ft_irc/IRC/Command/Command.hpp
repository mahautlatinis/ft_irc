/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:53:31 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 10:41:12 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../includes/Headers.hpp"

class	Command
{
	private:
		User				*_user;
		std::string			_type;
		std::vector<std::string>	_params;

		void				buildParams(std::string const &strParams);
		void				deduceType(std::string &cmd);
		void				treatTOPIC(std::string const &rawCmd);
		void				treatMODE(void);
		void				treatPRIVMSG_NOTICE(void);
		
	public:
		// Functions implemented here were added just to respect the canonical convention
		Command(void): _user(NULL) { return ;};
		Command(Command const &src)
		{
			*this = src;
			return ;
		};
		
		Command(User *user, std::string const &cmd);
		virtual ~Command(void);
		Command &operator=(Command const &rhs)
		{
			(void)rhs;
			return (*this); 
		};

		bool				isValid() const;
		bool				isExecutable() const;

		static void			initCommandList();
		
		friend class		IRC;
};
