#include <iostream>
#include "mixhuff.h"



int main() {
	std::vector<node> nods,huffNods;
	Byte rootValue = 0;
	
	const char s[] = "Ada Ate Apple";
	const std::size_t LEN = strnlen_s(s, sizeof(s));

	rootValue = (Byte)total_values(s, LEN);

	node* data_t = (CONST_PTR)ALLOC_N<Byte>(rootValue/2);
	node::setRoot(ROOT2P(&data_t));
	node::setSize(LEN);

	
	// Plot data from any external source to the tree
	for (const auto& c : s)
		data_t->Add((Byte)c);


	// Collect data from the tree
	for (const auto& c : s) {
		node ns = NODE_T(TO_FREQ_NODE(*PNODE<Byte>((Byte)c)) );
		nods.emplace_back(ns);

	}

	sort_Nodes<double>(nods, LEN);

	NPRINT(nods);
	RET;
	RET;

	frequency_Filter(huffNods, nods);

	NPRINT(huffNods);

	
	RET;
	RET;
	node::Dispose();

	return -1;	
};
