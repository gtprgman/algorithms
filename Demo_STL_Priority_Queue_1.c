#include "C:\PROJECTS\MIXUTIL\Libs\mixhuff.h"
#include "C:\PROJECTS\MIXUTIL\Libs\mixhuff_impls.h"


int main()
{	
	std::string _Text = "Ada Ate Apple";
	std::vector<node> v_nodes;
	mix::generic::STL_Priority_Queue<node, std::vector<node>> pqn;

	for (auto&& _t : _Text) v_nodes.push_back(_t);
	for (auto&& _e : v_nodes) pqn.push(node(_e) ); 

	pqn.update_queue();

	for (; !pqn.empty();)
	{
		RPRINTC(pqn.top());
		pqn.pop();
	}


	v_nodes = {};

	return 0;
}

