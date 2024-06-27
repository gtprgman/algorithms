#ifndef REQUIRE_H
	#include "mixutil.h"
#endif

#include "mixbit.h"

	

int main()
{
	BitN bitFun;
	
	PRINT("19 -> (bin) = ");
	PRINT(bitFun.to_signed_bits(19).data());
	RPRINT("Bit Range: "); RPRINT(MAX_BIT);
	RET2();
	RET;

	PRINT("-19 -> (bin) = ");
	PRINT(bitFun.to_signed_bits(-19).data());
	RPRINT("Bit Range: "); RPRINT(MAX_BIT);
	
	RET;
	RET2();

	
	PRINT("25 -> (bin) = ");
	PRINT(bitFun.to_signed_bits(25).data());
	RPRINT("Bit Range: "); RPRINT(MAX_BIT);

	RET2(); RET;

	PRINT("-25 -> (bin) = ");
	PRINT(bitFun.to_signed_bits(-25).data());
	RPRINT("Bit Range: "); RPRINT(MAX_BIT);
	RET2(); RET;

	
	PRINT("256 -> (bin) = ");
	PRINT(bitFun.to_signed_bits(256).data());
	RPRINT("Bit Range: "); RPRINT(MAX_BIT);
	RET2(); RET;

	
	PRINT("358 -> (bin) = ");
	PRINT(bitFun.to_signed_bits(358).data());
	RPRINT("Bit Range: "); RPRINT(MAX_BIT);
	RET2(); RET;

	PRINT("-358 -> (bin) = ");
	PRINT(bitFun.to_signed_bits(-358).data());
	PRINT("Bit Range: "); RPRINT(MAX_BIT);
	RET2(); RET;
	

	RET;

	return -1;
}
