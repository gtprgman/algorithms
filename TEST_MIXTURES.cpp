#include "mixutil.h"


int main()
{
	Alloc_Share<int> baseShr = SHARED_CREATE<int>(3);
	SHARED_ARRAY<int> pshr = baseShr(3); // calls the functor

	std::initializer_list<int> lsn{ 150,250,350 };

	SHARED_INIT<int,3>(pshr, 0, 100, 200, 300);

	smart_print(&pshr[0], &pshr[3]);
	
	SHARED_INIT<int>(pshr, lsn);

	std::cout << "\n";


	smart_print(&pshr[0], &pshr[3]);

	system("PAUSE");
	return 0;
}

