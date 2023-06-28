#include <iostream>
#include <string>
#include "mixtree.h"

using PNODE = BNode*;


int main()
{
	PNODE nRoot = ALLOC_N(15);
	nRoot->setTopRoot();

	std::cout << "Adding up.. " << "\n\n";

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

	RET;

	LEFT_TAIL(nRoot);
	RIGHT_TAIL(nRoot);

	RET;

	std::cout << "Deleting Node 8 .. " << "\n";

	nRoot->Find(8)->Remove();
	std::cout << "Node 8 Deleted. " << "\n\n";

	std::cout << "Searching Node 8.. .. .. " << "\n";
	nRoot->Find(8)->Print();

	std::cout << "Node 8 : " << P_ASSERT(nRoot->Find(8)) << "\n\n";

	std::cout << "List Node 4.. " << "\n";
	nRoot->Find(4)->Print();

	std::cout << "Deleting Node 28 .. " << "\n";

	nRoot->Find(28)->Remove();
	std::cout << "Node 28 Deleted. " << "\n\n";

	std::cout << "List Node 23.. " << "\n";
	nRoot->Find(23)->Print();

	R_HEIGHT(nRoot);
	RIGHT_TAIL(nRoot);

	RET;

	std::cout << "Deleting Node 2.. " << "\n";

	nRoot->Find(2)->Remove();
	std::cout << "Node 2 Deleted. " << "\n";
	RET;

	std::cout << "List Node 2.. " << "\n";
	nRoot->Find(2)->Print();

	L_HEIGHT(nRoot);
	RET;

	LEFT_TAIL(nRoot);

	std::cout << "List Node 5 .. " << "\n";
	nRoot->Find(5)->Print();

	std::cout << "Right Count on Node 5: "
		<< nRoot->Find(5)->RCount() << "\n";

	std::cout << "Left Count on Node 5: "
		<< nRoot->Find(5)->LCount() << "\n";

	RET;

	std::cout << "List Node 3 .. " << "\n";
	nRoot->Find(3)->Print();


	std::cout << "Right Count on Node 3: "
		<< nRoot->Find(3)->RCount() << "\n";

	std::cout << "Left Count on Node 3: "
		<< nRoot->Find(3)->LCount() << "\n";

	RET;

	
	NULLP(nRoot);
	return 0;

}
