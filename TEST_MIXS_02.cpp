#include <iostream>
#include <string>
#include "mixtree.h"



int main()
{
	PNODE nRoot = ALLOC_N(15);
	nRoot->setTopRoot();

	std::cout << "Adding up.. " << "\n\n";

	/* Use BRoot as a Global Static Root to make it
		Fixed to the root */
	BRoot->Add(ALLOC_N(9));
	BRoot->Add(ALLOC_N(23));
	BRoot->Add(ALLOC_N(4));
	BRoot->Add(ALLOC_N(12));
	BRoot->Add(ALLOC_N(8));
	BRoot->Add(ALLOC_N(3)); // R_TURNS happens after this one

	BRoot->Add(ALLOC_N(17));
	BRoot->Add(ALLOC_N(28));

	BRoot->Add(ALLOC_N(2));
	BRoot->Add(ALLOC_N(11));
	BRoot->Add(ALLOC_N(5));


	BRoot->Print();

	// try print any number of nodes to ascertain the rotation function
	BRoot->Find(4)->Print();


	L_HEIGHT(BRoot);
	R_HEIGHT(BRoot);

	free(BRoot);


	NULLP(nRoot);
	return 0;

}

