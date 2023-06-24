#include <iostream>
#include <string>
#include "mixtree.h"

using PNODE = BNode*;


int main()
{
	PNODE nRoot = ALLOC_N(15);
	nRoot->setTopRoot();

	nRoot->Add(ALLOC_N(9));
	nRoot->Add(ALLOC_N(23));
	nRoot->Add(ALLOC_N(4));
	nRoot->Add(ALLOC_N(12));
	nRoot->Add(ALLOC_N(8));
	nRoot->Add(ALLOC_N(3));
	nRoot->Add(ALLOC_N(17));
	nRoot->Add(ALLOC_N(28));
	
	nRoot->Add(ALLOC_N(2));
	nRoot->Add(ALLOC_N(11));
	nRoot->Add(ALLOC_N(5));
	
	L_HEIGHT(nRoot);
	R_HEIGHT(nRoot);
	
	nRoot->Find(2)->Print();

	nRoot->Find(11)->Print();

	nRoot->Find(28)->Print();

	L_HEIGHT(nRoot);
	R_HEIGHT(nRoot);

	LEFT_TAIL(nRoot);
	RIGHT_TAIL(nRoot);


	NULLP(nRoot);
	return 0;
	
}
