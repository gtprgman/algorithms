#ifndef REQUIRE_H
	#include "C:\PROJECTS\MIXUTIL\Libs\mixutil.h"
	#include "C:\PROJECTS\MIXUTIL\Libs\mixhuff.h"
#endif




int main()
{
	std::vector<_Canonical> vci = {};
	std::vector<node> vNodes = {};

	Can_Bit cn = {};

	/* for example, this is the generated huffman encoding information */
	std::vector<_Canonical> vcn = {
		{cn._xData = 'A', cn._bitLen = 1},
		{cn._xData = 'B', cn._bitLen = 3},
		{cn._xData = 'C',cn._bitLen = 4},
		{cn._xData = 'D',cn._bitLen = 4},
		{cn._xData = 'R',cn._bitLen = 3}
	};

	mix::generic::fast_sort(vcn.begin(), vcn.end(), mix::generic::NLess<char>());
	mix::generic::fast_sort(vcn.begin(), vcn.end(), mix::generic::NLess<intmax_t>());


	_Gen_Canonical_Info(vci, vcn);

	cni_enforce_unique(vci);

	/*
	for (const auto& _ce : vci) {
		RPRINTC(_ce._xData); RPRINTC(_ce._bitLen); RET;
	}
	*/
	
	RET;

	vectorClean(vci);
	vectorClean(vcn);


	/*
		 THIS PART OF CODES SHOW THE PROCESSES INVOLVED IN COMPRESSING A GENERATED HUFFMAN CODES
		 WHICH GENERALLY WE HAD REFERED TO IT AS 'ENCODING HEADER INFORMATION' THAT WOULD BE SAVED
		 TO THE BEGINNING OF '*.sqz ' FILE JUST BEFORE THE CORE COMPRESSED DATA WOULD BE WRITTEN IN.

	*/

	std::vector<UC> _EncDat = { 'A','B','C','D','R' };
	intmax_t _EncInt = 0;
	std::vector<BPAIR> _EncBP;
	std::vector<intmax_t> _EncPac;
	size_t _EncHeadInt = 0;
	std::FILE* _fEncHead = nullptr;
	std::vector<_Canonical> _Cnh1, _Cnh2;

	std::string _fname = "C:\\PROJECTS\\SAMPLES\\header_f.sqz";

	_EncInt = Gen_Encoding_Info(_EncDat, _EncBP, vci, _EncPac, COMP_RATE); RET;
	
	RET;

	
	RET;

	_EncHeadInt = Gen_Cni_Header_Info(_Cnh1,_Cnh2, vci);

	/*
	for (const auto& c1 : _Cnh1)
	{
		RPRINTC(c1._xData); RPRINTC(c1._bitLen); RPRINTC(c1._rle_bit_len); RET;
	}
	*/

	RET;

	_EncHeadInt = Write_Header(_fname.c_str(), _fEncHead, _Cnh1, _Cnh2, 'D');


	vectorClean(vci);
	vectorClean(_EncDat);
	vectorClean(_EncBP);
	vectorClean(_EncPac);

	vectorClean(_Cnh1);
	vectorClean(_Cnh2);

	if (_fEncHead) std::fclose(_fEncHead);

	return 0;
}


	
