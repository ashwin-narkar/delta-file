#include "Hash.h"
#include <iostream>
#include <list>

HashTable::HashTable()
{
	;
}

int HashTable::getHashIndex(const string& key) //hash function
{
	if (key == "")
		return -1;
	int avg = 2;
	for (size_t i = 0; i < key.size(); i++)
	{
		avg = avg*3 + (key[i]);
		
	}
	while (avg >= size)
	{
		avg /= 5;
	} 
	
	return (avg);
}

bool HashTable::addItem(const string& key,int offset)
{
	int index = getHashIndex(key);
	if (index < 0)
		return false;
	if (findKey(key, index) == "")	//if it doesnt exist
	{
		index = getHashIndex(key);
		stringData s(key, offset);
		bucket[index].push_back(s);
	}
	return true;
}

string HashTable::findKey(const string& key,int& offset)
{
	int i = getHashIndex(key);
	string s = "";
	offset = -1;
	for (list<stringData>::iterator j = bucket[i].begin(); j != bucket[i].end(); j++)
		if (j->str == key)
		{
			s = j->str;
			offset = j->offset;
			//bucket[i].erase(j);
		}
	return s;
}

void HashTable::printTable()
{
	cout << " Key " << " Values " << endl;
	for (int i = 0; i < size; i++)
	{
		cout << i << ".		";
		for (list<stringData>::iterator j = bucket[i].begin(); j != bucket[i].end(); j++)
			cout << j->str << "   ";
		cout << endl;
	}
}