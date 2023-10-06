#include <string>
#include <vector>
#include <unistd.h>

using namespace std;

int main(void)
{
	vector<pair<int, string>>	vec;

	while (true)
	{
		vec.clear();
		for (int i = 0; i < 10; ++i)
			vec.push_back(make_pair(i, "Some string"));
		usleep(100);
	}
	return (0);
}
