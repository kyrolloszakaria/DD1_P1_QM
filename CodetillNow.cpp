#include <iostream>
#include<fstream>
#include<map>
#include<vector>
#include<string>
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;
	
vector<int> inputProcessing(string min)
{
	vector<int> fixedInput;
	int sizeMin = min.length();
	vector<int> commosPos;
    string temp;
	
	
	for (int i = 0;i < sizeMin;i++)
	{
		if (min[i] == ',')
			commosPos.push_back(i);
	}
	for (int i = 0;i < commosPos.size();i++)
	{
		if (i == 0)
		{
			for (int j = 0; j < commosPos[i];j++)
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
		else 
		{
			for (int j = commosPos[i-1]+1;j < commosPos[i];j++)
			{
				temp = temp + min[j];
			}
			int t = stoi(temp);
			fixedInput.push_back(t);
			temp = "";

		}
	}
	
	for (int j = commosPos[commosPos.size()-2] + 1;j < commosPos[commosPos.size() - 1];j++)
	{
		temp = temp + min[j];
	}
	int t = stoi(temp);
	fixedInput.push_back(t);
	temp = "";
	
	return fixedInput;

}
void checkInput(int variables,vector<int> terms)
{
	// Check if the minterm should not exist  
	for (int i = 0;i < terms.size();i++)
	{
		if (terms[i] > pow(2, variables))
		{
			cout << " Error , " << terms[i] << " is not an acceptable minterm ! ";
			exit(1);
		}

	}

	//Check if there is a repeated term 
	sort(terms.begin(), terms.end());
	for(int i=0;i<terms.size();i++)
	{ 
		for (int j = i + 1;j < terms.size();j++)
		{
			if (terms[i] == terms[j])
			{
				cout << "The minterm " << terms[i] << " is repeated !" << endl;
				exit(1);
			}
		}
	}
}
void CheckRepeated(vector<int> min, vector<int> dontCare)
{
	for (int i = 0;i < min.size();i++)
	{
		for (int j = 0;j < dontCare.size();j++)
		{
			if (min[i] ==dontCare[j])
			{
				cout << " The Term : " << min[i] << " exists as minterm and as Don't Care ";
				exit(1);
			}
		}
	}
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
int count_1s(int number) {
	int bit = 0;
	int count = 0;
	while (number > 0)
	{
		bit = number % 2;
		number >>= 1; // divide number by 2. number =\2;
		if (bit)
		{
			count++;
		}
	}
	return count;
}
map <int, vector <int>> create_map(vector <int> Minterms, vector <int> DontCares, int NumberOfVariables) {
	multimap <int, int> groups_map; // multi in order to have the same key more than one time and to sort them accordingly
	int num_ones;

	for (int i = 0; i < Minterms.size(); i++) {
		groups_map.insert({ count_1s(Minterms[i]),Minterms[i] });

	}
	for (int i = 0; i < DontCares.size(); i++) {
		groups_map.insert({ count_1s(DontCares[i]),DontCares[i] });

	}


	map<int, vector<int>> final_groups;
	vector<int> temp;
	auto i = groups_map.begin();
	for (; i != groups_map.end(); )
	{
		temp.clear();
		//cout << i->first << ":  ";
		auto itr = groups_map.lower_bound(i->first);
		for (; itr != groups_map.upper_bound(i->first); itr++) {

			//cout << itr->second << " ";
			temp.push_back(itr->second);

		}
		final_groups.insert({ i->first,temp });

		i = itr;    //this skips i through all the values for the key: "i->first" and so it won't print the above loop multiple times (equal to the number of values for the corresponding key).
		cout << "\n";

	}




	return final_groups;


}
//multimap <int, int> create_map(vector <int> Minterms, vector <int> DontCares, int NumberOfVariables) {
//	multimap <int, int> groups_map;// multi in order to have the same key more than one time and to sort them accordingly
//	int num_ones;
//
//	for (int i = 0; i < Minterms.size(); i++) {
//		groups_map.insert({ count_1s(Minterms[i]),Minterms[i] });
//
//	}
//	for (int i = 0; i < DontCares.size(); i++) {
//		groups_map.insert({ count_1s(DontCares[i]),DontCares[i] });
//
//	}
//	// the following commented loop is just to test the map is working correctly
//	int key = 0;
//
//
//
//	return groups_map;
//
//
//}
// convert the number to binary 
int main()
{
	ifstream Text1;
	Text1.open("Text.txt");
	int numberOFvariables;
    string minterms;
    string dontCare;
	while (!Text1.eof())
	{
		Text1 >> numberOFvariables;
		Text1 >> minterms;
		Text1 >> dontCare;
	}

	vector<int> Min_1 = inputProcessing(minterms); //Min_1 : all minterms
	vector<int> dCare = inputProcessing(dontCare); // dCare : all don't cares
	checkInput(numberOFvariables,Min_1);
	checkInput(numberOFvariables,dCare);
	CheckRepeated(Min_1, dCare);

	map <int, vector<int>> test = create_map(Min_1, dCare,numberOFvariables);
	//cout << endl << endl;
	auto i = test.begin();
	for (; i != test.end();i++) {
		cout << i->first << "  ";
		for (auto j = i->second.begin(); j != i->second.end();j++) {
			cout << *j << " ";

		}
		cout << endl;
	}

	/*for (int i = 0;i < dCare.size();i++)
	{
		cout << dCare[i] << endl;
	}
	cout << " ---------------------"; 
	for (int i = 0;i < Min_1.size();i++)
	{
		cout << Min_1[i] << endl;
	}*/
}