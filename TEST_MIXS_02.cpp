#include "mixutil.h"
#include "mixer.h"


using P_NODE = BNode*;


int main() {
	
	P_NODE nd0 = ALLOC_N("Tom");

	if P_ASSERT(nd0) nd0->Print();

	
	P_NODE nd1 = treeAdd(nd0, "Peter");

	std::cout << "\n";

	nd0->Print();


	std::cout << "\n";

	nd1->Print();


	std::cout << "\n\n";
	
}


