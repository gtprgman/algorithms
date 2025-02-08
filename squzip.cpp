#include <filesystem>

#ifndef REQUIRE_H
	#include "mixutil.h"
	#include "mixhuff.h"
#endif


/*
; Usages: 	  For compressing one file into a *.sqz file.
		  squzip -q <file1.[ext]> <file2.sqz> <COMP_RATE>
		  eg: " squzip -q Letter1.txt Letter1.sqz 0 " --> uses default COMP_RATE
		      " squzip -q Letter1.txt Letter1.sqz 0.65 " --> takes COMP_RATE specified by user.

		  For uncompressing a *.sqz into its original format
		  squzip -d <file2.sqz> <fileX.[ext]> <0>
		  eg: " squzip -d Letter1.sqz Letter1A.txt 0 "

*/

constexpr int MAX = 5;
static char* _pSource = nullptr;



int main(const int argc, const char* args[MAX])
{
	/*             0                 1                   2            3             4
	  args ['program_name'] [-q /-d / -s /-t / -o ] [input_file] [output_file] [ COMP_RATE ]  
	  
	  NB: A minimum of 3 input arguments must be specified to the squzip command.
    */

	
	const char* _c0 = (args[1])? args[1] : "\0";
	
	const double _d1 = (args[4])? std::strtod(args[4], nullptr) : 0.00;

	std::string _f0 = (args[2])? args[2] : "\0",  // raw input file
		    _f1 = (args[3])? args[3] : "\0";  // target output file


	switch (_c0[1])
	{
		case 'q' :
			Compress(_f1,_f0, _d1 ,_pSource);
			break;

		case 'd' :
			UnCompress(_f0, _f1);
			break;

		default:
			PRINT("unknown command or bad filename.");
			break;
	}

	return -1;
}


