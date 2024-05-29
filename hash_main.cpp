
#ifndef REQUIRE_H
	#include "mixutil.h"
#endif

#include "mixhuff.h"





int main(int argc, const char* argv[]) 
{
	std::string s = "She Sells Shells in the Sea Shore.";
	const std::size_t SZ = s.size();
	cHash<cElem<NODE_T>> hs(SZ);

	for (const auto& c : s)
		hs = cElem<NODE_T>(c);

	for (const int i : s)
		RPRINT((char)hs.get(i)._v);

	RET2();

	hs.Sweep(); // Delete all elements
	
	for (const int i : s)
		RPRINT((char)hs.get(i)._v);
		
	
	RET;
	
	// define new data elements, the size previously determined in '_SZ' is not changed.
	
	hs = cElem<NODE_T>('H');
	hs = cElem<NODE_T>('e');
	hs = cElem<NODE_T>('l');
	hs = cElem<NODE_T>('l');
	hs = cElem<NODE_T>('o');

	hs = cElem<NODE_T>('Y');
	hs = cElem<NODE_T>('u');


	RPRINT((char)hs.get('H')._v);
	RPRINT((char)hs.get('i')._v);
	RPRINT((char)hs.get('Y')._v);
	RPRINT((char)hs.get('u')._v);
	RPRINT((char)hs.get('l')._v);


	
	RET2();

	return -1;
}
