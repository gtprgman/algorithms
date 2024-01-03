#include <iostream>
#include "mixhuff.h"



int main() {
	std::vector<HF_REC> hfc;
	std::vector<node> nods,huffNods;
	Byte rootValue = 0;
	
	const char s[] = "Ada Ate Apple Lorem Ipsum Lorem Ipsum Lorem Ipsum Lorem Ipsum";

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
		//node ns = NODE_T(TO_FREQ_NODE(*PNODE<Byte>((Byte)c)) );
		nods.emplace_back(ANODE(c));

	}


	sort_Nodes<double>(nods, LEN);

	
	filter_Nodes(huffNods, nods);

	NPRINT(huffNods);


	RET;
	RET;
	HF_REC hcf;


	std::unique_ptr<node> ht = nullptr;
	ht.reset((CONST_PTR)huff_tree_create(huffNods, huffNods.size()));

	node* fh = ht.get(); // get raw pointer

	huffman_encode(hfc, fh);


	for (const HF_REC& hf : hfc) {
		RPRINT((char)hf._data); RPRINT(":") RPRINT(hf._bits); RPRINT("|");
		RET;
	}


	RET;
	RET;

	node::Dispose();

	return -1;	
};
