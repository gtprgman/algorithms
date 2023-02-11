#include "mixutil.h"
#include "mixer.h"




int main() {

	std::initializer_list<int> il = { 250, 300, 350 };

	iList2<int> l2st = il;

	iList2<int>&& l3st = std::move(l2st);


	for (auto const& t : l2st)
		std::cout << t << " , ";
	
	std::cout << "\n";
	std::cout << "verifying content in l2st.. \n ";
	std::cout << l2st[0] << "," <<  l2st[1] << "," << l2st[2] << " \n";

	std::cout << "\n";
	std::cout << "verifying content in l3st.. \n";
	std::cout << l3st[0] << "," << l3st[1] << "," << l3st[2] << "\n";
	

	std::cout << "\n\n printing l3st values .. \n\n";

	for (auto const& k : l3st)
		std::cout << k << ", ";
	


	system("PAUSE");
	return 0;
}


