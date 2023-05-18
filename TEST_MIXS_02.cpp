
#include "mixutil.h"
#include "mixer.h"


using P_NODE = BNode*;


int main()
{
	P_NODE nd0 = ALLOC_N("Tom");


	nd0->Add(ALLOC_N("Jade"));
	nd0->Add(ALLOC_N("Ulfha"));

	nd0->Print(); std::cout << "\n\n";

	P_NODE nd1 = nd0->Left(); // jade

	nd1->Insert(ALLOC_N("Irene"));

	nd0->Print();

	std::cout << "\n\n";

	std::cout << "Left Height: " << BNode::T_LEFT() << "\n";
	std::cout << "Right Height: " << BNode::T_RIGHT() << "\n";

	std::cout << "\n\n";

	nd1->Print();

	P_NODE nd01 = nd1->Parent();

	std::cout << "\n\n";

	nd01->Print();

	std::cout << "\n\n";

	nd01 = treeAdd(nd0, "Gerry");

	nd01->Print();

	std::cout << "\n\n";

	std::cout << "Left Height: " << BNode::T_LEFT() << "\n";
	std::cout << "Right Height: " << BNode::T_RIGHT() << "\n";

	std::cout << "\n\n";

}

