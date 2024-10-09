#ifndef REQUIRE_H
	#include "mixutil.h"
#endif

#include "mixhuff.h"



int main()
{
	std::string s = "Ada Ate Apple.";
	const std::size_t SZ = s.size();
	std::priority_queue<node> pq;
	std::vector<node> fuNod;
	std::vector<bitInfo<UINT>> bfo;
	std::vector<UINT> packed;
	std::map<int, char> codeMap;

	for (const auto& e : s)
		pq.push(e);


	for (; !pq.empty(); )
	{
		filter_pq_nodes(fuNod, (NODE_T)pq.top(), pq.size());
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

	_TREE::plot_huffman_tree(fuNod);
	
	fuNod.clear();
	codeMap.clear();

	bfo.clear();
	packed.clear();


	RET2();
	return -1;
}
