
#ifndef REQUIRE_H
	#include "mixutil.h"
	
#endif

#include "mixhuff.h"


int main()
{
	cHash<cElem<node*>> hs(3);
	const LINT SZ = hs.size();
	U_ARRAY<LINT> ns = MK_U_ARRAY<LINT>((UINT)3);
	node* u = new node(LINT('D'));

	hs = cElem<node*>((node*)ALLOC_N(LINT('A')));
	hs = cElem<node*>((node*)ALLOC_N(LINT('B')));
	hs = cElem<node*>((node*)ALLOC_N(LINT('C')));
	hs = cElem<node*>(u);


	for (LINT i = 0; i < SZ; i++)
	{
		if (hs.elements(i).DataC == nullptr) continue;
		ns[i] = hs.elements(i).KeyC;
	}
	
	for (LINT j = 0; j < SZ; j++)
		RPRINT(hs.get(ns[j])->dataValue());


	// get an element beyond the maximum predefined slot
	PRINT(hs.get((LINT)u)->dataValue());

	delete u;

	RET;

	
	return -1;
}
