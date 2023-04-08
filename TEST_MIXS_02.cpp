#include "mixutil.h"
#include "mixer.h"




int main() {
	
	shareP<char> ch97 = sp_create<char>('A');

	std::cout << *ch97 << "\n\n";

	shareP<double> d0 = sp_create<double>();

	std::cout << *d0 << "\n\n";

	shareP<int> un = sp_create<int>(-1,2);

	std::cout << *un << "\n\n";

	shareP<double> sdb = sp_create<double>(2.93);

	std::cout << *sdb << "\n\n";

	shareP<Bucket> sb0 = sp_create<Bucket>("One");

	std::cout << sb0->data() << "\n\n";

	shareP<Bucket> sb2 = sp_create<Bucket>("Two");

	std::cout << sb2->data() << "\n\n";

	shareP<Bucket> sb01 = up_create<Bucket>();

	std::cout << sb01->data() << "\n\n";

	
}


