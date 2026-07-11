#include "C:\PROJECTS\MIXUTIL\Libs\mixhuff.h"
#include "C:\PROJECTS\MIXUTIL\Libs\mixhuff_impls.h"


int main()
{
	std::vector<int> v = { 18,97,113,49,198,1 };
	mix::generic::STL_Priority_Queue<int, std::vector<int>> vxq;

	for (auto&& _i : v) vxq.push(int(_i));

	vxq.update_heap();

	for (; !vxq.empty(); ) PRINT(vxq.pop());

	vxq.dispose_off();


	return 0;
}
