#include <iostream>
#include "mixhuff.h"



int main() {
	std::vector<node> freqNod, readyNods;

	const char s[] = "Ada Ate Apple";

	// LEN(s + '/0')
	const std::size_t LEN = std::strlen(s);
	Byte cnt = (Byte)total_values(s, LEN) / 2;

	node* data_t = (CONST_PTR)ALLOC_N<Byte>(cnt);
	node::setRoot(ROOT2P(&data_t));
	node::setSize(LEN);

	// adding data to the tree
	for (const auto& c : s)
		data_t->Add((Byte)c);

	// collecting nodes from the tree
	for (const auto& c : s) {
		node nx = *PNODE<Byte>((Byte)c);
		freqNod.emplace_back(TO_FREQ_NODE(node(nx)));
	}

	// sorting the nodes
	nodesSort<Byte>(freqNod, LEN);

	// filter the nodes to the separate container
	for (const node& e : freqNod)
		add_FrequencyNodes(readyNods, e);

	/*
	// Printing the ready nodes
	for (const node& e : readyNods) {
		RPRINT(e.dataValue()); RPRINT(e.FrequencyData());
		RET;
	}
	*/
	RET;
	const std::size_t fLEN = readyNods.size();

	std::unique_ptr<node, N_DELETER> ht = nullptr;
	ht.reset( (CONST_PTR)huff_tree_create(readyNods, fLEN) );

	node* f0 = ht.get();

	std::vector<HF_REC> huff;

	huffman_encode(huff, f0);


	for (const HF_REC& _hf : huff)
	{
		RPRINT((char)_hf._data); RPRINT(_hf._bits);
	}

	RET;

	
	
	node::Dispose();

	return -1;
};
