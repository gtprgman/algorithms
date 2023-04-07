#include "mixutil.h"
#include "mixer.h"




int main() {
	
	shareP<int> sn0 = sp_create<int>(0);

	std::cout << *sn0 << "\n\n";

	shareP<int> sn100 = sp_create<int>(100);

	std::cout << *sn100 << "\n\n";

	shareP<Bucket> sb0 = sp_create<Bucket>("One");

	std::cout << sb0->data() << "\n\n";

	uniqueP<Bucket> ub2 = up_create<Bucket>("Two");

	std::cout << ub2->data() << "\n\n";

	uniqueP<Bucket> ub0 = up_create<Bucket>("");

	std::cout << ub0->data() << "\n\n";


	return 0;
}


