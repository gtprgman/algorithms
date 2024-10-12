#ifndef REQUIRE_H
	#include "mixutil.h"
#endif

#include "mixhuff.h"


/*
	The current stable version is Pointers-Free !!
	You can now run the program in Debugging Mode, without a fear of crash.
	Just press [F5] to run..
*/


int main()
{
	std::string s = "Hello World You.";
	const std::size_t SZ = s.size();
	std::priority_queue<node> pq;
	std::vector<node> fuNod;
	std::vector<bitInfo<UINT>> bfo;
	std::vector<UINT> packed;
	std::vector<BPAIR> vCodeMap;
	BPAIR _bpt = {};


	// Collect raw data from input sources
	for (const auto& e : s)
		pq.push(e);

	// filter the data and classifying them based on frequency of appearances
	for (; !pq.empty(); )
	{
		filter_pq_nodes(fuNod, (node&&)pq.top(), pq.size());
		pq.pop();
	}

	
	RET;

	// sort the data based on frequencies
	std::priority_queue<node, std::vector<node>,
		fqLess<node>> fpq{ fuNod.begin(), fuNod.end() };

	RET;

	fuNod.clear();

	// top up the vector from the frequencies-sorted data
	for (node _nf = 0; !fpq.empty(); )
	{
		_nf = fpq.top();
		fuNod.push_back(_nf);
		fpq.pop();
	}

	
	_TREE::plot_tree(fuNod);

	vCodeMap = _TREE::CodeMap();

	// display the encoding table
	for (std::vector<BPAIR>::iterator _Vit = vCodeMap.begin(); _Vit != vCodeMap.end(); _Vit++)
	{
		//RPRINT(_Vit->_data); RPRINT("->"); RPRINT(to_binary<int>::eval(_Vit->_val).data());
		bfo.push_back({ _Vit->_val,
					  oneAdder(num_of_bits<UINT>::eval(_Vit->_val)) });
		RET;
	}

	bitsPack(packed, bfo);

	// display the packed encoding data
	for (const UINT ui : packed)
	{
		PRINT(to_binary<UINT>::eval(ui).data());;
	}

	
	fuNod.clear();
	vCodeMap.clear();
	
	bfo.clear();
	packed.clear();
	
	_TREE::Clean();

	RET2();
	return -1;

}
