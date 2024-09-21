#ifndef REQUIRE_H
	#include "mixutil.h"
#endif

#include "mixhuff.h"



int main() 
{
	
	std::string s = "Ada Ate Apple.";
	const std::size_t SZ = s.size();
	std::priority_queue<node> pq;
	std::vector<node> fNod;

	for (const auto& e : s)
		pq.push(e);
	

	for (; !pq.empty(); )
	{
		filter_pq_nodes(fNod, (NODE_T)pq.top(),pq.size());
		pq.pop();
	}

	std::priority_queue<node, std::vector<node>,
		freqLess<node>> fpq{ fNod.begin(), fNod.end() };


	fNod.clear();

	for (NODE_T _nf = 0; !fpq.empty(); )
	{
		_nf = fpq.top();
		fNod.push_back(_nf);
		fpq.pop();
	}

	RET;

	_TREE::build_huffman_tree(fNod);

	_TREE::ENCODE_SCHEMA = true;
	_TREE::plot_huffman_tree(_TREE::_Root);

	
	fNod.clear();

	RET2();
	return -1;
}

