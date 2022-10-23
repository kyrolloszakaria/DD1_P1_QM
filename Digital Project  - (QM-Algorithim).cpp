#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <stdlib.h>
using namespace std;



struct Implicant {
    vector<int> minterms; // minterms covered by this PI
    int MinNum; // decimal
    int dashes;
    bool used;// paired or not
    Implicant(int n) // when creating the Column_I while taking the input
    {
        this->MinNum = n;
        this->minterms.push_back(n);
        this->dashes = 0;
        this->used = false;

    }
    Implicant(int n, int d) 
    {
        this->MinNum = n;
        this->dashes = d;
        this->used = false;
    }
    //Implicant(int n, int d, bool used) 
    //{
    //    this->number = n;
    //    this->dashes = d;
    //    this->used = used;
    //}
};
map<int,bool> MintermsCovered; // minterms covered or not by EPIS
int VarNum; //minimum bits to print
vector <int> input_values; // minterms + don't cares
vector<int> mintermsOnly;
vector<int> dCareOnly;
// <minterm, (occurences (number of PIs covering thos minterm),PIs)>
map<int, pair<int,vector<Implicant>>> Occurrences; // Number of PIs for each minterm
vector < vector < Implicant > > Column1; //original table
vector < vector < Implicant > > Column2; //mid process table
vector < vector < Implicant > > Column3; //final table
vector < Implicant > printed_implicants; //avoid printing the same final numbers
//----------------------------------------------------------
int count_1s(int number); //count the number of 1s in a number
void print_binary(int number); //print the number in binary
void create_Col_I(); //create original table sorted by the number of 1s
void print_CoL_I(); //print the table
Implicant init_B_number(int n, int d, bool u); //initialize a B_number
void create_Col_II(); //create mid process table
void print_Col_II(); //print it
string print_PI_binary_dashes(int n, int d); //print the mid table (with -'s)
void create_Col_III(); //create final table
void print_Col_III(); //print final table with -'s and unused terms
bool is_printed(Implicant n); //dont print terms that were already printed
void init(); //start the table making and printing
void getinput(); //get input from user
int count_bits(int n); //min bits to represent a number 
void printOccurrencesMap();
void createOccurrencesMap(vector<int> minterms);
void printMinsThatAreNotCoveredByEPIs();
void printEssentialPrimeImplicants();
void getInputFromFile();

void printBooleanexpression(string binary);

int main() {

    cout << "\nQuine McCluskey Algorithm - DD1_P1\n";
    getInputFromFile();
    create_Col_I();
    print_CoL_I();
    create_Col_II();
    print_Col_II();
    create_Col_III();
    createOccurrencesMap(mintermsOnly); // pass only minterms as vector<int>
    print_Col_III();
    printOccurrencesMap();
    printEssentialPrimeImplicants();
    printMinsThatAreNotCoveredByEPIs();
    return 0;
}
/* counts 1s by getting the LSB (%2) and then shifting until 0 */
int count_1s(int number) // convert decimal to binary and count number of 1's
{
    int bit = 0;
    int count = 0;
    while (number > 0) 
    {
        bit = number % 2; 
        number = number / 2; 
        if (bit) // if bit is 1
        {
            count++;
        }
    }
    return count;
}
/*get LSB, arrange it in array, the print array in reverse order so MSB is on
the left */
void print_binary(int number)  // simply convert decimal to binary
{
    int* bits = new int[VarNum];
    int count = 0;
    while (number > 0 || count < VarNum) {
        bits[count] = number % 2;
        number /=2;
        count++;
    }
    for (int i = count - 1; i >= 0; i--)
        cout << bits[i];
}


void create_Col_I() {
    int OnesNum; // Group Number (number of 1's)
    for (int i = 0; i < input_values.size(); i++) 
    {
        OnesNum = count_1s(input_values[i]);

        if (OnesNum + 1 > Column1.size())
            Column1.resize(OnesNum + 1);

        Implicant temp_num(input_values[i]); // make PI = number(minterm), dashes => none, used => false, minterms -> this number(minterm)
        Column1[OnesNum].push_back(temp_num);
    }
}

void print_CoL_I() {
    cout << endl << "Coulmn_I:" << endl;
    for (int i = 0; i < Column1.size(); i++) {
        cout << "G_" << i; // Group Number
        for (int j = 0; j < Column1[i].size(); j++) // elements of each group
        {
            cout << "\tm" << Column1[i][j].MinNum << "\t";
            print_binary(Column1[i][j].MinNum);
            cout << endl;
        }
        cout << "\n-------------------------------------" << endl;
    }
}


void create_Col_II() {
    int onesNum; // Group Number (number of 1's)
    int temp_number, temp_dashes; // components of struct
    for (int i = 0; i < Column1.size() - 1; i++) { // for each group
        for (int j = 0; j < Column1[i].size(); j++) { // for each element in the group
            for (int k = 0; k < Column1[i + 1].size(); k++) { // for each element in the next group
                temp_number = Column1[i][j].MinNum & Column1[i + 1][k].MinNum; // to copy any of them except for the diffreneces
                temp_dashes = Column1[i][j].MinNum ^ Column1[i + 1][k].MinNum; // get 1 if different so temp_dashes are the positions of dashes
                if (count_1s(temp_dashes) == 1) // different only in one bit
                {
                    Column1[i][j].used = true;
                    Column1[i + 1][k].used = true;
                    Implicant temp_num(temp_number, temp_dashes);
                    temp_num.minterms = Column1[i][j].minterms;
                    temp_num.minterms.insert(temp_num.minterms.end(), Column1[i + 1][k].minterms.begin(), Column1[i + 1][k].minterms.end());


                    onesNum = count_1s(temp_number); 

                    if (onesNum + 1 > Column2.size())
                        Column2.resize(onesNum + 1);

                    Column2[onesNum].push_back(temp_num);
                }
            }
        }
    }
}
void print_Col_II() 
{
    cout << endl << "Column_II:" << endl;
    for (int i = 0; i < Column2.size(); i++) 
    {
        cout << "G_" << i;
        for (int j = 0; j < Column2[i].size(); j++) {
            cout << "\t\t";
            print_PI_binary_dashes(Column2[i][j].MinNum, Column2[i][j].dashes);
            cout << endl;
        }
        cout << "\n-------------------------------------" << endl;
    }
}

string print_PI_binary_dashes(int n, int d) { // print binary representation with dashes
    int* bits = new int[VarNum];
    string boolExp;
    int count = 0;
    while (n > 0 || count < VarNum) {
        if (!(d % 2)) // if there is not dash
            bits[count] = n % 2;
        else
            bits[count] = 2;
        n /=2;
        d /=2;
        count++;
    }
    for (int i = count - 1; i >= 0; i--) 
    {
        if (bits[i] != 2)
        {
            cout << bits[i];
            boolExp += char(48 + bits[i]); // converting int to string
        }
            
        else
        {
            cout << "-";
            boolExp += "-";
        }
           
    }
    cout << "\n";
    boolExp += "+";
    return boolExp;
}

void create_Col_III() {
    int onesNum;
    int temp_number, temp_dashes;
    for (int i = 0; i < Column2.size() - 1; i++) {
        for (int j = 0; j < Column2[i].size(); j++) {
            for (int k = 0; k < Column2[i + 1].size(); k++) 
            {
                if (Column2[i][j].dashes == Column2[i + 1][k].dashes) { // dashes must be in the same position for implicants to be pairable
                    temp_number = Column2[i][j].MinNum & Column2[i + 1][k].MinNum;
                    temp_dashes = Column2[i][j].MinNum ^ Column2[i + 1][k].MinNum;
                    if (count_1s(temp_dashes) == 1) 
                    {
                        temp_dashes ^= Column2[i][j].dashes; // temp_dashes =  temp_dashes ^ Column2[i][j].dashes;
                                                             // to get combination of dashes 
                                                             // to get position of new dash
                        //11011 ^ 01011 -> 10000
                        Column2[i][j].used = true;
                        Column2[i + 1][k].used = true;
                        // to pair minterms:
                        Implicant temp_num(temp_number, temp_dashes);
                        temp_num.minterms = Column2[i][j].minterms;
                        temp_num.minterms.insert(temp_num.minterms.end(), Column2[i + 1][k].minterms.begin(), Column2[i + 1][k].minterms.end());


                        onesNum = count_1s(temp_number);

                       if (onesNum + 1 > Column3.size())
                            Column3.resize(onesNum + 1);
                            
                        Column3[onesNum].push_back(temp_num);
                    }
                }
            }
        }
    }
}

//   minterm,  PIcount , covers
// map<int, pair<int, vector<Implicant>>> Occurrences;
void createOccurrencesMap(vector<int> minterms)
{
    for (int i = 0; i < minterms.size(); i++) {
        Occurrences[minterms[i]] = { 0,{} };
    }
}
void printOccurrencesMap() 
{
    cout << "Occurences of each minterm:\n";
    for (auto it = Occurrences.begin(); it != Occurrences.end(); it++)
    {                                                     // number of occurences
        cout << "minterm: " << it->first << " covered by: " << it->second.first << " PIs\n";
    }
    
}
void printEssentialPrimeImplicants() 
{
    string BoolExp; //ABCD


   // <minterm , occurs ,<PIs>> Occurences
   // <int, (int, <Implicant>)> Occurences
   
   //  <minterm, isCovered>
   // map<int, bool> MintermsCovered;
    cout << "Essential Prime Implicants are: ";
    for (auto it = Occurrences.begin(); it != Occurrences.end(); it++)
    {                                                     
        if (it->second.first == 1) // if occurences  == 1
        {
            //                            EPI     ->      its minterms      
            for (int i = 0; i < it->second.second.front().minterms.size(); i++)
            {
                MintermsCovered[it->second.second.front().minterms[i]] = true;
            }
            BoolExp += print_PI_binary_dashes(it->second.second.front().MinNum, it->second.second.front().dashes);
            cout << " ";
        }
    }
    cout << "\n";
   printBooleanexpression(BoolExp);
}

void printMinsThatAreNotCoveredByEPIs() 
{
    bool flag = true;
    cout << "\nMinterms that are not covered by the Essential Prime implicants: \n";
    for (auto it = MintermsCovered.begin(); it != MintermsCovered.end(); it++) 
    {
        if (!it->second)
        {
            flag = false;
            cout << it->first;
            cout << "  ";
        }
    }
    if (flag)
    {
        cout << "All minterms are covered by the Essential Prime Implicants.\n";
        cout << "This is Minimal QuineMcCluskey Expression.\n";
    }
}


void print_Col_III() {
    cout << endl << "Column_III:\n-------------------------------------" << endl;
    int i, j;
    for (i = 0; i < Column3.size(); i++) // for each group
    {
        for (j = 0; j < Column3[i].size(); j++) // for each element in the group
        {
            if (!is_printed(Column3[i][j])) 
            {
                for (int k = 0; k < Column3[i][j].minterms.size(); k++) // to check if the covered minterms are whether minterms or don't cares
                                                                        // to create the occurneces map and then get the EPIs
                {
                    int m = Column3[i][j].minterms[k]; // the minterm
                    if (dCareOnly.end() == find(dCareOnly.begin(), dCareOnly.end(), m)) // if this minterm doesn't exits in dCares, then it is a minterm
                    {
                        Occurrences[m].first++; // occurences[minterm].NumberOfPIs++
                        Occurrences[m].second.push_back(Column3[i][j]); // occurences[minterm].PIs.push(PI)
                    }

                }
                print_PI_binary_dashes(Column3[i][j].MinNum, Column3[i][j].dashes);
                cout << endl;
                printed_implicants.push_back(Column3[i][j]);
            }
        }
    }
    for (i = 0; i < Column2.size(); i++) {
        for (j = 0; j < Column2[i].size(); j++) {
            if (!Column2[i][j].used) {

                for (int k = 0; k < Column2[i][j].minterms.size(); k++)
                {
                    int m = Column2[i][j].minterms[k]; // the minterm
                    if (dCareOnly.end() == find(dCareOnly.begin(), dCareOnly.end(), m))
                    {
                        Occurrences[m].first++;
                        Occurrences[m].second.push_back(Column2[i][j]);
                    }
                }
                print_PI_binary_dashes(Column2[i][j].MinNum, Column2[i][j].dashes);
                cout << endl;
            }
        }
    }
    for (i = 0; i < Column1.size(); i++) {
        for (j = 0; j < Column1[i].size(); j++) {
            if (!Column1[i][j].used) {
                for (int k = 0; k < Column1[i][j].minterms.size(); k++)
                {
                    int m = Column1[i][j].minterms[k]; // the minterm
                    if (dCareOnly.end() == find(dCareOnly.begin(), dCareOnly.end(), m))
                    {
                        Occurrences[m].first++;
                        Occurrences[m].second.push_back(Column1[i][j]);
                    }
                }
                print_PI_binary_dashes(Column1[i][j].MinNum, Column1[i][j].dashes);
                cout << endl;
            }
        }
    }
    cout << "-------------------------------------" << endl;

}
/*used to avoid printing duplicates that can exist in the final table*/
bool is_printed(Implicant n) {
    for (int i = 0; i < printed_implicants.size(); i++)
        if (n.MinNum == printed_implicants[i].MinNum && n.dashes == printed_implicants[i].dashes)
            return true;
    return false;
}
/*initialize all table*/

vector<int> inputProcessing(string line, bool isMinterm)
{

    vector<int> fixedInput;
    int sizeMin = line.size();
    vector<int>commosPos; string temp;
    try
    {
        if (line == "") // handels if no minterms OR no don't cares
        {
            fixedInput.resize(0);
            return fixedInput;
        }

        for (int i = 0; i < sizeMin; i++) // get positions of all commas
        {
            if (line[i] == ',')
                commosPos.push_back(i);
        }

        if (commosPos.size() == 0) // if no commas
        {
            int t = stoi(line);
            if (isMinterm) MintermsCovered[t] = false; // init new minterm (t) in MintermsNcovered map and make it not covered (false).
            fixedInput.push_back(t);
            temp = "";
        }
        else 
        {
            for (int i = 0; i < commosPos.size(); i++)
            {
                if (i == 0)
                {
                    for (int j = 0; j < commosPos[i]; j++) // tske first minterm/dCare till first comma
                    {
                        temp = temp + line[j];
                    }
                    int t = stoi(temp);
                    if (isMinterm) MintermsCovered[t] = false;
                    fixedInput.push_back(t);
                    temp = "";
                }

                else
                {

                    for (int j = commosPos[i - 1] + 1; j < commosPos[i]; j++) // start from the next pos of the prev comma until the next comma
                    {
                        temp = temp + line[j];
                    }

                    int t = stoi(temp);
                    if (isMinterm) MintermsCovered[t] = false;
                    fixedInput.push_back(t);
                    temp = "";
                }
            }

            for (int j = commosPos[commosPos.size() - 1] + 1; j < line.size(); j++) // start from posistion after the last comma
            {
                temp = temp + line[j];
            }

            int t = stoi(temp);
            if (isMinterm) MintermsCovered[t] = false;
            fixedInput.push_back(t);
            temp = "";
        }
    }
    catch(int num)
    {
        cout << " Error procssing the input";
        exit(3);
    }
   
    return fixedInput;

}
void checkInput(int variables, vector<int> terms)
{
    // Check if the minterm should not exist  
    if (terms.size() == 0)
        return;

    for (int i = 0; i < terms.size(); i++)
    {
        if (terms[i] > pow(2, variables))
        {
            cout << " Error , " << terms[i] << " is out of range ! ";
            exit(1);
        }

    }
    //Check if there is a repeated term 
    for (int i = 0; i < terms.size()-1; i++)
    {
            if (terms[i] == terms[i+1])
            {
                cout << "The minterm " << terms[i] << " is repeated !" << endl;
                exit(1);
            }
    }
}

void CheckRepeated(vector<int> min, vector<int> dontCare)
{
    for (int i = 0; i < min.size(); i++)
    {
        for (int j = 0; j < dontCare.size(); j++)
        {
            if (min[i] == dontCare[j])
            {
                cout << " The Term : " << min[i] << " exists as minterm and as Don't Care ";
                exit(1);
            }
        }
    }
}

void getInputFromFile() {
    ifstream Text1;
    Text1.open("TestCases.txt");
    string minterms; string dontCare;

        Text1 >> VarNum;
        Text1 >> minterms;
        Text1 >> dontCare;
    
     
    mintermsOnly = inputProcessing(minterms,1);
    dCareOnly = inputProcessing(dontCare,0);
    input_values = mintermsOnly;
    input_values.insert(input_values.end(), dCareOnly.begin(), dCareOnly.end());
    sort(input_values.begin(), input_values.end());
   checkInput(VarNum, mintermsOnly);
   checkInput(VarNum, dCareOnly);
   CheckRepeated(mintermsOnly, dCareOnly);
}

/*return min number of bits a number is represented by. used for best output*/
int count_bits(int n) {
    short bit = 0;
    int count = 0;
    while (n > 0) {
        bit = n % 2;
        n >>= 1;
        count++;
    }
    return count;
}

void printBooleanexpression(string binary) 
{
    cout << "Boolean Expression for the EPIs: ";
    int letter = 65; // A
    for (int i =0; i < binary.length()-1; i++)
    {
        if (binary[i] == '+') letter = 65;
        if (binary[i] == '0') cout << '~'<< char(letter);
        if (binary[i] == '1') cout << char(letter);
        if (binary[i] == '2') letter++;
        letter++;  
    }
    cout << "\n";
}
