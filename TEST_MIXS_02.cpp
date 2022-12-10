#include "mixutil.h"
#include "mixer.h"




int main() {

	unique_array_ptr<int> uIntFact;
	Alloc_Share<Bucket> allocBuckets(3);


	UNIQUE_ARRAY<int> uaInt = uIntFact.create(3);
	uaInt = uIntFact.initialize({ 275, 345, 780 });

	SHARED_ARRAY<Bucket> saBucket = allocBuckets.get_shared();
	saBucket = allocBuckets.initialize({ Bucket("b0"), Bucket("b1"), Bucket("b2") });


	std::cout << "\n\n";

	smart_print(uaInt.get(), uaInt.get() + 3);

	std::cout << "\n\n";


	smart_print(saBucket.get(), saBucket.get() + 3);

	std::cout << "\n\n";
	


	system("PAUSE");
	return 0;
}
