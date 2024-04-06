#include <iostream>
#include "D:\MIXUTIL\User\Libs\mixhuff.h"

int main() {
	
	node* ft = (CONST_PTR)ALLOC_N<double>(50.00);
	node::setRoot(ROOT2P(&ft));
	node::setSize(3);

	if (!vNods.empty()) vNods.clear();

	ft->Add(NODE_T(65, 21.12));
	ft->Add(NODE_T(112, 14.2857));
	ft->Add(NODE_T(107, 65.0119));


	for (const auto& e : vNods) {
		RPRINT(e.dataValue()); RPRINT("|"); RPRINT(e.FrequencyData());
		RET;
	}

	RET2();
	node::Dispose();

	return -1;
}
