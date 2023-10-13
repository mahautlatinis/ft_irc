/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commons.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:24:30 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/12 10:14:52 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define ADMIN_EMAIL             "qdam@student.42.fr"
#define ADMIN_LOC	            "Paris FR"
#define ADMIN_NAME	            "Damien Dam"
#define BOT_FD			        -1
#define BOT_NICKNAME	        "MrBot"
#define CMD_DELIM	            "\r\n"
#define IRC_HOST	            "ft-irc.42.fr"
#define IRC_VER		            "ft_irc v1.0"
#define LEN_DELIM	            2
#define MOTD_FILE	            "motd"
#define OPER_PASSWORD	        "ftirc"
#define OPER_USERNAME	        "admin"
#define USR_HOST	            "user.ft-irc.42.fr"

#include <string>

typedef std::pair<int, std::string> t_clientCmd;

class	                            Bot;
class	                            Channel;
class	                            Client;
class	                            Command;
class	                            IRC;
class	                            Server;
class	                            User;

void	                            strSplit(std::vector<std::string> &results,
                                        std::string const &str,
                                        std::string const &delim);

bool	                            strMatch(char const *mask,
                                        char const *str);