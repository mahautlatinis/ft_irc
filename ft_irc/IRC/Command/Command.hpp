/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:53:31 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/11 18:08:14 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../includes/Headers.hpp"

class	Command
{
	private:
		User				*_user;
		string				_type;
		std::vector<string>	_params;

		void				buildParams(string const &strParams);
		void				deduceType(string &cmd);
		void				treatTOPIC(string const &rawCmd);
		void				treatMODE();
		void				treatPRIVMSG_NOTICE();
		
	public:
		// Functions implemented here were added just to respect the canonical convention
		Command(void): _user(NULL) { return ;};
		Command(Command const &src)
		{
			*this = src;
			return ;
		};
		
		Command(User *user, string const &cmd);
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
