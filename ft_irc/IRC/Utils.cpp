/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:05:10 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:05:43 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <string>

void	strSplit
	(std::vector<std::string> &results, std::string const &str,
		std::string const &delim)
{
	size_t	i(0);

	while (i < str.size())
	{
		size_t delimIdx(str.find(delim, i));
		if (delimIdx == std::string::npos)
			delimIdx = str.size();
		if ((delimIdx - i) > 0)
			results.push_back(str.substr(i, delimIdx - i));
		i = delimIdx + delim.size();
	}
	return ;
}

bool	strMatch(char const *mask, char const *str)
{
	if (*mask == '\0' && *str == '\0')
		return true;

	if (*mask == '*' && *(mask+1) != '\0' && *str == '\0')
		return false;

	if (*mask == *str)
		return strMatch(mask+1, str+1);

	if (*mask == '*')
		return strMatch(mask+1, str) || strMatch(mask, str+1);

	return false;
}
