
#include "mixutil.h"
#include "mixer.h"


using P_NODE = BNode*;


int main()
{
	P_NODE ndRoot = ALLOC_N("Wendy");
	setTopRoot(ndRoot);

	std::cout << "Wendy->Add(Tom)" << "\n";
	std::cout << "Wendy->Add(Jade)" << "\n";
	RET;

	ndRoot->Add(ALLOC_N("Tom"));
	ndRoot->Add(ALLOC_N("Jade"));

	std::cout << "Root->Print()" << "\n";

	ndRoot->Print(); RET;  TALL; RET;

	std::cout << "Searching Tom... " << "\n";
	RET;

	P_NODE ndTom = traverseNodes(ndRoot, "Tom");

	ndTom->Print(); RET;

	std::cout << "Delete 'Jade' ... " << "\n";
	RET;

	ndTom->Left()->Remove();

	std::cout << "Try printing the deleted Node..." << "\n";
	RET;

	ndTom->Left()->Print();

	std::cout << "Wendy->Tom->Print(); " << "\n";
	RET;

	ndRoot->Left()->Print();
	
	TALL;
}
