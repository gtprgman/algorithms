#ifndef REQUIRE_H
	#include "mixutil.h"
#endif

#include "mixbit.h"

	

int main()
{

	BitN bitC;
	
	PRINT("127 -> (bin) := ");  
	PRINT(bitC.toBits(127).data()); 
	
	RET; RET;

	PRINT("0111 1111 -> (dec):= "); 
	std::cout << bitC.value_from_bitlist({ 0,1,1,1,1,1,1,1, }) << "\n";
	
	RET;

	RET2();

	bitC.setBits({ 1,0,1,0,1,1 });
	std::string strBits = bitC.Bits();

	PRINT("101011 -> (dec) := "); RET;
	PRINT(bitC.value_from_bitstr(strBits)); RET;
	PRINT("43 -> (bin) := "); RPRINT(bitC.toBits(43).data());

	RET2();


	return -1;
}
