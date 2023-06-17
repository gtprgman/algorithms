#include <iostream>
#include <string>
#include "mixtree.h"

using PNODE = BNode*;


int main()
{
	PNODE nRoot = ALLOC_N(15);
	nRoot->setTopRoot(nRoot);


	nRoot->Add(ALLOC_N(9));
	nRoot->Add(ALLOC_N(23));

	nRoot->Add(ALLOC_N(17));
	nRoot->Add(ALLOC_N(28));

	nRoot->Add(ALLOC_N(4));
	nRoot->Add(ALLOC_N(12));

	nRoot->Add(ALLOC_N(3));
	nRoot->Add(ALLOC_N(8));

	nRoot->Find(3)->Print();
	nRoot->Find(3)->Parent()->Print(); // node 4->Print();

	nRoot->Find(9)->Print();

	// recall that BNode::Remove() is now implied move instead of copy..
	std::cout << "Removing Node 4.." << "\n\n";
	nRoot->Find(4)->Remove()->Print();;

	PNODE nx = nRoot->Find(4);

	nx->Print();

	nx = nRoot->Find(9);

	nRoot->Print();

	nx->Print();

	nx = nRoot->Find(28)->Parent();

	nx->Print();

	nRoot->Find(28)->Print();

	std::cout << "Removing Node 28.. " << "\n\n";

	nRoot->Find(28)->Remove()->Print();

	nx->Print();

	nRoot->Remove()->Print();

	RET;


	NULL2P(nRoot,nx);

	return 0;
}

