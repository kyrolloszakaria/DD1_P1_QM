#include <iostream>
#include <vector>
#include <map>
using namespace std;
struct B_number{
  int number; // decimal
  int dashes; 
  bool used; 
};
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
map<int, vector<int>> table;
void create_p_group() {
  short tmp; // Group Number (number of 1's)
  B_number temp_num;
  int temp_number, temp_dashes;
    map<int, vector<int>>::iterator itrI;
	vector<int>::iterator itrJ;
    vector<int>::iterator itrK;

	auto itr= table.upper_bound(itrI->first);
	int temp_number, temp_dashes;
  for (itrI = table.begin(); itrI!=table.end(); ++itrI) { // for each group
    for (itrJ = itrI->second.begin(); itrJ != itrI->second.end();++itrJ) { // for each element in the group
        ++itrI;
      for (itrK = itrI->second.begin(); itrK != itrI->second.end();++itrJ) { // for each element in the next group
        temp_number = *itrJ & *itrK;
        temp_dashes = *itrJ ^ *itrK;
        if (count_1s(temp_dashes) == 1) {
          table[i][j].used = true;
          table[i + 1][k].used = true;

          tmp = count_1s(temp_number);

          if (tmp + 1 > p_group.size())
            p_group.resize(tmp + 1);

          temp_num = init_B_number(temp_number, temp_dashes, false);
          p_group[tmp].push_back(temp_num);
        }
      }
      --itrI;
    }
  }
}
int main()
{
    int number = 3; //1100
    print_binary(number);
}