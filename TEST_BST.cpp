// TEST_BST.cpp : This file contains the 'main' function. Program execution begins and ends there.

/* In Visual Studio 2022:
	- Choose 'Release' in the Solution Configurations Combo Box on the I.D.E.
	- To run this program, Click the [Debug] menu and choose 'Start Without Debugging'
		or simply press [Ctrl] + [F5]
	
	In Code Blocks that uses mingw64 Compiler:
	- The program runs well without any problems.
   
*/
#include <iostream>
#include "\MIXUTIL\mixtree.h"


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

	nRoot->Add(ALLOC_N(17));
	nRoot->Add(ALLOC_N(28));

	nRoot->Add(ALLOC_N(2));
	nRoot->Add(ALLOC_N(11));
	nRoot->Add(ALLOC_N(5));

	nRoot->Add(ALLOC_N(16));
	nRoot->Add(ALLOC_N(35)); 
	nRoot->Add(ALLOC_N(52));	//L_TURNS() happens after this one

	L_HEIGHT(nRoot);
	R_HEIGHT(nRoot);

	BNode::Collect();
	BNode::Dispose();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
