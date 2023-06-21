#include <iostream>
#include <string>
#include "mixtree.h"

using PNODE = BNode*;


int main()
{
	T_INFO nfo;

	PNODE nRoot = ALLOC_N(15);
	nRoot->setTopRoot();

	nRoot->Add(ALLOC_N(9));
	nRoot->Print();


	nRoot->leftTop()->Print();
	nRoot->rightTop()->Print();

	L_HEIGHT(nRoot);
	R_HEIGHT(nRoot);

	LEFT_END_INFO(nRoot);
	RIGHT_END_INFO(nRoot);

	
	nRoot->Add(ALLOC_N(23));
	nRoot->Add(ALLOC_N(17));
	nRoot->Add(ALLOC_N(28));

	nRoot->Add(ALLOC_N(12));
	nRoot->Add(ALLOC_N(4));
	nRoot->Add(ALLOC_N(3));
	nRoot->Add(ALLOC_N(8)); 

	
	L_HEIGHT(nRoot);
	R_HEIGHT(nRoot);

	LEFT_END_INFO(nRoot);
	RIGHT_END_INFO(nRoot);

	nRoot->Find(4)->Print();

	NULLP(nRoot);

	return 0;
}

