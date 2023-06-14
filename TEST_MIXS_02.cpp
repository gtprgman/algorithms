#include <iostream>
#include <string>
#include "mixtree.h"

using PNODE = BNode*;


int main()
{
	PNODE nRoot = ALLOC_N(15);
	setTopRoot(nRoot);


	nRoot->Add(ALLOC_N(9));
	nRoot->Add(ALLOC_N(23));

	nRoot->Add(ALLOC_N(17));
	nRoot->Add(ALLOC_N(28));

	nRoot->Add(ALLOC_N(4));
	nRoot->Add(ALLOC_N(12));

	nRoot->Add(ALLOC_N(3));
	nRoot->Add(ALLOC_N(8));

	Find(3)->Print();
	Find(3)->Parent()->Print(); // node 4->Print();

	Find(9)->Print();

	
	// recall that BNode::Remove() is now implied move instead of copy..
	std::cout << "Removing Node 4.." << "\n\n";
	Find(4)->Remove()->Print();;

	setTopRoot(nRoot);

	// to hinder being bewildered by the extracted error statement of Find()
	PNODE nx = Find(4);

	nx->Print();

	nx = Find(9);

	nRoot->Print();

	nx->Print();

	nx = Find(28)->Parent();

	nx->Print();

	Find(28)->Print();

	std::cout << "Removing Node 28.. " << "\n\n";

	Find(28)->Remove()->Print();
	setTopRoot(nRoot);

	nx->Print();

	NULL2P(nRoot,nx);

	return 0;
}
