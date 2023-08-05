#include <iostream>
#include "mixtree.h"



int main()
{
	PNODE nRoot = ALLOC_N(15);
	BNode::setTopRoot(&nRoot); 

	std::cout << "Adding up.. " << "\n\n";

	nRoot->Add(ALLOC_N(9));
	nRoot->Add(ALLOC_N(23));
	nRoot->Add(ALLOC_N(4));
	nRoot->Add(ALLOC_N(12));
	nRoot->Add(ALLOC_N(8));
	nRoot->Add(ALLOC_N(3)); // R_TURNS happens after this one

	nRoot->Print();
	// try to find ,remove or modify any nodes to ascertain the R_TURNS()
	
	
	PRINT("Delete Node 4.")
	DEL(nRoot, 4);
	
	NODE(nRoot, 4);
	NODE(nRoot, 9);
	NODE(nRoot, 3);

	PRINT("Delete Node 23.")
	DEL(nRoot, 23);
	NODE(nRoot, 23);
	NODE(nRoot, 15);

	L_HEIGHT(nRoot);
	R_HEIGHT(nRoot);
	RET;
	
	//---------------------------------------------------------------

	nRoot->Add(ALLOC_N(17));
	nRoot->Add(ALLOC_N(28)); 

	nRoot->Add(ALLOC_N(2));
	nRoot->Add(ALLOC_N(11));
	nRoot->Add(ALLOC_N(5));

	nRoot->Add(ALLOC_N(16));
	nRoot->Add(ALLOC_N(35)); //L_TURNS() happens after this one
	nRoot->Add(ALLOC_N(52)); 

	nRoot->Print();
	// try to find, modify or remove any nodes to ascertain the L_TURNS()
	
	NODE(nRoot, 28);

	PRINT("Delete Node 28.");
	DEL(nRoot, 28);

	NODE(nRoot, 28);
	NODE(nRoot, 17);

	NODE(nRoot, 35);

	L_HEIGHT(nRoot);
	R_HEIGHT(nRoot);


	delete nRoot;
	

	return -1;
}

