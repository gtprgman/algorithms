#include "mixutil.h"
#include "mixer.h"




int main() {

	
	// codes demo for unique pointers.
	unique_array_ptr<Bucket> ubFactory;

	UNIQUE_ARRAY<Bucket> upb = ubFactory.create(2);

	upb = ubFactory.initialize({ Bucket("u1"), Bucket("u2") });

	iPrinter<Bucket*>::_print_(upb.get(), upb.get() + 2);

	iList<Bucket> bList{ Bucket("b1"), Bucket("b2")};

	upb = ubFactory.initialize(bList);

	iPrinter<Bucket*>::_print_(upb.get(), upb.get() + 2);


	// codes demo for shared pointers.

	std::cout << "\n\n";

	Alloc_Share<Bucket> shrFactory(2);

	SHARED_ARRAY<Bucket> spb = shrFactory.get_shared();

	iPrinter<Bucket*>::_print_(spb.get(), spb.get() + 2);

	spb = shrFactory.initialize(bList);

	iPrinter<Bucket*>::_print_(spb.get(), spb.get() + 2);



	system("PAUSE");
	return 0;
}
