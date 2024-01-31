#include <iostream>
#include <map>
#include "mixhuff.h"


using BPAIR = std::pair<Bit, Byte>;


int main() {
	std::vector<HF_REC> hfc;
	std::vector<node*> nvt;
	std::vector<node> nods,huffNods;
	Byte rootValue = 0;
	
	const char s[] = "Ada Ate Apple.";

	const std::size_t LEN = strnlen_s(s, sizeof(s));

	const LongRange maxLen = (LongRange)(LEN - 1);

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


	if (nods.size() > 10)
		merge_sort<double>(nods, (LongRange)nods.size());
	else
		range_sort<double>(nods, (LongRange)0, maxLen);

	sort_Nodes<double>(nods, LEN-1);


	filter_Nodes(huffNods, nods);

	NPRINT(huffNods);

	
	RET;
	RET;
	
	
	std::unique_ptr<node> ht = nullptr;
	ht.reset((CONST_PTR)huff_tree_create(nvt, huffNods, huffNods.size()));

	node* fh = ht.get(); // get raw pointer


	huffman_encode(hfc, fh);


	for (const HF_REC& hf : hfc) {
		RPRINT((char)hf._data); RPRINT(":"); RPRINT(hf.Bits()); RPRINT("|");
		RET;
	}

	RET;
	RET;

	
	std::map<Bit, Byte> mbp;

	for (const HF_REC& hf : hfc) {
		Bit x = 0b1, xc = 0b0;

		for (std::size_t j = 0; j < hf._bits.size(); j++) {
			x &= (Bit)hf._bits[j];
			x <<= (7 - j);
			xc |= x;
			x = 0b1;
		}
		mbp.emplace(std::pair<Bit, Byte>(xc, hf._data));
	}


	for (const BPAIR& cp : mbp) {
		printf("% d ,", cp.first);
		printf("% c ,", cp.second);
		RET;
	}


	RET;
	RET;

	
	node::Dispose();
	return -1;
};
