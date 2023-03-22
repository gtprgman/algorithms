#include "mixutil.h"
#include "mixer.h"




int main() {

	shareP<int> sp = _MAKE_S<int>(100);

	std::cout << *sp << "\n\n";

	shareP<Bucket> bp = _MAKE_S<Bucket>();

	std::cout << bp->data() << "\n\n";


	shareP<Bucket> bpa = _MAKE_S<Bucket>("Hello World..");

	std::cout << bpa->data() << "\n\n";

	int n = _Instantiator<int>::_Construct(100);
	
	std::cout << n << "\n\n";

	 
	n = _FORWRD<int>(100);

	int kn = _frWard<int>::_construct(250);

	std::cout << kn << "\n\n";


	
	return 0;
}


