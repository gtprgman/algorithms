#ifndef REQUIRE_H
	#include "mixutil.h"
#endif

#include "mixhuff.h"


int main() {
	
	std::string s = "Ada Ate Apple.";
	const std::size_t SZ = s.size();
	std::vector<node> nod, fNod;

	for (const char& c : s)
		nod.push_back(c);

	sort_Nodes<LINT>(nod, SZ);

	filter_Nodes(fNod, nod); nod.clear();

	sort_Nodes<double>(fNod, fNod.size());

	build_huffman_tree(fNod);

	_TREE::_Root->Print();
	_TREE::Info();

	PRINT( _Map.find('e')->second->dataValue() );
	PRINT(_Map.find('e')->second->FrequencyData());


	RET2();
	return -1;
}
