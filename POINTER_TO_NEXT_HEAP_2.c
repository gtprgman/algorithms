#include "C:\PROJECTS\MIXUTIL\Libs\mixhuff.h"
#include "C:\PROJECTS\MIXUTIL\Libs\mixhuff_impls.h"




int main()
{	/*
	  Below demonstrating pointer to next address of heap block as basically we often implemented it in 
	  the linked-list data structure. But this time, we draw the same concept on the bare heap blocks
	  underlying the physical R.A.M.
	*/

	int* p = new int[2] {65, 66};
	int* p_max = p + 2;
	int* p_next = new(p_max) int(67);

	p_next = new(p_next + 1) int(NULL);

	
	for (int* _pi = p; _pi < p_next; _pi++)
	{
		std::cout << _pi << " " <<  *_pi << "\n";
	}

	//delete[3] p, p_max, p_next;
	
	return 0;
}
