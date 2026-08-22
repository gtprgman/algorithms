/* Using License: GPL v. 3.0 */

#ifndef REQUIRE_H
	#include	"C:\PROJECTS\MIXUTIL\Libs\mixutil.h"
	#include	"C:\PROJECTS\MIXUTIL\Libs\mixhuff_impls.h"

#endif



/*
; Usages: For compressing one file into a *.sqz file.
		  squzip -q file1.<ext> file2.sqz [COMP_RATE]
		  eg: "squzip -q Letter1.txt Letter1.sqz" --> uses default COMP_RATE & execute in normal undebug mode.

			  "squzip -q Letter1.txt Letter1.sqz D " --> uses default COMP_RATE & execute in debug mode.

			  "squzip -q Letter1.txt Letter1.sqz u 0.65" --> takes COMP_RATE specified by user
			                                                 and execute in normal/undebug mode. 
			  
			  "squzip -q Letter1.txt Letter1.sqz D 0.65" --> takes user chosen COMP_RATE & execute in debug mode.
			
		  For uncompressing a *.sqz file into its original format
		  squzip -d file2.sqz fileX.<ext> 
		  eg: "squzip -d Letter1.sqz Letter1A.txt"     
*/

constexpr int MAX = 6;
const std::size_t _RowSize = 80;

int main(const int argc, const char* args[MAX])
{
	/*             0            1          2            3             4					5
	  args ['program_name'] [-q /-d ] [input_file] [output_file] [ Debug / Normal ]	[ COMP_RATE ]

	  NB: A minimum of 4 input arguments must be specified to the squzip command.
	*/

	const char* _c0 =	(args[1])? args[1] : "\0";

	std::string _f0 =	(args[2])? args[2] : "\0",  // raw input file
				_f1 =	(args[3])? args[3] : "\0";  // target output file

	const char* _cDbg = (args[4])? args[4] : "\0"; // [ Debug / Normal ] Mode
	std::string _rN =	(args[5])? args[5] : "\0"; // COMP_RATE argument
	

	const double _d1 = (_rN.empty())? 0 : std::strtod(_rN.data(), nullptr);


	bool gfSucceed = false;
	size_t fgSize = 0;

	mix::ptr_type::U_ARRAY<unsigned char> _uBuff = MK_U_ARRAY<unsigned char>(_RowSize);

	switch (_c0[1])
	{
	case 'q':
		gfSucceed = Compress(_f1, _f0, _d1, _uBuff.get(), char(*_cDbg) );
		goto EndStop;

	case 'd':
		fgSize = UnCompress(_f0, _f1);
		goto EndStop;

	default:
		PRINT("unknown command or bad filename.");
		goto EndStop;
	}


EndStop:
	if (_c0[1] == 'q' && !gfSucceed) std::cerr << "\n Compression Failed! Could not proceed. \n\n";
	if (_c0[1] == 'd' && !fgSize) std::cerr << "\n Decompression Failed! Could not proceed. \n\n";

	return 0;
}


