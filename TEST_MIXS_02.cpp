#include "mixutil.h"
#include "mixer.h"


using P_NODE = BNode*;


int main()
{
	P_NODE nTemp = nullptr;

	P_NODE nRoot = ALLOC_N("Tom");

	setTopRoot(nRoot);

	nRoot->Add(ALLOC_N("Jade"));
	nRoot->Add(ALLOC_N("Ulfha"));

	nRoot->Print();

	RET;

	nTemp = treeAdd(nRoot, "Mitchel");

	nTemp->Print();

	RET;

	nTemp = treeAdd(nRoot,"Irene");

	nTemp->Print();

	RET;
	TALL;
	RET;

	nTemp = traverseNodes(nRoot, "Ulfha");

	nTemp->Print();
	RET;

	nTemp->Insert(ALLOC_N("Wendy"));

	BNode::recent()->Print(); RET;

	BNode::recent()->Parent()->Print(); RET;

	TALL;
	RET;

	nTemp = traverseNodes(nRoot, "Mitchel");
	nTemp->Remove();

	nTemp->Print();

	RET;
	TALL;

	nTemp = traverseNodes(nRoot, "Jade");

	nTemp->Print();

	RET;

	nRoot->Remove();

	nRoot->Print();

	nTemp = nRoot->Left();

	nTemp->Print();

	RET;
	TALL;
	RET;

	NULL2P(nRoot, nTemp);
	
}
