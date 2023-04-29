#include "mixutil.h"
#include "mixer.h"




int main() {
	
	P_NODE nd1 = ALLOC_N("Tom");

	nd1->Print();
	
	std::cout << "\n";

	nd1->Add(ALLOC_N("Peter"));
	nd1->Add(ALLOC_N("Vera"));

	std::cout << "\n";

	nd1->Print();

	std::cout << "\n\n";

	P_NODE nd2 = traverseNodes(nd1->_Left, "Peter");
	
	
	if P_ASSERT(nd2) std::cout << TEXT(nd2) << "\n\n";
	else std::cout << "empty" << "\n\n"; 


	nd2 = treeAdd(nd1, "Michael");


	if P_ASSERT(nd2) std::cout << TEXT(nd2) << "\n\n";
	else std::cout << "empty" << "\n\n";

	
	nd2->Print();

	std::cout << "\n\n";
	
	
}


