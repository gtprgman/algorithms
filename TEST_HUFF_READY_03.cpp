#ifndef REQUIRE_H
	#include "mixutil.h"
#endif

#include "mixhuff.h"



int main() 
{
	
	std::string s = "Ada Ate Apple.";
	const std::size_t SZ = s.size();
	std::vector<node> nod, fNod;
	std::vector<bitInfo<unsigned int> > bfo;
	std::map<int, char> mPair;
	

	for (const auto& c : s)
		nod.push_back(c);

	
	sort_Nodes<int>(nod, SZ);

	filter_Nodes(fNod, nod); 
	
	nod.clear();

	sort_Nodes<double>(fNod, fNod.size());

	//NPRINT(fNod); RET;

	_TREE::build_huffman_tree(fNod);
	

	RET;

	/*
	 Debugging purposes ... 
	 the function below should be execute independently
	 without precede or after call to '_TREE::encode_tree()' 
	*/

/*
	_TREE::ENCODE_SCHEMA = true;
	_TREE::plot_huffman_tree(_TREE::_Root);
*/

	RET;
	
	_TREE::ENCODE_SCHEMA = false;
	_TREE::encode_tree(mPair, _TREE::_Root);

	RET;


	for (const std::pair<int, char>& mp : mPair)
	{
		RPRINT(mp.second); RPRINT("->"); RPRINT(biXs.toBits(mp.first).data());
		//bfo.push_back({ mp.first,num_of_bits<unsigned int>::eval(mp.first) + 1 });

		RET;

	}

	RET;
	
	
	const UINT bfPacked = bitsPack<UINT>(bfo);

	RPRINT("Packed Integer Value: "); RPRINT(bfPacked); RET2();

	RPRINT("Packed Size: ");  RPRINT(sizeof(bfPacked)); RET2();

	PRINT(to_binary<UINT>::eval(bfPacked));

	RET;

	RET2();


	fNod.clear();
	mPair.clear();
	bfo.clear();

	return -1;
}

