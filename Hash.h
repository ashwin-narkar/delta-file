#ifndef HASH_H
#define HASH_H
#include <string>
#include <list>
using namespace std;
class HashTable
{
private:
	static const int size = 10000;	//only 128 possible characters
	struct stringData
	{
		stringData(string s, int i)
		{
			str = s;
			offset = i;
		}
		string str;
		int offset;
	};
	list<stringData> bucket[size];

public:
	HashTable();
	int getHashIndex(const string& key);
	string findKey(const string& key, int& offset);
	bool addItem(const string& key,int offset);
	void printTable();
};
#endif // !HASH_H
