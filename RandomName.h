#pragma once
#include <list>
#include <string>
#include <vector>
using namespace std;

class FirstName
{
public:
	string m_first;
	vector<string>m_last_list;
};
class RandomName
{
	vector<FirstName *>m_pool;
public:
	RandomName();
	string GetName();
	void Release(string _name);
	void LoadFile();
	virtual~RandomName();

};

