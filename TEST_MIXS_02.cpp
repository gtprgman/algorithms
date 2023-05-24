
#include "mixutil.h"
#include "mixer.h"


using P_NODE = BNode*;


int main()
{
	P_NODE nd0 = ALLOC_N("Tom");

	setTopRoot(nd0);

	nd0->Add(ALLOC_N("Jade"));
	nd0->Add(ALLOC_N("Kaleen"));

	nd0->Print();

	RET;

	BNode::recent()->Print();

	RET;

	BNode::recent()->Parent()->Print();
	
	RET;
	TALL;
	RET;

	P_NODE ndKal = traverseNodes(nd0, "Kaleen");

	ndKal->Insert(ALLOC_N("Michael"));

	BNode::recent()->Print();

	RET;


	BNode::recent()->Parent()->Print();

	RET;
	TALL;
	RET;

	NULLP(ndKal);

	P_NODE ndNew = treeAdd(nd0, "Gerry");

	ndNew->Print();

	RET;

	BNode::recent()->Print();

	RET;
	TALL;
	RET;

}
