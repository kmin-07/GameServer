#include "RandomName.h"
#include <fstream>
#include <vector>

void RandomName::LoadFile()
{
	ifstream first("random_fist.txt");
	ifstream last("random_last.txt");

	string last_name;
	vector<string>tmp;
	while (getline(last,last_name))
	{
		tmp.push_back(last_name);
	}



}
