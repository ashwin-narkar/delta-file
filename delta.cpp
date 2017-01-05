#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include "Hash.h"
#include <chrono>

using namespace std;

bool applyDelta(istream& oldf, istream& deltaf, ostream& newf);
void createDelta(istream& oldf, istream& newf, ostream& deltaf);
int convertToInt(string x, size_t& index);

bool applyDelta(istream& oldf, istream& deltaf, ostream& newf)
{
	//if any file cannot be opened return false
	if (!oldf)
		return false;
	if (!deltaf)
		return false;
	if (!newf)
		return false;
	string del;	
	int offset = 0;
	string toCopy;
	string oldFile;
	string copyBuffer;
	string delBuffer;
	size_t i = 0;
	getline(deltaf, del);
	//get the contents of the delta file as one big string
	while (getline(deltaf, delBuffer))
	{
		del += '\n';	//add newlines
		del += delBuffer;		
	} 
	//get the contents of the old file as one big string
	getline(oldf, oldFile);
	while (getline(oldf, delBuffer))
	{
		oldFile += '\n' + delBuffer;
	}
	while (i<del.size())	//go through the delta file string
	{
		
		if (del[i] != 'A' && del[i] != 'C')	//if you are not checking an add or copy instruction, the delta file is not made correctly
			return false;
		if (del[i] == 'A')		//add instrucction
		{
			int numToEval = 0;
			if ((i + 1) < del.size() && isdigit(del[i + 1]))	//get the number of chars you need to add
			{
				i++;
				numToEval= convertToInt(del, i);
				i+=2;
				
				for (int j = 0; j < numToEval; j++)
				{
					newf << del[i];		//add this chars to the delta file
					i++;	//increment i as you go
				}
			}
			
		}
		if (del[i] == 'C')	//in the case of a copy instruction
		{
			int numToEval = 0;
			if ((i + 1) < del.size() && isdigit(del[i + 1]))
			{
				i++;
			}
			numToEval = convertToInt(del, i);	//get the number of chars you want to copy
			if (del[i + 1] != ',')	//if the char after the number of chars is not a comma, the delta file is incorrecct
				return false;
			i+=2;	//go past the comma
			if (isdigit(del[i]))
			{
				offset = convertToInt(del, i);	//get the offset number from the string
				i++;
			}
			else
				return false;	//if there is not a number something is wrong
			for (int j = 0; j < numToEval; j++)
			{				
				newf << oldFile[j + offset];	//copy the numToEval characters from the old files offset to the newfile
			}
		}		
		offset = 0;	//reset the offset
	} 
	return true;	//if the function gets to here, the delta file was correct so return true.
}

void createDelta(istream& oldf, istream& newf, ostream& deltaf)
{
	//if any file cannot be opened, exit the funciton
	if (!oldf)
		return;
	if (!deltaf)
		return;
	if (!newf)
		return;
	int n =4;		//this will be the ncharacter sequence we check
	HashTable charSeqs;
	string oldFile, newFile;
	string buffer;
	getline(oldf, oldFile);
	while (getline(oldf, buffer))	//get the entire contents of the file as one big string
	{
		oldFile += '\n' + buffer;	//add the newlines into the string
	}
	
	getline(newf, newFile);
	while (getline(newf, buffer))
	{
		newFile += '\n' + buffer;
	}
	if (newFile.size() > 10000)	//if the file size is more than 10 kb, use n=12 since it is more efficient
	{
		n = 12;
	}
	const int nSize = n;
	string toAdd;
	//add nchar sequences to the hash table charSeqs
	for (size_t i = 0; i < oldFile.size(); i++)
	{
		toAdd = "";
		for (size_t j = 0; j < nSize && (i+j) < oldFile.size(); j++)
		{
			toAdd += oldFile[i + j];
		}
		charSeqs.addItem(toAdd,i);	//this code uses the hash function
	}
	
	toAdd = "";
	int offset;
	for (size_t i = 0; i < newFile.size() - nSize; i++)
	{		
		
		string seqToCheck;
		for (int j = 0; j < nSize; j++)
		{
			seqToCheck += newFile[i + j];
		}
		
		if (charSeqs.findKey(seqToCheck, offset) != "")		//if a match is found
		{
			int j = 0;
			if (toAdd != "")		//if there are things to be added
			{
				deltaf << "A" << toAdd.size() << ":" << toAdd;	//add them
				toAdd = "";	//reset what needs to be added
			}
			while ( (offset + j + nSize < oldFile.size()) && (i + nSize < newFile.size()) &&   oldFile[offset + j + nSize] == newFile[i + nSize])
			{
				seqToCheck += newFile[i+nSize];		//keep checking the next matches until something doesnt match
				i++;
				j++;
			}
			deltaf << "C" << seqToCheck.size() << "," << offset;	//the copy instruction
			i += nSize-1;	//move i plus the nsequence
		}
		else
			toAdd += newFile[i];	//if a match is not found, then these characters need to be added to new file
		if (i >= newFile.size() - nSize - 1)	//add the remaining characters that are not checked
		{
			i++;
			for (; i < newFile.size(); i++)
					toAdd += newFile[i];
		}
		
	}
	if (toAdd != "")	//if there are still characters remaining
	{
		deltaf << "A" << toAdd.size() << ":" << toAdd;	//add them
		
	}
	
}

int convertToInt(string x, size_t& index)	//pass the index by ref so after the funciton call, the index is at the position of the last digit
{
	size_t i = index;
	if (index > x.size())
		return -1;
	string number;
	while (i < x.size() && isdigit(x[index]))	//get the digits of the number in order
	{
		number += x[index];
		index++;
	}
	index--;
	int num = x[index]-'0';	//last digit is just ones place	
	int placeValue = 10;
	for (int j = index - 1; j >= i; j--)	//go backwards. the first digit is accounted for. do the rest
	{
		num += placeValue*(x[j] - '0');	//each digit is multiplied by its placevalue and added
		placeValue *= 10;	//place value is incremented by a power of 10 each time
	}
	return num;	
}

#include <iostream>
#include <sstream>  // for istringstream and ostringstream
#include <string>
#include <cassert>
using namespace std;

void runtest(string oldtext, string newtext)
{
	istringstream oldfile(oldtext);
	istringstream newfile(newtext);
	ostringstream deltafile;
	createDelta(oldfile, newfile, deltafile);
	string result = deltafile.str();
	cout << "The delta length is " << result.size()
		<< " and its text is " << endl;
	cout << result << endl;
	oldfile.clear();   // clear the end of file condition
	oldfile.seekg(0);  // reset back to beginning of the stream
	istringstream deltafile2(result);
	ostringstream newfile2;
	assert(applyDelta(oldfile, deltafile2, newfile2));
	string finalstr = newfile2.str();
	cout << finalstr << endl;
	assert(newtext == newfile2.str());
}
/*
int main()
{
	runtest("There's a bathroom on the right.", "There's a bad moon on the rise.");
	runtest("ABCDEFGHIJBLAHPQRSTUVPQRSTUV", "XYABCDEFGHIJBLETCHPQRSTUVPQRSTQQELF");
	cout << "All tests passed" << endl;

}
*/

int main()
{
	
	ifstream infile("C:/Users/Ashwin/Desktop/res/strange1.txt");  
	ifstream newfile("C:/Users/Ashwin/Desktop/res/strange2.txt");
	fstream deltafile("C:/Users/Ashwin/Desktop/res/test.txt");
	
	string s;
	//getline(delFile, s);
	//cout << s << endl;
	infile.clear();   // clear the end of file condition
	infile.seekg(0);
	deltafile.clear();   // clear the end of file condition
	deltafile.seekg(0);
	newfile.clear();   // clear the end of file condition
	newfile.seekg(0);
	
	createDelta(infile, newfile, deltafile);
	
	infile.clear();   // clear the end of file condition
	infile.seekg(0);

	deltafile.clear();   // clear the end of file condition
	deltafile.seekg(0);
	//cin.ignore(1000000, '\n');

	ofstream newfile2("C:/Users/Ashwin/Desktop/res/strange3.txt");

	applyDelta(infile, deltafile, newfile2);

}
