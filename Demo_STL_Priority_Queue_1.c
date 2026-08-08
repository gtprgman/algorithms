#include "C:\PROJECTS\MIXUTIL\Libs\mixhuff.h"
#include "C:\PROJECTS\MIXUTIL\Libs\mixhuff_impls.h"


int main()
{	
	std::string _Text = "Ada Ate Apple";
	std::vector<node> v_nodes;
	mix::generic::STL_Priority_Queue<int, std::vector<int>> vpq;

	for (auto&& _s : _Text) v_nodes.push_back(_s);

	for (node _n : v_nodes) vpq.push(node(_n) );

	vpq.update_queue();

	for (; !vpq.empty(); ) RPRINTC(vpq.pop());

	v_nodes.clear();

	return 0;
}
