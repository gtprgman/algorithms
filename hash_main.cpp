#include <iostream>
#include "mixhuff.h"
#include "mixhash.h"



int main(int argc, const char* argv[]) {

	cHash<NODE_T> hs(5);

	hs = NODE_T(65);
	hs = NODE_T(66);
	hs = NODE_T(67);
	hs = NODE_T(68);
	hs = NODE_T(69);


	hs = NODE_T(97);
	hs = NODE_T(98);
	hs = NODE_T(99);


	for (std::size_t j = 0; j < 5; j++)
		PRINT(hs[j].DataC());

	

	RET2();

	return -1;
}