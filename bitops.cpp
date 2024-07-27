#ifndef REQUIRE_H
	#include "mixutil.h"
#endif


#include "mixbit.h"




int main()
{
	BitN bx;
	std::vector<bitInfo> bits{ {0b1,1},{0b01,2},{0b011,3},{0b0111,4},{0b01111,5} };
				
	const UINT packed = bitsPack(bits);

	bx = packed;

	bx(); 

	RET;


	PRINT(num_of_bits<127088>::eval());


	RET2();
	return -1;
}

