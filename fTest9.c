#ifndef REQUIRE_H
	#include "C:\PROJECTS\MIXUTIL\Libs\mixutil.h"
	#include "C:\PROJECTS\MIXUTIL\Libs\mixhuff.h"
#endif


int main()
{	
	std::vector<std::string> _vHex = { "76","E2","FB","1F","1E" };

	RPRINTC("DEC DIGIT: 118  226  251  31   30");RET;
	RPRINTC("HEX DIGIT: 76   E2   FB   1F   1E"); RET;
	RPRINTC("BINARY BITS.."); RET;

	for (const auto& _hx : _vHex)
		RPRINTC(To_HexF<intmax_t>::to_bit_str(_hx.c_str()));



	RET;

	vectorClean(_vHex);

	return 0;
}


	
