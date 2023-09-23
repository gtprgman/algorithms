// TEST_BST.cpp : This file contains the 'main' function. Program execution begins and ends there.

/* In Visual Studio 2022:
	- Choose 'Release' in the Solution Configurations Combo Box on the I.D.E.
	- To run this program, Click on the [Debug] menu and choose 'Start Without Debugging'
		or simply press [Ctrl] + [F5]
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

	PRINT("Adding up nodes..");

	nRoot->Add(ALLOC_N<CTEXT>(9));
	nRoot->Add(ALLOC_N<CTEXT>(23));
	nRoot->Add(ALLOC_N<CTEXT>(4));
	nRoot->Add(ALLOC_N<CTEXT>(12));
	nRoot->Add(ALLOC_N<CTEXT>(8));
	nRoot->Add(ALLOC_N<CTEXT>(3)); // R_TURNS() CALLED AFTER THIS ONE
	
	PRINT("R_TURNS() Executed.");
	// Try to find, modify or delete any nodes in the tree to assert the R_TURNS().

	
	PRINT("Root INFO:");
	nRoot->Print();

	RET;

	L_HEIGHT(nRoot);
	R_HEIGHT(nRoot);
	RET;

	// -------------------------------------

	PRINT("Adding next sequence of nodes..");
	nRoot->Add(ALLOC_N<CTEXT>(17));
	nRoot->Add(ALLOC_N<CTEXT>(28));

	nRoot->Add(ALLOC_N<CTEXT>(2));
	nRoot->Add(ALLOC_N<CTEXT>(11));
	nRoot->Add(ALLOC_N<CTEXT>(5));

	nRoot->Add(ALLOC_N<CTEXT>(16));
	nRoot->Add(ALLOC_N<CTEXT>(35));
	nRoot->Add(ALLOC_N<CTEXT>(52)); //L_TURNS() happens after this one
	
	PRINT("L_TURNS() Executed.");

	PRINT("Root INFO:");
	nRoot->Print();

	RET;

	L_HEIGHT(nRoot);
	R_HEIGHT(nRoot);
	
	
	BNode<CTEXT>::Collect();
	BNode<CTEXT>::Dispose();

	// ---------------

	PRINT("TESTING THE GARBAGE COLLECTOR..");
	RET;

	nRoot->Print();

	L_HEIGHT(nRoot);
	R_HEIGHT(nRoot);
	RET;

};


