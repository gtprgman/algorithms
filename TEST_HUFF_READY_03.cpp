#ifndef REQUIRE_H
	#include "mixutil.h"
#endif

#include "mixhuff.h"




int main() {
	
	std::string s = "She Sells Shells in the Sea Shore.";
	const std::size_t SZ = s.size();
	std::vector<node> nod, fNod;
	std::map<int, char> mPair;
	

	for (const auto& c : s)
		nod.push_back(c);


	sort_Nodes<int>(nod, SZ);

	filter_Nodes(fNod, nod); 
	
	nod.clear();

	sort_Nodes<double>(fNod, fNod.size());

	//NPRINT(fNod);

	build_huffman_tree(fNod);

	_TREE::Info(); RET;

	build_huffman_code(mPair, _TREE::_Root);

	RET;


	for (const std::pair<int, char>& mp : mPair)
	{
		RPRINT(mp.second); RPRINT("->"); RPRINT(biXs.toBits(mp.first).data());
		RET;
	}

	fNod.clear(); 
	mPair.clear();


	RET;
	

	RET2();
	return -1;
}

