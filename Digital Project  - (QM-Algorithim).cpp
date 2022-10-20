// Digital Project  - (QM-Algorithim).cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <cmath>
#include <iostream>
#include<fstream>
#include<vector>
#include<string>
using namespace std;

// solve the error in the input 

// input verification : 
//	1- if the one of the minterms is not in the range
//  2- if the input is not int 

vector<int> inputProcessing(string min)
{
	vector<int> fixedInput;
	int sizeMin = min.size();
	vector<int>commosPos;string temp;
	
	
	for (int i = 0;i < sizeMin;i++)
	{
		if (min[i] == ',')
			commosPos.push_back(i);
	}
	for (int i = 0;i < commosPos.size();i++)
	{
		if (i == 0)
		{
			for (int j = 0;j < commosPos[i + 1];j++)
			{
				temp = temp + min[j];
			}
			int t = stoi(temp);
			fixedInput.push_back(t);
			temp = "";
		}
		else if (i == commosPos.size() - 1)
		{
			for (int j = commosPos[i];j < min.size();j++)
			{
				temp = temp + min[j+1];
			}
			int t = stoi(temp);
			fixedInput.push_back(t);
			temp = "";
		}
		else {
			for (int j = commosPos[i-1];j < commosPos[i + 1];j++)
			{
				temp = temp + min[j + 1];
			}
			int t = stoi(temp);
			fixedInput.push_back(t);
			temp = "";

		}
	}
	
	
	return fixedInput;

}
void checkInput(int variables,vector<int> terms)
{
	// Check if the minterm should not exist  
	for (int i = 0;i < terms.size();i++)
	{
		if (terms[i] > pow(2, variables))
		{
			cout << " Error !! ";
		}

	}
	//Check if there is a repeated term 
}
int convertBinary(int n)
{
	int a[10000], i;
	for (i = 0; n > 0; i++)
	{
		a[i] = n % 2;  
		n = n / 2;
	}
	
	string temp="";
	string fix = "";
	for (i = i - 1;i >= 0;i--)
	{
		temp = to_string(a[i]);
		fix = fix + temp;
		temp = "";
	}

	int Binary = stoi(fix);
	return Binary;
}

// convert the number to binary 
int main()
{
	ifstream Text1;
	Text1.open("D://Digital Design//Text.txt");
	int numberOFvariables;string minterms;string dontCare;
	while (!Text1.eof())
	{
		Text1 >> numberOFvariables;
		Text1 >> minterms;
		Text1 >> dontCare;
	}

	vector<int> Min_1 = inputProcessing(minterms);
	vector<int> dCare = inputProcessing(dontCare);
	checkInput(numberOFvariables, Min_1);
	checkInput(numberOFvariables, dCare);
	//vector<int>BinaryMinterms;
}

