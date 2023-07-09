#include <iostream>
#include <string>
#include "mixtree.h"



int main()
{
	PNODE nRoot = ALLOC_N(15);
	setTopRoot(&nRoot);

	std::cout << "Adding up.. " << "\n\n";

	nRoot->Add(ALLOC_N(9));
	nRoot->Add(ALLOC_N(23));
	nRoot->Add(ALLOC_N(4));
	nRoot->Add(ALLOC_N(12));
	nRoot->Add(ALLOC_N(8));
	nRoot->Add(ALLOC_N(3)); // R_TURNS happens after this one

	nRoot->Add(ALLOC_N(17));
	nRoot->Add(ALLOC_N(28));

	nRoot->Add(ALLOC_N(2));
	nRoot->Add(ALLOC_N(11));
	nRoot->Add(ALLOC_N(5));

	nRoot->Add(ALLOC_N(16));
	nRoot->Add(ALLOC_N(35));
	//nRoot->Add(ALLOC_N(52)); // L_TURNS should happen after this

	nRoot->Print();

	// try print any number of nodes to ascertain the rotation function
	nRoot->Find(12)->Remove()->Print();
	nRoot->Find(15)->Print();


	L_HEIGHT(nRoot);
	R_HEIGHT(nRoot);

	delete nRoot;

	return -1;

}

