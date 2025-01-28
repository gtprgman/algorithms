#ifndef REQUIRE_H
	#include "mixutil.h"
	#include "mixhuff.h"
#endif



/*
; Usages: 	  For compressing one file into a *.sqz file.
		  squzip -q <file1.[ext]> [<file2.sqz>]
		  or just squzip -q <file1.[ext]>

		  For uncompressing a *.sqz into its original format
		  squzip -d <file2.sqz> [ <fileX.[ext]> ]
		  or just squzip -d <file2.sqz>
*/

constexpr int MAX = 4;


int main(const int argc, const char* args[MAX])
{
	
	const char* _lnComm = (args[1])? args[1] : "\0"; // line input command

	std::string _f0 = (args[2])? args[2] : "\0",  // raw input file
		    _f1 = (args[3])? args[3] : "\0";  // target output file

	std::vector<char> _rawSrc;
	

	switch (_lnComm[1])
	{
		case 'q' :
			Compress(_f0, _f1, _rawSrc);
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

