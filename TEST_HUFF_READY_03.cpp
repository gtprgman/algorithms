#include <iostream>
#include <cstdio>

#include "mixhuff.h"


/* Thinking how to reorganize data nodes in the tree, so we can
*  quiet easily sort the nodes based on its frequency and plot
*  these sorted nodes to a new tree. The new tree that has the
*  frequency as its key value could be used for encoding mechanism.
*/

/* NB:
	
	Every key value has a specific frequency that determines how often/
	how frequently a data node appears in the sequences of a dataset collected
	from a tree or reading from the file stored in an external media devices.

	For example, for the data value of Byte, there coupled with it a frequency data 
	member called _freq which denotes how frequently a node with that Byte value appears
	in the sequences. The same principal hold true for a frequency data value of type
	double.

	node.Value(); <--> node.Freq(); ( _data <--> _freq )
	node.FrequencyData(); <--> node.Freq(); ( _fdata <--> _freq )

*/
 

/* 

	UPDATE NOTICES per: 2023/12/09
	- Arrange the sorted frequency nodes to be added to the
		frequency tree.
	- The arrangement should composed of binary node of which the root
		will represent the sum of frequencies of the two nodes.

	- The sorting mechanism need to be heedfully reconcerned and redesigned
		from scratch.
*/

int main() {
	std::vector<node> dataNod;
	std::vector<node> freqNod;

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
	
	
	for (const auto& c : s)
		dataNod.emplace_back(node(*PNODE<Byte>((Byte)c)));

	for (const auto& e : dataNod)
		freqNod.emplace_back(node(TO_FREQ_NODE(e)));


	nodesSort<Byte>(freqNod, LEN); 

	for (const auto& e : freqNod) {
		RPRINT(e.dataValue());
		RPRINT(e.FrequencyData());
		RET;
	}

	RET;


	std::unique_ptr<node> ht = nullptr;

	ht.reset((CONST_PTR)huff_tree_create(freqNod, LEN));

	ht->Print();

	RET;
	node::Dispose();

	return -1;
};
