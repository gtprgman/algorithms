#ifndef REQUIRE_H
	#include "mixutil.h"
#endif

#include "mixbit.h"

	

int main()
{
	BitN bitFun;
	
	std::string f1 = "3.8";
	bitFun.to_fixed_point_bits(f1);
	bitFun(); 
	
	RET2();

	f1 = "8.75";
	bitFun.to_fixed_point_bits(f1);
	bitFun(); 
	
	RET2();

	f1 = "8.63";
	bitFun.to_fixed_point_bits(f1);
	bitFun(); 
	
	RET2();

	f1 = "1.625";
	bitFun.to_fixed_point_bits(f1);
	bitFun(); 
	

	RET2();

	return -1;
}

