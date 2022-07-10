#include "mixutil.h"





int main()
{
	Alloc_Share<int> baseShr = SHARED_CREATE<int>(3);
	SHARED_ARRAY_TYPE<int> pshr = baseShr(3); // calls the functor

	SHARED_INIT<3,int>(pshr, 0, 100, 200, 300);
	

	FOR_EACH(&pshr[0], &pshr[3], [](auto const& ptr)->decltype(void()) {
		std::cout << ptr << ", " << std::endl;
	}
	);
	


	system("PAUSE");
	return 0;
}
