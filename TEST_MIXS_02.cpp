#include "mixutil.h"
#include "mixer.h"




int main() {

	/*  ls2 is constructed by move, whereas argument to _MOVE is a rvalue of iList2
	created by its parameterized ctor */
	iList2<int> ls2 = _MOVE( iList2<int>( { 2,4,6,8 } ) );
	
	for (auto const& k : ls2) std::cout << k << ", ";

	std::cout << "\n\n";

	iList2<double> lsf = _MOVE(iList2<double>({ 3.14,2.78,11.15 }));

	for (auto const& l : lsf) std::cout << l << ", ";

	std::cout << "\n\n";

	iList<Bucket> lc{ Bucket("One"), Bucket("2"), Bucket() };

	// lb is instantiated by its parameterized ctor.
	iList2<Bucket> lb = lc;

	for (auto const& b : lb) std::cout << b.data() << " ,";

	std::cout << "\n\n";

	iList2<int> ln = std::array<int, 2>{1290, 3059};
	
	std::cout << "\n\n";

	for (auto const& kn : ln) std::cout << kn << ",";

	std::cout << "\n\n";
	
	double ffArr[] = {3.19, 0.998, 69.05};
	iList2<double> ffLs = ffArr;

	for (auto const& f : ffLs) std::cout << f << ", ";

	std::cout << "\n\n";

	
	
	return 0;
}


