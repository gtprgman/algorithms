#include "mixutil.h"
#include "mixer.h"




int main() {

	int Value = 55;
	
	_Instantiator<int,int> nInit;

	Value = nInit.Construct(250);

	std::cout << Value << "\n\n";

	
	uniqueP<int>&& uNum = _MAKE_U<int>(330);

	uniqueP<double> uFloat = _MAKE_U<double>(3.25);

	std::cout << *uNum << "\n\n";

	std::cout << *uFloat << "\n\n";

	uniqueP<Bucket> uBcket = _MAKE_U<Bucket>("Hello World..");

	std::cout << uBcket->data() << "\n\n";


	
	return 0;
}


