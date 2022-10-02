#include "mixutil.h"



template < class Ty >
struct unique_del {

};

template < class Ty >
struct unique_del<Ty*>
{
	bool operator()(Ty* _pty) {
		static_assert(sizeof(_pty) > 0, "invalid pointer argument '_pty' ");
		delete[] _pty;
		return(nullptr == _pty);
	}
};








int main()
{
	// TESTING UNIQUE_ARRAY<int> ...

	std::cout << "Testing UNIQUE_ARRAY<int> ... " << std::endl;

	unique_array_ptr<int, unique_del<int*>> uArrayFactory;

	UNIQUE_ARRAY<int, unique_del<int*>>&& upArray = uArrayFactory.create(3);

	upArray = uArrayFactory.initialize({ 100,200,300 });

	smart_print(upArray.get(), upArray.get() + 3);

	std::cout << "\n\n";

	// TESTING SHARED_ARRAY<int> ...

	std::cout << "Testing SHARED_ARRAY<int> ... " << std::endl;

	Alloc_Share<int> sharedFactory(3);

	SHARED_ARRAY<int> shr = sharedFactory.get_shared();

	SHARED_INIT(shr, { 200,450,650 });

	smart_print(shr.get(), shr.get() + 3);


	system("PAUSE");
	return 0;
	
}

