/* Using License: GPL v. 3.0 */

#ifndef REQUIRE_H
	#include	"C:\PROJECTS\MIXUTIL\Libs\mixutil.h"
	#include	"C:\PROJECTS\MIXUTIL\Libs\mixhuff.h"

#endif



/*
; Usages: For compressing one file into a *.sqz file.
		  squzip -q <file1.(ext)> <file2.sqz> [COMP_RATE]
		  eg: "squzip -q Letter1.txt Letter1.sqz" --> uses default COMP_RATE
			  "squzip -q Letter1.txt Letter1.sqz 0.65" --> takes COMP_RATE specified by user.
			
		  For uncompressing a *.sqz file into its original format
		  squzip -d <file2.sqz> <fileX.(ext)> 
		  eg: "squzip -d Letter1.sqz Letter1A.txt" --> uses default value
		      "squzip -d Letter1.sqz Letter1A.txt 0.65" --> takes the user specified value.

		  NB: It's important to notice that the COMP_RATE argument must be 
		      the same value provided when you run squzip for squeezing [ -q ]
			  nor unsqueezing [ -d ]. Otherwise the unsqueezing processes may
			  produce undesired results.
		      
*/

constexpr int MAX = 5;
const std::size_t _RowSize = 80;

int main(const int argc, const char* args[MAX])
{
	/*             0            1          2            3             4
	  args ['program_name'] [-q /-d ] [input_file] [output_file] [ COMP_RATE ]

	  NB: A minimum of 4 input arguments must be specified to the squzip command.
	*/

	const char* _c0 = (args[1])? args[1] : "\0";

	std::string _f0 = (args[2])? args[2] : "\0",  // raw input file
				_f1 = (args[3])? args[3] : "\0",  // target output file
				_fN = (args[4])? args[4] : "\0"; // COMP_RATE argument

	const double _d1 = (_fN.empty())? 0 : std::strtod(_fN.data(), nullptr);

	bool gfSucceed = false;
	size_t fgSize = 0;

	mix::ptr_type::U_ARRAY<unsigned char> _uBuff = MK_U_ARRAY<unsigned char>(_RowSize);

	switch (_c0[1])
	{
	case 'q':
		gfSucceed = Compress(_f1, _f0, _d1, _uBuff.get());
		goto EndStop;

	case 'd':
		fgSize = UnCompress(_f0, _f1, _d1);
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


