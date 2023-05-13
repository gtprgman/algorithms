#include "mixutil.h"
#include "mixer.h"


using P_NODE = BNode*;


int main() {
	P_NODE nd0 = ALLOC_N("Tom");

	nd0->Add(ALLOC_N("Jade"));
	nd0->Add(ALLOC_N("Kaleen"));

	nd0->Print(); std::cout << "\n\n";

	P_NODE nd1 = nd0->Left();

	nd1->Print(); std::cout << "\n\n";


	nd0->Add(ALLOC_N("Ulfha"));

	nd0->Print(); std::cout << "\n\n";


	P_NODE nd2 = traverseNodes(nd0, "Jade");

	nd2->Print();

	std::cout << "\n\n";

	P_NODE nd3 = treeAdd(nd0, "Heather");

	nd3->Print();

	std::cout << "\n\n";

	nd0->Print();

	std::cout << "\n\n";

	nd0->Add(ALLOC_N("Alice"));

	nd0->Left()->Print();

	std::cout << "\n\n";

	nd0->Left()->Left()->Print();

	std::cout << "\n\n";	
}


