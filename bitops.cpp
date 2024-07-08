#ifndef REQUIRE_H
	#include "mixutil.h"
#endif

#include "mixbit.h"

	

int main()
{
	std::cout.precision(8);

	fixN<32> f1 = 7.0221;
	fixN<32> f2 = 3.0051;


	PRINT(f1()); RET;
	PRINT(f2()); RET;

	RPRINT(f1() + f2()); RET;
	
	RPRINT(f1() * f2()); RET;

	RET;
	PRINT(f1.rational()); RET;
	PRINT(f2.rational()); RET;

	RET;

	PRINT(f1.decimal()); RET;
	PRINT(f2.decimal()); RET;


	fixN<16> d = 33570; 

	PRINT(d(0)); RET;

	

	RET2();

	return -1;
}

