/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commons.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:24:30 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:26:25 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define CMD_DELIM	            "\r\n"
#define LEN_DELIM	            2

#define IRC_HOST	            "ft-irc.42.fr"
#define IRC_VER		            "ft_irc v1.0"
#define USR_HOST	            "user.ft-irc.42.fr"

#define ADMIN_NAME	            "Damien Dam"
#define ADMIN_LOC	            "Paris FR"
#define ADMIN_EMAIL             "qdam@student.42.fr"

#define MOTD_FILE	            "motd"

#define OPER_USERNAME	        "admin"
#define OPER_PASSWORD	        "ftirc"

#define BOT_FD			        -1
#define BOT_NICKNAME	        "MrBot"

using	std::string;
using	std::stringstream;
typedef std::pair<int, string>  t_clientCmd;

class	                        Server;
class	                        Client;
class	                        IRC;
class	                        Channel;
class	                        User;
class	                        Bot;
class	                        Command;

void	                        strSplit(std::vector<string> &results,
                                    string const &str,
                                    string const &delim);

bool	                        strMatch(char const *mask,
                                    char const *str);