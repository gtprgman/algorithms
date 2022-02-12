#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "usorts_impl.h"


using namespace std;


int main(int argc, char* args[])
{
    int set1[3] = { 75,62,55 };
	
	int set2[23] = { 458,895,625,150,478,
					 325,287,605,715,950,
					 110,98,75,69,43,
					 62,55,80,95,114,
					 129,101,95 };

	
	static unique_ptr<int[]> p1(set1);
	static unique_ptr<int[]> p2(set2);

	static unique_ptr<int[]> res1 = std::move(mergesSort(std::move(p1),3));
	static unique_ptr<int[]> res2 = std::move(mergesSort(std::move(p2), 23));
	
		for (int g = 0; g < 3; g++)
			cout << res1[g] << ",";

		cout << endl;


		for (int u = 0; u < 23; u++) {
			cout << res2[u] << ",";

			if (u == 10) cout << "\n";
		}



	system("PAUSE");
	return -1;
}
