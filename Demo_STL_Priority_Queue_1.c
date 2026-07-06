#include "C:\PROJECTS\MIXUTIL\Libs\mixhuff.h"




int main()
{

	mix::generic::STL_Priority_Queue<int, std::vector<int>, std::greater<int>> mxq;

	mxq.push(30); mxq.push(25); mxq.push(26);


	mxq.update_heap();


	for (; !mxq.empty(); ) PRINT(mxq.pop());

	mxq.dispose_off();

	


	return 0;
}
