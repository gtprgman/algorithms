#include <iostream>
#include "mixhuff.h"



int main() {
	std::string s = "She Sells Shells in the Sea Shore.";
	std::vector<NODE_T> nodT;
	std::vector<node> nods, ordered, treeNods;

	for (const auto& x : s)
		nodT.push_back(x);

	transForm(nods, nodT); nodT.clear();

	sort_Nodes<Byte>(nods,nods.size());

	filter_Nodes(ordered, nods); nods.clear();
	
	node* ft = (CONST_PTR)ALLOC_N(50.00);
	node::setRoot(ROOT2P(&ft));

	for (auto& e : ordered)
		ft->Add(std::forward<node>(e) );

	transForm2(treeNods); ordered.clear();

	//sort_Nodes<double>(treeNods, treeNods.size());

	sort_Nodes<Byte>(treeNods, treeNods.size());

	NPRINT(treeNods); RET;

	NODE_T nf = 't';
	node ndf;


	RPRINT("Found: ");  RPRINT(vector_search(treeNods, nf, ndf));
	RPRINT((char)ndf());
	
	

	RET2();

	treeNods.clear();
	node::Dispose();
	return -1;
}
