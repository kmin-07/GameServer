#pragma once
#include <list>
#include <string>
using namespace std;

class FirstNmae
{
public:
	string m_first;
	list<string>m_last_list;
};
class RandomName
{
	list<FirstNmae*>m_pool;
public:
	void LoadFile();

};

