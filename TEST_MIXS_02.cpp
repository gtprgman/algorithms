#include <iostream>
#include <string>
#include "mixtree.h"

using P_NODE = BNode*;


int main()
{
	P_NODE nRoot = ALLOC_N(15);
	P_NODE n1 = nullptr, n2 = nullptr;
	
	setTopRoot(nRoot);

	nRoot->Add(ALLOC_N(9));
	nRoot->Add(ALLOC_N(23));
	nRoot->Print();

	nRoot->Add(ALLOC_N(17));
	nRoot->Add(ALLOC_N(28));

	nRoot->Add(ALLOC_N(3));
	nRoot->Add(ALLOC_N(12));
	
	nRoot->Add(ALLOC_N(4));
	nRoot->Add(ALLOC_N(8));

	BNode::Find(9)->Print();

	std::cout << "Node 28 has childs? :" << HAS_CHILDS(BNode::Find(28)) << "\n\n";
	std::cout << "Node 4 has childs? :" << HAS_CHILDS(BNode::Find(4)) << "\n\n";

	TALL;
	RET;

	std::cout << "Removing Node 3.. " << "\n\n";

	BNode::Find(3)->Remove();
	BNode::Find(9)->Print();


	TALL;
	RET;

	BNode::Find(4)->Print();

	BNode::Find(23)->Print();

	std::cout << "Removing Node 28.. " << "\n\n";

	BNode::Find(28)->Remove();
	BNode::Find(23)->Print();


	TALL;
	RET;

	NULL3P(n1,n2, nRoot);
	
}
