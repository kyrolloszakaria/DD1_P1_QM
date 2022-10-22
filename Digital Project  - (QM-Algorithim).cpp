#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <stdlib.h>
using namespace std;



struct B_number {
    vector<int> minterms; 
    int number; // decimal
    int dashes;
    bool used;
    B_number(int n) 
    {
        this->number = n;
        this->minterms.push_back(n);
        this->dashes = 0;
        this->used = false;

    }
    B_number(int n, int d)
    {
        this->number = n;
        this->dashes = d;
        this->used = false;
    }
    B_number(int n, int d, bool used) 
    {
        this->number = n;
        this->dashes = d;
        this->used = used;
    }
};
map<int,bool> MintermsNcovered; // minterms not covered.
int MIN_BITS = 4; //minimum bits to print
vector <int> input_values; // minterms + don't cares
vector<int> mintermsOnly;
vector<int> dCareOnly;
// <minterm, (occurence,PIs)>
map<int, pair<int,vector<B_number>>> Occurrences; // Number of PIs for each minterm
vector < vector < B_number > > table; //original table
vector < vector < B_number > > p_group; //mid process table
vector < vector < B_number > > final_group; //final table
vector < B_number > printed_numbers; //avoid printing the same final numbers
//----------------------------------------------------------
int count_1s(int number); //count the number of 1s in a number
void print_binary(int number); //print the number in binary
void create_table(); //create original table sorted by the number of 1s
void print_table(); //print the table
B_number init_B_number(int n, int d, bool u); //initialize a B_number
void create_p_group(); //create mid process table
void print_p_group(); //print it
void print_p_binary(int n, int d); //print the mid table (with -'s)
void create_final_group(); //create final table
void print_final_group(); //print final table with -'s and unused terms
bool is_printed(B_number n); //dont print terms that were already printed
void init(); //start the table making and printing
void getinput(); //get input from user
int count_bits(int n); //min bits to represent a number 
void printOccurrencesMap();
void createOccurrencesMap(vector<int> minterms);
void printPIsThatAreNotCoveredByEPIs();
void printEssentialPrimeImplicants();
void getInputFromFile();

int main() {

    cout << "\nQMCS - Quine McCluskey Simplifier\n";
    
   // getinput();
    getInputFromFile();
    init();
    printEssentialPrimeImplicants();
    printPIsThatAreNotCoveredByEPIs();
    return 0;
}
/* counts 1s by getting the LSB (%2) and then shifting until 0 */
int count_1s(int number) {
    short bit = 0;
    int count = 0;
    while (number > 0) {
        bit = number % 2;
        number >>= 1; // divide number by 2. number =\2;
        if (bit) {
            count++;
        }
    }
    return count;
}
/*get LSB, arrange it in array, the print array in reverse order so MSB is on
the left */
void print_binary(int number)  // simply convert decimal to binary
{
    int* bits = new int[MIN_BITS];
   // int bits[MIN_BITS];
    int count = 0;
    while (number > 0 || count < MIN_BITS) {
        bits[count] = number % 2;
        number >>= 1;
        count++;
    }
    for (int i = count - 1; i >= 0; i--)
        cout << bits[i];
}
/*creating first table: append current number to the array located in
table[number of 1s f this number]*/
void create_table() {
    int tmp; // Group Number (number of 1's)
    for (int i = 0; i < input_values.size(); i++) {
        tmp = count_1s(input_values[i]);
        if (tmp + 1 > table.size())
            table.resize(tmp + 1);

        B_number temp_num(input_values[i]);
        table[tmp].push_back(temp_num);
    }
}
void print_table() {
    cout << endl << "COMPUTING:" << endl;
    for (int i = 0; i < table.size(); i++) {
        cout << i;
        for (int j = 0; j < table[i].size(); j++) {
            cout << "\tm" << table[i][j].number << "\t";
            print_binary(table[i][j].number);
            cout << endl;
        }
        cout << "\n-------------------------------------" << endl;
    }
}
/* initialize a B_number variable - like a constructor */
/*void init_B_number(B_number& B, int n, int d, bool u) {
    B_number num;
    num.minterm.push_back(n);
    num.number = n;
    num.dashes = d;
    num.used = u;
    return num;
}*/
/*like the original table, but the paring of numbers from the original tabledashes are represented by a 1. for example original A=0011 B=1011, new number
is -011 which is represented as C.number=A&B=0011,C.dashes=A^B=1000*/
void create_p_group() {
    short tmp; // Group Number (number of 1's)
    int temp_number, temp_dashes; // components of struct
    for (int i = 0; i < table.size() - 1; i++) { // for each group
        for (int j = 0; j < table[i].size(); j++) { // for each element in the group
            for (int k = 0; k < table[i + 1].size(); k++) { // for each element in the next group
                temp_number = table[i][j].number & table[i + 1][k].number;
                temp_dashes = table[i][j].number ^ table[i + 1][k].number;
                if (count_1s(temp_dashes) == 1) 
                {
                    table[i][j].used = true;
                    table[i + 1][k].used = true;
                    B_number temp_num(temp_number, temp_dashes);
                    temp_num.minterms.insert(temp_num.minterms.end(), table[i][j].minterms.begin(), table[i][j].minterms.end());
                    temp_num.minterms.insert(temp_num.minterms.end(), table[i + 1][k].minterms.begin(), table[i + 1][k].minterms.end());


                    tmp = count_1s(temp_number); 

                    if (tmp + 1 > p_group.size())
                        p_group.resize(tmp + 1);

                    p_group[tmp].push_back(temp_num);
                }
            }
        }
    }
}
void print_p_group() 
{
    cout << endl << "MID PROCESS COMPUTATION:" << endl;
    for (int i = 0; i < p_group.size(); i++) 
    {
        cout << i;
        for (int j = 0; j < p_group[i].size(); j++) {
            cout << "\t\t";
            print_p_binary(p_group[i][j].number, p_group[i][j].dashes);
            cout << endl;
        }
        cout << "\n-------------------------------------" << endl;
    }
}
/*print a number such as -001; this allocates bits in an array dash=2 then
prints reverse array */
void print_p_binary(int n, int d) { // print binary representation with dashes
    int* bits = new int[MIN_BITS];
   // int bits[MIN_BITS];
    int count = 0;
    while (n > 0 || count < MIN_BITS) {
        if (!(d % 2)) // if there is not dash
            bits[count] = n % 2;
        else
            bits[count] = 2;
        n >>= 1; // n /=2;
        d >>= 1; // d/=2;
        count++;
    }
    for (int i = count - 1; i >= 0; i--) {
        if (bits[i] != 2)
            cout << bits[i];
        else
            cout << "-";
    }
}
/*creates final table. works like p_group(). example; in p_group you have:
A=-001 B=-011 -> C= -0-1 which will be represented as
C.number=A&B=0001&0011=0001, and
C.dashes=A^B^A.dashes=0001^0011^1000=1010.
Computation is done only when A.dashes = b.dashes*/
void create_final_group() {
    int tmp;
    int temp_number, temp_dashes;
    for (int i = 0; i < p_group.size() - 1; i++) {
        for (int j = 0; j < p_group[i].size(); j++) {
            for (int k = 0; k < p_group[i + 1].size(); k++) 
            {
                if (p_group[i][j].dashes == p_group[i + 1][k].dashes) { // dashes must be in the same position
                    temp_number = p_group[i][j].number & p_group[i + 1][k].number;
                    temp_dashes = p_group[i][j].number ^ p_group[i + 1][k].number;
                    if (count_1s(temp_dashes) == 1) 
                    {
                        temp_dashes ^= p_group[i][j].dashes; // temp_dashes =  temp_dashes ^ p_group[i][j].dashes;
                                                             // to get combination of dashes 

                        p_group[i][j].used = true;
                        p_group[i + 1][k].used = true;
                        // to accumulate minterms:
                        B_number temp_num(temp_number, temp_dashes,false);
                        temp_num.minterms = p_group[i][j].minterms;
                        temp_num.minterms.insert(temp_num.minterms.end(), p_group[i + 1][k].minterms.begin(), p_group[i + 1][k].minterms.end());


                        tmp = count_1s(temp_number);

                       if (tmp + 1 > final_group.size())
                            final_group.resize(tmp + 1);
                            
                        final_group[tmp].push_back(temp_num);
                    }
                }
            }
        }
    }
}
/*print all the values from the final table, except for duplicates.
 print all the unused numbers from original table and mid process table*/
/*struct occurrence {
    int minterm;
    int occurrence;
    vector<B_number> covers;
};
*/
//minterm, PI count
void createOccurrencesMap(vector<int> minterms)
{
    for (int i = 0; i < minterms.size(); i++) {
        Occurrences[minterms[i]] = { 0,{} };
    }
}
void printOccurrencesMap() 
{
    for (auto it = Occurrences.begin(); it != Occurrences.end(); it++)
    {                                                     // number of occurences
        cout << "minterm: " << it->first << " covered by: " << it->second.first << " PIs\n";
    }
    
}
void printEssentialPrimeImplicants() 
{
    // <minterm , occurs ,<PIs>>
    // <int, (int, <B_numbers>)>
    cout << "Essential Prime Implicants are: ";
    for (auto it = Occurrences.begin(); it != Occurrences.end(); it++)
    {                                                     
        if (it->second.first == 1) // if occurences  == 1
        {
            MintermsNcovered[it->first] = true; // minterm covered by this EPIs
            print_p_binary(it->second.second.front().number, it->second.second.front().dashes);
            cout << " ";
        }

    }
    cout << "\n";
}
void printPIsThatAreNotCoveredByEPIs() 
{
    bool flag = true;
    cout << "Minterms that are not covered by the Essential Prime implicants: \n";
    for (auto it = MintermsNcovered.begin(); it != MintermsNcovered.end(); it++) 
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


void print_final_group() {
    cout << endl << "FINAL:\n-------------------------------------" << endl;
    int i, j;
    for (i = 0; i < final_group.size(); i++) 
    {
        for (j = 0; j < final_group[i].size(); j++) 
        {
            if (!is_printed(final_group[i][j])) 
            {

                for (int k = 0; k < final_group[i][j].minterms.size(); k++)
                {
                    int m = final_group[i][j].minterms[k]; // the minterm
                    if (dCareOnly.end() == find(dCareOnly.begin(), dCareOnly.end(), m))
                    {
                        Occurrences[m].first++;
                        Occurrences[m].second.push_back(final_group[i][j]);
                    }

                }
                print_p_binary(final_group[i][j].number, final_group[i][j].dashes);
                cout << endl;
                printed_numbers.push_back(final_group[i][j]);
            }
        }
    }
    for (i = 0; i < p_group.size(); i++) {
        for (j = 0; j < p_group[i].size(); j++) {
            if (!p_group[i][j].used) {

                for (int k = 0; k < p_group[i][j].minterms.size(); k++)
                {
                    int m = p_group[i][j].minterms[k]; // the minterm
                    if (dCareOnly.end() == find(dCareOnly.begin(), dCareOnly.end(), m))
                    {
                        Occurrences[m].first++;
                        Occurrences[m].second.push_back(p_group[i][j]);
                    }
                }
                print_p_binary(p_group[i][j].number, p_group[i][j].dashes);
                cout << endl;
            }
        }
    }
    for (i = 0; i < table.size(); i++) {
        for (j = 0; j < table[i].size(); j++) {
            if (!table[i][j].used) {
                for (int k = 0; k < table[i][j].minterms.size(); k++)
                {
                    int m = table[i][j].minterms[k]; // the minterm
                    if (dCareOnly.end() == find(dCareOnly.begin(), dCareOnly.end(), m))
                    {
                        Occurrences[m].first++;
                        Occurrences[m].second.push_back(table[i][j]);
                    }
                }
                print_p_binary(table[i][j].number, table[i][j].dashes);
                cout << endl;
            }
        }
    }
    cout << "-------------------------------------" << endl;

}
/*used to avoid printing duplicates that can exist in the final table*/
bool is_printed(B_number n) {
    for (int i = 0; i < printed_numbers.size(); i++)
        if (n.number == printed_numbers[i].number && n.dashes ==
            printed_numbers[i].dashes)
            return true;
    return false;
}
/*initialize all table*/
void init() {
    table.resize(1);
    p_group.resize(1);
    final_group.resize(1);
    create_table();
    print_table();
    create_p_group();
    print_p_group();
    create_final_group();
    createOccurrencesMap(mintermsOnly); // pass only minterms as vector<int>
    print_final_group();
    printOccurrencesMap();
    


}
vector<int> inputProcessing(string min, bool isMinterm)
{

    vector<int> fixedInput;
    int sizeMin = min.size();
    vector<int>commosPos; string temp;

    if (min == "")
    {
        fixedInput.resize(0);
        return fixedInput;
    }

    for (int i = 0; i < sizeMin; i++)
    {
        if (min[i] == ',')
            commosPos.push_back(i);
    }
    for (int i = 0; i < commosPos.size(); i++)
    {
        if (i == 0)
        {
            for (int j = 0; j < commosPos[i]; j++)
            {
                temp = temp + min[j];
            }
            int t = stoi(temp);
            fixedInput.push_back(t);
            temp = "";
        }

        else
        {
            for (int j = commosPos[i - 1] + 1; j < commosPos[i]; j++)
            {
                temp = temp + min[j];
            }
            int t = stoi(temp);
            fixedInput.push_back(t);
            temp = "";

        }
    }

    for (int j = commosPos[commosPos.size() - 1]; j < min.size(); j++)
    {
        temp = temp + min[j + 1];
    }
    int t = stoi(temp);
    fixedInput.push_back(t);
    temp = "";

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
            cout << " Error , " << terms[i] << " is not an acceptable minterm ! ";
            exit(1);
        }

    }
    //Check if there is a repeated term 
    sort(terms.begin(), terms.end());
    for (int i = 0; i < terms.size(); i++)
    {
        for (int j = i + 1; j < terms.size(); j++)
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
    int numberOFvariables; string minterms; string dontCare;
    while (!Text1.eof())
    {
        Text1 >> numberOFvariables;
        Text1 >> minterms;
        Text1 >> dontCare;
    }
    MIN_BITS = numberOFvariables;
    mintermsOnly = inputProcessing(minterms,1);
    dCareOnly = inputProcessing(dontCare,0);
    input_values = mintermsOnly;
    input_values.insert(input_values.end(), dCareOnly.begin(), dCareOnly.end());
    sort(input_values.begin(), input_values.end());
   checkInput(MIN_BITS, mintermsOnly);
   checkInput(MIN_BITS, dCareOnly);
   CheckRepeated(mintermsOnly, dCareOnly);
}
void getinput() {
    int in;
    int num_bits = 0;
    cout << "\nInput value followed by ENTER[^D ends input]\n> ";
    while (cin >> in) {
        input_values.push_back(in);
        MintermsNcovered[in] = false;
        num_bits = count_bits(in);
        if (num_bits > MIN_BITS)
            MIN_BITS = num_bits;
        cout << "> ";
    }
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
