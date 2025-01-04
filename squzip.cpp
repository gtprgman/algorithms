#ifndef REQUIRE_H
	#include "mixutil.h"
	#include "mixhuff.h"
#endif



/*
; Usages: For compressing one file into a *.sqz file.
		  squzip -q <file1.[ext]> [<file2.sqz>]
		  or just squzip -q <file1.[ext]>

		  For uncompressing a *.sqz into its original format
		  squzip -d <file2.sqz> [ <fileX.[ext]> ]
		  or just squzip -d <file2.sqz>
*/

constexpr int MAX = 4;


int main(const int argc, const char* args[MAX])
{
	std::vector<BPAIR> vp = {
		{'A',0b01}, {' ',0b011}, {'p', 0b0100},
		{'e',0b0110}, {'l',0b01001}, {'a',0b010100},
		{'t',0b010110}, {'d',0b0101110}, {'.',0b01011100}
	};
	
	std::vector<packed_info> vpck, vpkRead;

	std::vector<bitInfo<int>> bIf;
	
	std::vector<BPAIR>::iterator _bIter;

	mix::generic::fast_sort(vp.begin(), vp.end(), chrLess());

	std::string _Src = "Ada Ate Apple.";

	bitInfo<int> bi = {};  


	for (auto const& _C : _Src)
	{
		if (mix::generic::vector_search(vp.begin(), vp.end(), _C, chrLess(), _bIter))
		{
			bi._Alpha = _bIter->_data;
			bi.X = _bIter->_val;
			bi.numBits = oneAdder(num_of_bits<int>::eval(bi.X));

			bIf.push_back(bi);
			bi = {};
		}
	}

	bitsPack(vpck, bIf); // packed the encoded information

	// Save the encoded information to 'pckinfo.tbi'.

	writePackInfo("D:\\DATA\\pckinfo.tbi", vpck);

	readPackInfo("D:\\DATA\\pckInfo.tbi", vpkRead);
	
	ReSync(vpkRead);

	/*
	for (auto const& pk : vpkRead)
	{
		PRINT(to_binary<int>::eval(pk._PACKED).data());
		RPRINT(pk.L_BIT); RPRINT(pk.R_BIT);
		RET;
	}
	*/

	
	// Working on the raw data source.
	std::vector<BPAIR> BpC;

	for (auto const& _a : _Src)
	{
		if (mix::generic::vector_search(vp.begin(), vp.end(), _a, chrLess(), _bIter))
			BpC.push_back({ _bIter->_data, _bIter->_val });
	}

	
	std::vector<bitInfo<int>> BiC;

	bi = {};

	for (auto const& _bp : BpC)
	{
		bi._Alpha = _bp._data;
		bi.X = _bp._val;
		bi.numBits = oneAdder(num_of_bits<int>::eval(bi.X));

		BiC.push_back(bi);
		bi = {};
	}

	std::vector<packed_info> vpck1;
	std::vector<int> _readInt, _srcInt;

	bitsPack(vpck1, BiC);

	/*
	for (auto const& pk : vpck1)
	{
		PRINT(to_binary<int>::eval(pk._PACKED).data());
		RPRINT(pk.L_BIT); RPRINT(pk.R_BIT);
		RET;
	}
	*/

	writePack("D:\\DATA\\packed.sqz", vpck1);
	readPack("D:\\DATA\\packed.sqz", _readInt);
	

	RET;
	
	for (packed_info const& _pck : vpck1)
	{
		_srcInt.push_back(_pck._PACKED);
		PRINT(_pck._PACKED);
	}

	ReSync_Int(_readInt, _srcInt);

	RET2();

	UnPack_Bits(_readInt, vpck1);

	for (auto const& _i : _readInt)
		PRINT(to_binary<int>::eval(_i).data() );
 

	//finishedDone:
	vp.clear();
	bIf.clear();

	vpkRead.clear();
	vpck.clear();

	vpck1.clear();
	BiC.clear();
	BpC.clear();
	_readInt.clear();
	_srcInt.clear();
	

	RET2();

	return -1;
}


