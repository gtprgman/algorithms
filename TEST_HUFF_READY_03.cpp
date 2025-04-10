#ifndef REQUIRE_H
	#include "mixutil.h"
	#include "mixhuff.h"
#endif



/*
	The current stable version is Pointers-Free !!
	You can now run the program in Debugging Mode, without a fear of crash.
	Just press [F5] to run..
*/


int main()
{
	std::string s = "She Sells Shells in the Sea Shore.";
	const std::size_t SZ = s.size();
	std::priority_queue<node> pq;
	std::vector<node> fuNod;
	std::vector<bitInfo<int>> bfo;
	std::vector<int> packed;
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
		fqLess> fpq{ fuNod.begin(), fuNod.end() };

	RET;

	fuNod.clear();

	// top up the vector from the frequencies-sorted data
	for (node _nf = 0; !fpq.empty(); )
	{
		_nf = fpq.top();
		fuNod.push_back(_nf);
		fpq.pop();
	}

	//NPRINT(fuNod); RET; RET;

	_TREE::plot_tree(fuNod);

	vCodeMap = _TREE::CodeMap();

	bitInfo<int> _bi = {};

	// display the encoding table
	for (const BPAIR& _bp : vCodeMap)
	{
		RPRINT(_bp._data); RPRINT("->"); RPRINT(to_binary<int>::eval(_bp._val).data());
		RET;

		/*
			_bi.X = _bp._val;
			_bi.numBits = oneAdder(num_of_bits<int>::eval(_bp._val));

			bfo.push_back(_bi);
		*/
	}

/*
	bitsPack(packed, bfo);

	// display the packed encoding data

	RET;

	std::string _file = "D:\\DATA\\packed.sqz";

	PRINT("Saving packed data.."); RET;

	if (writePack(_file.data(), s, vCodeMap))
		PRINT("Packed Data Saved !!");

*/

	fuNod.clear();
	vCodeMap.clear();

	bfo.clear();
	packed.clear();

	_TREE::Clean();

	RET2();
	return -1;
}


