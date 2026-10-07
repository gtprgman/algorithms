#include "C:\PROJECTS\MIXUTIL\Libs\mixhuff.h"
#include "C:\PROJECTS\MIXUTIL\Libs\mixhuff_impls.h"




int main()
{	
	std::string f_path = "C:\\PROJECTS\\SAMPLES\\header_info.sqz";
	std::vector<UC> x_Chars = { 'A', ' ', 'e', 'p', 'l', 'd', 'a', 't' };
	std::vector<intmax_t> header_info, bit_len = { 2,2,3,4,4,4,4,5 };
	std::string str_dec;
	int64_t sqz_val = 0;

	/* try feed in these numerical instant to one of any conversion functions defined in 'mixbit.h'  */
	/* 6404071158350993606 */
	/* 58DFD4C9CDD5D8C6 */
	
	str_dec = cni_bits_pack(sqz_val, bit_len);

	PRINT(sqz_val);
	PRINT(str_dec);

	x_Chars = {};
	bit_len = {};
	header_info = {};

	return 0;
}
