#ifndef REQUIRE_H
	#include "mixutil.h"
#endif

#include "mixhuff.h"



int main() {
	std::string s = "Ada Ate Apple.";
	const std::size_t SZ = s.size();
	std::vector<node> nod, fNod;

	for (const int i : s)
		nod.push_back(NODE_T(i));

	sort_Nodes<Byte>(nod, SZ);
	
	filter_Nodes(fNod, nod);

	nod.clear();

	RET2();

	sort_Nodes<double>(fNod, fNod.size());

	build_huffman_tree(fNod);

	_TREE::_Root->Print();
	_TREE::Info();


	RET2();


	fNod.clear();
	
	

	return -1;
}
