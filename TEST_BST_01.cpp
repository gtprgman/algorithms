// TEST_BST.cpp : This file contains the 'main' function. Program execution begins and ends there.

/* In Visual Studio 2022:
	- Choose 'Release' in the Solution Configurations Combo Box on the I.D.E.
	- To run this program, Click on the [Debug] menu and choose 'Start Without Debugging'
		or simply press [Ctrl] + [F5]
	
	In Code Blocks that uses mingw64 Compiler:
	- The program runs well without any problems.
   
*/
#include <iostream>
#include "\MIXUTIL\mixtreeX.h"

/* 
	BNode<T>; Where T is the type of the data member stored in a particular Node.
	Each Node Object of BNode<T> associated with a specific int value, this is to ease the
	sorting and searching functions perform on a particular dataset that based its key on
	the tree nodes'values. The construction of BNode of any type T only require the primary 
	initiation of the key value as argument to the BNode's constructor. We can then later 
	associate the data object of any type T to the BNode storage by calling the 'Set_Data()' 
	method of BNode.

*/




int main()
{
	PNODE<CTEXT> nRoot = ALLOC_N<CTEXT>(15);
	BNode<CTEXT>::setTopRoot(&nRoot);

	
	std::cout << "Adding up.. " << "\n\n";
	
	nRoot->Add(ALLOC_N<CTEXT>(9));
	nRoot->Add(ALLOC_N<CTEXT>(23));
	nRoot->Add(ALLOC_N<CTEXT>(4));
	nRoot->Add(ALLOC_N<CTEXT>(12));
	nRoot->Add(ALLOC_N<CTEXT>(8));
	nRoot->Add(ALLOC_N<CTEXT>(3)); // R_TURNS() CALLED AFTER THIS ONE
	
	//nRoot = BNode<CTEXT>::T_ROOT();

	/* the root should be reassigned a primary root,
		even though the prior call to 'setTopRoot()' has
		been initiated. Maybe the templated instance of a
		pointer to BNode (BNode<T>*) is destroyed when the 
	    'setTopRoot()' returns.
	*/

	nRoot->T_ROOT()->Print();

	

	RET;

	L_HEIGHT(nRoot->T_ROOT());
	R_HEIGHT(nRoot->T_ROOT());

	
	nRoot->Add(ALLOC_N<CTEXT>(17));
	nRoot->Add(ALLOC_N<CTEXT>(28));

	nRoot->Add(ALLOC_N<CTEXT>(2));
	nRoot->Add(ALLOC_N<CTEXT>(11));
	nRoot->Add(ALLOC_N<CTEXT>(5));

	nRoot->Add(ALLOC_N<CTEXT>(16));
	nRoot->Add(ALLOC_N<CTEXT>(35)); //L_TURNS() happens after this one
	nRoot->Add(ALLOC_N<CTEXT>(52));

	nRoot->T_ROOT()->Print();

	RET;

	L_HEIGHT(nRoot->T_ROOT());
	R_HEIGHT(nRoot->T_ROOT());
	

	BNode<CTEXT>::Collect();
	BNode<CTEXT>::Dispose();
};


