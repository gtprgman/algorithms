
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

	return -1;
}

