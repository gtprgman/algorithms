
#ifndef REQUIRE_H
	#include "mixutil.h"
	
#endif

#include "mixhuff.h"


int main()
{
	cHash<cElem<node*>> hs(3);
	node* nods = nullptr;
	

	hs = cElem<node*>(new node((LongRange)'I'));
	hs = cElem<node*>(new node((LongRange)'a'));
	hs = cElem<node*>(new node((LongRange)'m'));

	// try the elements number of 0, 1, 2
	nods = hs.elements(2).DataC;

	PRINT( hs.get((LongRange)nods)->dataValue() );



	return -1;
}
