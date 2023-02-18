#include "mixutil.h"
#include "mixer.h"




int main() {

	std::array<int, 3> arr = { 229, 365, 710 };
	
	iList2<int, 3> ls = arr;
	
	std::cout << "\n\n";

	for (auto const& k : ls)
		std::cout << k << ", ";

	iList2<int, 3>&& ls2 = _MOVE(ls);

	std::cout << "\n\n";

	for (auto const& j : ls2)
		std::cout << j << ", ";

	std::cout << "\n\n";

	
	int nums[] = { 519, 625, 901 };
	ls2 = nums;


	std::cout << "\n\n";
	
	for (auto const& t : ls2)
		std::cout << t << " ,";


	std::cout << "\n\n";

	system("PAUSE");
	return 0;
}


