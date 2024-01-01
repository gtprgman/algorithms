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

	sort_Nodes<Byte>(nods, LEN);

	NPRINT(nods);
	RET;
	RET;

	// Demonstrates 'ANODE()' and 'TO_NODE()'
	PRINT( search_Node<Byte>(nods, ANODE('e')) ); RET;

	PRINT( vector_search<Byte>(nods, TO_NODE((Byte)'t')) ); RET;
	
	

	
	RET;
	RET;
	node::Dispose();

	return -1;	
};
