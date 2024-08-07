#ifndef REQUIRE_H
	#include "D:\MIXUTIL\User\Libs\mixutil.h"
#endif

#include "D:\MIXUTIL\User\Libs\mixhuff.h"




int main() {
	
	std::string s = "Ada Ate Apple.";
	const std::size_t SZ = s.size();
	std::vector<node> nod, fNod;
	std::vector<BPAIR> vtPair;

	for (const char& c : s)
		nod.push_back(c);

	sort_Nodes<LINT>(nod, SZ);

	filter_Nodes(fNod, nod); nod.clear();

	sort_Nodes<double>(fNod, fNod.size());

	build_huffman_tree(fNod);

	_TREE::Info(); RET;


	build_huffman_code(vtPair,_TREE::_Root);

	for (const BPAIR& bp : vtPair)
	{
		RPRINT((char)bp._data); RPRINT("="); RPRINT(biXs.toBits(bp._val).data() );
		RET;
	}
	

	RET2();
	return -1;
}

