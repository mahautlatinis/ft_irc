/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 17:53:31 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:30:29 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../includes/Headers.hpp"

class	Command
{
	public:
		Command(User *user, string const &cmd);
		virtual ~Command(void);

		bool				isValid() const;
		bool				isExecutable() const;

		static void			initCommandList();
		friend class		IRC;

	private:
		User				*_user;
		string				_type;
		std::vector<string>	_params;

		void				deduceType(string &cmd);
		void				buildParams(string const &strParams);
		void				treatTOPIC(string const &rawCmd);
		void				treatMODE();
		void				treatPRIVMSG_NOTICE();
};
