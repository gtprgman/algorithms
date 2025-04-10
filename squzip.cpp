/* Using License: GPL v 3.0 */
#include <filesystem>

#ifndef REQUIRE_H
	#include "c:\Users\User\source\repos\Libs\mixutil.h"
	#include "c:\Users\User\source\repos\Libs\mixhuff.h"
#endif



/*
; 	Usages:   For compressing one file into a *.sqz file.
		  squzip -q <file1.(ext)> <file2.sqz> [COMP_RATE]
		  eg: "squzip -q Letter1.txt Letter1.sqz" --> uses default COMP_RATE
		      "squzip -q Letter1.txt Letter1.sqz 0.65" --> takes COMP_RATE specified by user.

		  For uncompressing a *.sqz into its original format
		  squzip -d <file2.sqz> <fileX.(ext)> [0]
		  eg: "squzip -d Letter1.sqz Letter1A.txt"

*/

constexpr int MAX = 5;
std::uintmax_t SIZE_F = 0;

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
	SIZE_F = std::filesystem::file_size(_f0.data());
	std::vector<char> _rawSrc(SIZE_F); 
	
	bool _SqzDone = false;
	std::size_t _UnSqz = 0;

	
	switch (_c0[1])
	{
	case 'q':
		_SqzDone = Compress(_f1, _f0, _d1, _rawSrc);

		if (!_SqzDone)
			std::cerr << "compressing failed.. could not proceed !" << "\n\n";


		goto EndStop;

	case 'd':
		_UnSqz = UnCompress(_f0, _f1);

		if (!_UnSqz)
			std::cerr << "decompressing failed.. could not proceed !" << "\n\n";


		goto EndStop;

	default:
		PRINT("unknown command or bad filename.");
		goto EndStop;
	}


EndStop:
	return 0;
}

