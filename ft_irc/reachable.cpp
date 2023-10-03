/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reachable.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:30:39 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/03 18:30:45 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <unistd.h>

using namespace std;

int main()
{
	vector<pair<int, string> >	vec;

	while (true)
	{
		vec.clear();
		for (int i = 0; i < 10; ++i)
			vec.push_back(make_pair(i, "Some string"));
		usleep(100);
	}
}
