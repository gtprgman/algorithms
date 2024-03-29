#include <iostream>
#include "mixhuff.h"



int main() {
	std::string s = "Ada Ate Apple.";
	std::vector<NODE_T> nods;
	std::vector<node> bulk, refine;

	for (const auto& x : s)
		nods.push_back(x);

	transForm(bulk, nods);
	nods.clear();

	sort_Nodes<Byte>(bulk, bulk.size());

	filter_Nodes(refine, bulk);
	bulk.clear();

	for (const auto& e : refine) {
		RPRINT(e.FrequencyData()); RPRINT(" | ");
		RPRINT(e.dataValue());
		RET;
	};

	RET;

	RPRINT("Filtered length: "); RPRINT(refine.size()); RET;

	range_sort<double>(refine, 0, refine.size()-1);
	sort_Nodes<double>(refine, refine.size());

	RPRINT("Sorted Length:"); RPRINT(refine.size()); RET2();

	for (const auto& e : refine) {
		RPRINT(e.FrequencyData()); RPRINT(" | ");
		RPRINT(e.dataValue());
		RET;
	};
	RET2();

	huff_tree_create(refine, refine.size());


	_TREE::_Root->Print();

	LCOUNT(_TREE::_Root);
	RCOUNT(_TREE::_Root);
	
	
	refine.clear();
	node::Dispose();
	

	RET2();

	return -1;
}
