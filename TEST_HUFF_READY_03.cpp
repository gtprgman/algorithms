#ifndef REQUIRE_H
	#include "mixutil.h"
#endif

#include "mixhuff.h"



int main() 
{
	
	std::string s = "She Sells Shells in the Sea Shore.";
	const std::size_t SZ = s.size();
	std::priority_queue<node> pq;
	std::vector<node> fuNod;
	std::map<int, char> mPair;
	std::vector<bitInfo<UINT>> bfo;
	std::vector<UINT> packed;


	for (const auto& e : s)
		pq.push(e);
	

	for (; !pq.empty(); )
	{
		filter_pq_nodes(fuNod, (NODE_T)pq.top(),pq.size());
		pq.pop();
	}


	std::priority_queue<node, std::vector<node>,
		fqLess<node>> fpq{ fuNod.begin(), fuNod.end() };

	RET;

	fuNod.clear();

	for (NODE_T _nf = 0; !fpq.empty(); )
	{
		_nf = fpq.top();
		fuNod.push_back(_nf);
		fpq.pop();
	}


	//NPRINT(fuNod);

	RET;

	_TREE::build_huffman_tree(fuNod);

	_TREE::encode_tree(mPair, _TREE::_Root);


	for (const std::pair<int, char>& mp : mPair)
	{
		//RPRINT(mp.second); RPRINT("->"); RPRINT(to_binary<int>::eval(mp.first).data());
		//RET;
		bfo.push_back({ mp.first, oneAdder(num_of_bits<UINT>::eval(mp.first))});

	}
	
	
	bitsPack(packed, bfo);

	for (const auto& bi : packed)
	{
		PRINT(to_binary<UINT>::eval(bi).data());
	}
	

	fuNod.clear();
	mPair.clear();

	bfo.clear();
	packed.clear();


	RET2();
	return -1;
}

