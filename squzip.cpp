#include <filesystem>

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

		  Diagnostic tools:

		  View the content of the compressed file.
		  squzip -s <FileX.sqz>

		  View the content of encoded's information data table.
		  squzip -t <FileX.tbi>

		  View the content of the original/decompressed file.
		  squzip -o <FileX.[<Ext>] >


*/

constexpr int MAX = 4;
static char* _pSource = nullptr;


void console_proc(const std::string& _arg1, const std::string& _arg2, int _cArg)
{
	int _cntr = 0;
	std::vector<int> _vpInt;
	std::vector<char> _voChar;
	std::vector<packed_info> _vPack;

	switch (_cArg)
	{
		case 's':
		{
				readPack(_arg1.data(), _vpInt);

				for (const int& _i : _vpInt)
				{
					++_cntr;
					RPRINTC(_i);

					if (_cntr > 79)
					{
						_cntr = 0;
						RET;
					}
				}
				_cntr = 0;
				RET;
				std::cout << "Total Encoded Items: " << _vpInt.size();
				RET;
		}break;

		case 't':
		{
			readPackInfo(_arg1.data(), _vPack);
			ReSync(_vPack);
			_cntr = 0;

			for (packed_info const& _pck : _vPack)
			{
				RPRINTC(_pck.L_ALPHA); RPRINTC(_pck.R_ALPHA); RPRINTC(_pck._PACKED);
				RET;
				_cntr += _pck._PACKED;
			}
			RET;
			std::cout << "Total packed value: " << _cntr;
			RET;
			std::cout << "Total Encoded Items: " << _vPack.size();
			RET;
		}break;
		
		case 'o':
		{
			const std::size_t _memSz = FS::file_size(_arg1.data());
			char* _textBuff = new char[_memSz] {0};
			readOriginal(_arg1.data(), _textBuff, _memSz);
			mix::generic::BUFF_Print<char>(_textBuff,_memSz, RPRINT<char>);
			if (_textBuff) delete[] _textBuff;

		}break;

		default:
			break;
    }
	_vpInt.clear();
	_vPack.clear();
	_voChar.clear();
	
}



int main(const int argc, const char* args[MAX])
{
	const char* _lnComm = (args[1])? args[1] : "\0"; // line input command

	std::string _f0 = (args[2])? args[2] : "\0",  // raw input file
		    _f1 = (args[3])? args[3] : "\0";  // target output file

	
	switch (_lnComm[1])
	{
		case 'q' :
			Compress(_f0, _f1, _pSource);
			break;

		case 'd' :
			UnCompress(_f0, _f1);
			break;

		case 's':
			console_proc(_f0, _f1, 's');
			break;

		case 't':
			console_proc(_f0, _f1, 't');
			break;

		case 'o':
			console_proc(_f0, _f1, 'o');
			break;

		default:
			PRINT("unknown command or bad filename.");
			break;
	}

	return -1;
}




