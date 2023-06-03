#include <iostream>
#include <string>
#include "mixtree.h"

using P_NODE = BNode*;


int main()
{
	P_NODE nRoot = ALLOC_N(15);
	P_NODE nTemp = nullptr;
	
	setTopRoot(nRoot); // this is a 'must' for BNode::Find()

	nRoot->Add(ALLOC_N(9));
	nRoot->Add(ALLOC_N(23));
	nRoot->Print();

	BNode::Find(23)->Print();

	nRoot->Add(ALLOC_N(17));
	nRoot->Add(ALLOC_N(28));

	BNode::Find(18)->Print();

	nRoot->Right()->Print();

	nRoot->Add(ALLOC_N(3));
	nRoot->Add(ALLOC_N(12));

	nRoot->Left()->Print();
	
	NULL2P(nTemp, nRoot);
	
}
