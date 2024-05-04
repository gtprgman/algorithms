#include <iostream>
#include "mixutil.h"
#include "mixhuff.h"
#include "mixhash.h"



int main(int argc, const char* argv[]) {
	cHash<cElem<NODE_T>> hs(10);
	

	for (int i = 0; i < 10; i++)
		hs = cElem<NODE_T>(65 + i);


	for (int j = 0; j < 10; j++)
		PRINT((char)hs.elements(j).DataC());



	RET2();

	return -1;
}
