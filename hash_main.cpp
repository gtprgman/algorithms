#include <iostream>

#include "mixutil.h"
#include "mixhuff.h"
#include "mixhash.h"


 

int main(int argc, const char* argv[]) 
{
	cHash<cElem<NODE_T>> hs(5);

	for (int i = 0; i < 5; i++)
		hs = cElem<NODE_T>(65 + i);

	/* the following is adding a few number of extra elements
	   to the cHash beyond the predetermined size in '_M'
	*/

	hs = cElem<NODE_T>(70);
	hs = cElem<NODE_T>(71);
	hs = cElem<NODE_T>(72);

	for (int j = 0; j < 5; j++)
		PRINT(hs.elements(j).DataC());

	RET;
	
	
	PRINT(hs[70]._v);
	PRINT(hs[71]._v);
	PRINT(hs[72]._v);

	RET2();

	return -1;
}
