#include "mixutil.h"



using namespace mix;
using namespace mix::ptr_type;
using namespace mix::smart_ptr;




// custom deleter for unique_array
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





// custom deleter for shared array
template < class Ty >
struct del_shared_array {};

template < class Ty >
struct del_shared_array<Ty*> {
	bool operator()(Ty* _pty) {
		static_assert(sizeof(_pty) > 0, "invalid argument '_pty' ");
		delete[] _pty;
		return(nullptr == _pty);
	}
};



int main()
{
	// TESTING UNIQUE_ARRAY<int> ...

	std::cout << "Testing UNIQUE_ARRAY<int> ... " << std::endl;

	unique_array_ptr<int,unique_del<int*>> uArrayFactory;

	UNIQUE_ARRAY<int,unique_del<int*>>&& upArray = uArrayFactory.create(3);

	upArray = uArrayFactory.initialize({ 100,200,300 });

	smart_print(upArray.get(), upArray.get() + 3);

	std::cout << "\n\n";

	// TESTING SHARED_ARRAY<int> ...

	std::cout << "Testing SHARED_ARRAY<int> ... " << std::endl;

	Alloc_Share<int, del_shared_array<int*> > sharedFactory(3);

	SHARED_ARRAY<int> shrP = sharedFactory.get_shared();

	shrP = sharedFactory.initialize({ 200,450,650 });

	smart_print(shrP.get(), shrP.get() + 3);


	system("PAUSE");
	return 0;
}


