#pragma once
/* Using License GPL v 3.0. */

#ifndef MX_HUFF_IMPLS
	#define MX_HUFF_IMPLS

#endif


const size_t _ROWSZ = 80;
static std::string _SystemFile = "\0";


constexpr double COMP_RATE = 0.52; /* 0.52 is the default value, the users are allowed to tweak it
									 in the command line */



// ReSync the integers of *.sqz
static inline const std::size_t ReSync_Int(const intmax_t& _IntSqz, 
										   const std::vector<UC> Bit_Len,
										   const std::vector<Can_Bit>& _CniHeader, 
										   std::vector<UC>& _Original)
{
	intmax_t w = 0, bx = 0;
	std::size_t synced_size = 0;
	std::vector<Can_Bit>& HeaderInfo = (std::vector<Can_Bit>&)_CniHeader;
	std::vector<Can_Bit>::iterator _HiT;

	char _x = 0;
	std::string bitX = "\0", 
	_bit_str = concat_str((char*)"0", bit_str(intmax_t(_IntSqz)).c_str());
/*
	PRINT(_bit_str); RET;
	return 0;  
	// '_bit_str' is assigned with correct bits pattern
*/

	std::string::iterator _bitter = _bit_str.begin();
	char* _pb = (char*)_bitter._Ptr; // points to the beginning of _bit_str

	std::vector<intmax_t> _SqzCodes = {};
	const size_t _BitL_Size = Bit_Len.size();

	// acquiring codes symbols from the read *.sqz data
	for (size_t g = 0; g < _BitL_Size; g++) {
		w = Bit_Len[g];
		bitX = lstr(_pb, w);
		bx = int_bit(bitX.c_str());
		_SqzCodes.push_back(bx);
		
		bx = 0; bitX.clear();

		_bitter += w; w = 0;
		_pb = (char*)_bitter._Ptr;
	}

/*
	mix::generic::STL_Print(_SqzCodes.begin(), _SqzCodes.end(), RPRINTC<intmax_t>); RET;
	return 0;
   // '_SqzCodes' vector is successfully filled with correct codes symbols
*/

/*
	for (const auto& _cne : _CniHeader)
	{
		RPRINTC(_cne._xData); RPRINTC(_cne._codeWord); RET;
	}

*/

	const intmax_t pacSize = _SqzCodes.size();

	// resolving codes symbols into original data ..
	for (intmax_t t = 0; t < pacSize; t++)
	{
		if ( mix::generic::vector_search(HeaderInfo.begin(), HeaderInfo.end(), _SqzCodes.at(t),
										mix::generic::NLess<intmax_t>(), _HiT) )
		{
			_x = _HiT->_xData;
			_Original.push_back((int)_x);
			++synced_size;
			_x = 0;
		}

	}

	//mix::generic::STL_Print(_Original.begin(), _Original.end(), RPRINTC<char>); RET;

	return synced_size;
}



static inline void filter_pq_nodes(std::vector<node>& _target, std::priority_queue<node>& _Pqueue)
{
	std::size_t _Cnt = 0;
	node _nod = 0;
	intmax_t _fqr = 0;


	while (!_Pqueue.empty())
	{
		_nod = _Pqueue.top();
		_Pqueue.pop();

		if (_target.empty())
		{
			_fqr = _nod.FrequencyData();
			if (!_fqr) ++_fqr;
			_nod.setFrequencyData(intmax_t(_fqr) );
			_target.push_back(_nod);
			continue;
		}

		if (_target[_Cnt] == _nod)
		{
			_fqr = _target[_Cnt].FrequencyData();
			_target[_Cnt].setFrequencyData(intmax_t(++_fqr) );
			continue;
		}
		else
		{
			_fqr = 1;
			_nod.setFrequencyData(int64_t(_fqr) );
			_target.push_back(_nod);
			_Cnt = _target.size() - 1;
		}
	}
}



inline const bool _TREE::create_encoding(const size_t& _From, 
										 const size_t& _To,
										 int64_t& _bt,
										 const std::vector<node>& _Vn)
{
	node _e = '0'; bool _bEncodeable = false;
	intmax_t _Dir = 0, _recurr = 0, _sameVal = 0, _prevX = 0;
	static intmax_t _fq = 0;
	std::vector<BPAIR>::iterator _iGet;
	const size_t _LowerBound = _From, _UpperBound = _To;

	if (_LowerBound > _Vn.size() || _UpperBound > _Vn.size()) return false;

	_fq = 50; // the initial root frequency is 50%

	// Processing the Encoding from vector data
	for (size_t i = _From; i < _To; i++)
	{
		 _e = _Vn.at(i);

		if (_fq > _e.FrequencyData())
			_Dir = L;

		else if (_fq < _e.FrequencyData())
			_Dir = R;

		else if (_fq == _e.FrequencyData())
		{
			_Dir = R;
			_recurr++;
		}
	  
		_fq = _e.FrequencyData();

		if (_recurr > 1)
		{
			_sameVal = ++_prevX;
			_bt = _sameVal;
			_recurr = 0;
			goto filterPhase;
		}
		
		_bt |=_Dir;

	filterPhase: // "enforce unique mechanism" layer 1

		_sameVal = _bt;
		_prevX = _sameVal;

		if (mix::generic::
			vector_search(_vPair.begin(), _vPair.end(), _sameVal, bitLess(), _iGet))
			//if (std::binary_search(_vPair.begin(), _vPair.end(),_bpr))
		{
			_sameVal = _iGet->_val;
			_sameVal += ((_iGet - _vPair.begin()) / 2);
			_bt = _sameVal;
			_prevX = _sameVal;
		}

		_vPair.push_back(BPAIR{ UC(_e.dataValue()), intmax_t(_prevX) });
		mix::generic::fast_sort(_vPair.begin(), _vPair.end(), bitLess());
		//std::stable_sort(_vPair.begin(), _vPair.end());
		_bEncodeable = true;
	}
	_fq = 0;

	
	return _bEncodeable;
}




inline void _TREE::schema_Iter(const std::vector<node>& _fpNods, const double _cmpRate = 0)
{
	const size_t _TreeSizes = _fpNods.size();

	const double _CompRate = (_TreeSizes > 5 && _cmpRate)? std::floor(_cmpRate * _TreeSizes) : _TreeSizes;

	const double _fCompRate = (_CompRate)? std::ceil((double)_TreeSizes / _CompRate) : 0;
	const size_t _DivSize = (_fCompRate)? (size_t)_fCompRate : 1;
	size_t _divSize = _DivSize;
	int64_t _msk = 0, _BT = 2, _Dir = L;
	bool _bSucceed = false;

	//Clean();

	for (size_t t = 0; t < _TreeSizes; t += _DivSize)
	{
		if ( (_TreeSizes - t) <= _DivSize ) _divSize = 1;
		_bSucceed = create_encoding(t, (( t + _divSize) >= _TreeSizes)? (t + _TreeSizes) : (t + _divSize), _msk, _fpNods);

		if (!_bSucceed) break;
	
		_msk ^= _BT--;

		if (_BT < 1) 
			_BT = 2; 
			

		_Dir = _Dir ^ 1;
		_msk |= _Dir;
	}

	// to encode the last left item in '_fpNods' vector
	create_encoding(_TreeSizes - 1, _TreeSizes, _msk, _fpNods);

	mix::generic::t_sort(_vPair.begin(), _vPair.end(), 0.25, bitLess());
	_TREE::enforce_unique(_vPair); 
}




inline void _TREE::enforce_unique(std::vector<BPAIR>& _bPairs)
{
	int64_t _Addend = 0;
	int64_t _MaxSz = (int64_t)_bPairs.size();

	for (int64_t n = 0, m = 0; m < _MaxSz; m++)
	{
		_Addend = m;
		_bPairs[m]._val += (_Addend + 1);
		_Addend = 0;
	}
}



static inline const size_t writePackInfo(std::FILE*& _fHandle, const std::vector<unsigned char>& _hDatInfo)
{
	size_t f_size = 0;

	if (!_fHandle) {
		std::cerr << "\n Error saving header information to a File. \n";
		return 0;
	}

	const size_t h_size = _hDatInfo.size();

	if (!(f_size = std::fwrite(&h_size, sizeof(h_size), 1, _fHandle))) {
		std::cerr << "\n Error saving number of records' size to file \n";
		if (_fHandle) std::fclose(_fHandle);
		return 0;
	}

	for (size_t z = 0; z < h_size; z++)
	{
		std::fputc(_hDatInfo[z], _fHandle);
		++f_size;
	}


	return f_size;
}




// Read the encoded information data table from a file.
static inline const std::size_t readPackInfo(const std::string& _inFile, std::vector<_Canonical>& CanInfo0 ,std::vector<_Canonical>& CanInfo1)
{
	intmax_t _x = 0;
	Can_Bit cbi = {};
	size_t _readSize = 0, infSize = 0;
	std::FILE* _Fr = std::fopen(_inFile.data(), "rb");

	if (!_Fr)
	{
		std::cerr << "\n Error read accesses to file. \n";
		return 0;
	}

	// Read the records's size number
	if (!std::fread(&infSize, sizeof(size_t), 1, _Fr))
	{
		std::cerr << "\n Error read header size information. \n";
		if (_Fr) std::fclose(_Fr);
		return 0;
	}

	const size_t header_size = infSize;

	vectorClean(CanInfo0);

	// picking up the encoded chars ..
	for (size_t f = 0; f < header_size; f++)
	{
		cbi._xData = std::fgetc(_Fr);
		if (cbi._xData > 0)
		{
			CanInfo0.push_back(cbi); ++_readSize;
		}
		cbi = {};
	}

	// read the records' size
	if (!std::fread(&infSize, sizeof(size_t), 1, _Fr))
	{
		std::cerr << "\n Error read header size information. \n";
		if (_Fr) std::fclose(_Fr);
		return 0;
	}

	// picking up initial codes' length..
	for (size_t r = 0; r < header_size; r++)
	{
		_x = std::fgetc(_Fr);
		if (_x > 0)
		{
			CanInfo0[r]._bitLen = _x;
			++_readSize;
		}
	}

	// Reads up size information ..
	if (!std::fread(&infSize, sizeof(size_t), 1, _Fr))
	{
		std::cerr << "\n Error read header info data. \n";
		if (_Fr) std::fclose(_Fr);
		return 0;
	}

	const size_t cni_header_size = infSize;

	cbi = {};

	// picking up actual codes' length ..
	for (size_t j = 0; j < cni_header_size; j++)
	{
		cbi._bitLen = std::fgetc(_Fr);
		if (cbi._bitLen > 0) { CanInfo1.push_back(cbi); ++_readSize; }
		cbi = {};
	}

	// Reads up size information ..
	if (!std::fread(&infSize, sizeof(size_t), 1, _Fr))
	{
		std::cerr << "\n Error read header info data. \n";
		if (_Fr) std::fclose(_Fr);
		return 0;
	}

	// picking up actual codes' RLE of bit-lengths..
	for (size_t g = 0; g < cni_header_size; g++)
	{
		_x = std::fgetc(_Fr);
		if (_x > 0) {
			CanInfo1[g]._rle_bit_len = _x; ++_readSize;
		}
	}

	if (_Fr) std::fclose(_Fr);
	return _readSize;
}



// Packed the raw data source and saves it to a file.
static inline const intmax_t writePack(std::FILE*& _fSqz, const std::string& sqz_hex)
{
	if (!_fSqz) {
		std::cerr << "\n\n Error Open Write-Accesses to File !\n";
		return 0;
	}

	const size_t w_size = save_cni_bit(_fSqz, sqz_hex);

	if (_fSqz) std::fclose(_fSqz);

	return w_size;
}



// Read the packed data source into an int Vector.
static inline const std::size_t readPack(std::string&& _SqzFile, std::vector<intmax_t>& vInts)
{
	int _x = 0;
	size_t _totBytesRead = 0, h_size = 0;
	std::FILE* _fHandle = std::fopen(_SqzFile.c_str(), "rb");

	if (!_fHandle) {
		std::cerr << "\n Error open Read-Accesses to File !\n\n";
		return 0;
	}

	vectorClean(vInts);

	if (!(_totBytesRead = std::fread(&h_size, sizeof(size_t), 1, _fHandle)))
		std::cerr << "\n encoded chars information read error ..";
	else for (size_t i = 0; i < h_size; i++) _x = std::fgetc(_fHandle);
 

	if (!(_totBytesRead = std::fread(&h_size, sizeof(size_t), 1, _fHandle)))
		std::cerr << "\n initial bit-lengths information read error ..";
	else for (size_t i = 0; i < h_size; i++) _x = std::fgetc(_fHandle);


	if (!(_totBytesRead = std::fread(&h_size, sizeof(size_t), 1, _fHandle)))
		std::cerr << "\n actual codes' length read error ..";
	else for (size_t i = 0; i < h_size; i++) _x = std::fgetc(_fHandle);


	if (!(_totBytesRead = std::fread(&h_size, sizeof(size_t), 1, _fHandle)))
		std::cerr << "\n actual codes' RLE information read error ..";
	else for (size_t i = 0; i < h_size; i++) _x = std::fgetc(_fHandle);


	if (!(_totBytesRead += read_cni_bit(_fHandle, vInts)))
	{
		std::cerr << "\n Fatal !! corrupted packed symbols read ..";
		std::cerr << "\n could not proceed ..\n";
		if (_fHandle) std::fclose(_fHandle);
		return 0;
	}

	
return _totBytesRead;
}



// write the original uncompressed form of the data into a file.
static inline const std::size_t writeOriginal(const std::string& _OriginFile, const std::vector<UC>& raw_dat)
{
	size_t rawSize = 0;
	std::FILE* FRaw = std::fopen(_OriginFile.data(), "wb");

	if (!FRaw)
	{
		std::cerr << "\n Error Write-Accessed to File.";
		return 0;
	}

	const size_t nSize = raw_dat.size();;

	for (size_t s = 0; s < nSize; s++)
	{
		std::fputc(raw_dat[s], FRaw);
		++rawSize;
	}

	if (FRaw) std::fclose(FRaw);
	return rawSize;
}



// read the original data source from a file and populate the vector with the read data.
static inline const std::size_t readOriginal(std::FILE*& _fHandle , std::vector<unsigned char>& _vecData, unsigned char*& _DataSrc)
{
	std::size_t _wordsRead = 0;
	
	if (!_fHandle)
	{
		std::cerr << "\n Error read source file.. !! " << "\n\n";
		goto EndRead;
	}

	if (!_DataSrc) _DataSrc = new unsigned char[_ROWSZ];
	
	while (std::fgets((char*)_DataSrc, _ROWSZ, _fHandle))
	{
		for (size_t c = 0; c < _ROWSZ; c++) {
			if (_DataSrc[c] > 0) {
				_vecData.push_back(_DataSrc[c]);
				++_wordsRead;
			}
		}

		std::memset(_DataSrc, 0, _ROWSZ);
	}
	

EndRead:
	return _wordsRead;
}


// generates a huffman encoding information ..
static inline const int64_t Gen_Encoding_Info(std::vector<unsigned char>& _Src, 
											  std::vector<BPAIR>& CodInfo, 
										      std::vector<_Canonical>& Cni_Info0, 
											  std::vector<_Canonical>& Cni_Info1,
											  std::vector<intmax_t>& PacResults,
											  std::vector<intmax_t>& PacInts,
										      const double& cmp_rate, 
											  char&& _cCode = 'u')
{
	intmax_t SqzInt = 0;
	std::priority_queue<node, std::vector<node>, std::less<node>> _pq = {};
	std::priority_queue<node, std::vector<node>, fqLess> _fpq = {};
	std::vector<node> PNodes = {};

	Can_Bit cbi = {};

	std::vector<_Canonical> CniBits = {};
	std::vector<_Canonical>::iterator CBiT = {};

	std::string xs_bit = "\0";

	const double comp_ratio = (cmp_rate)? cmp_rate : COMP_RATE;

	const size_t T_SIZE = _Src.size();

	for (size_t t = 0; t < T_SIZE; t++)
	{
		PNodes.push_back(node(UC(_Src[t])));
	}

	if (_cCode == 'D') {
		PRINT("\nNodes data..");
		for (const auto& _e : PNodes) RPRINTC(_e.dataValue());
	}
	RET;

	for (const node& e : PNodes)
	{
		_pq.emplace(e);
	}

	PNodes.clear();

	filter_pq_nodes(PNodes, _pq);

	if (_cCode == 'D')
	{
		PRINT("\nFiltered Nodes Data..");
		for (const auto& _e : PNodes) RPRINTC(_e.dataValue());
		RET;
	}

	for (const node& nod : PNodes)
	{
		_fpq.emplace(nod);
	}

	PNodes.clear();

	for (node e = 0; !_fpq.empty(); )
	{
		e = _fpq.top(); _fpq.pop();
		PNodes.push_back(e);
	}

	if (_cCode == 'D')
	{
		PRINT("\nFrequency nodes data..");
		for (const auto& _e : PNodes) RPRINTC(_e.dataValue());
		RET;
	}

	
	_TREE::plot_tree(PNodes, comp_ratio);

	CodInfo = _TREE::CodeMap(); // huffman encoding info generated

	vectorClean(Cni_Info0);

	if (_cCode == 'D')
	{
		PRINT("\nGenerated Huffman Encoding Information. (BPAIR)");
		PRINT("_xData <=> _val");
		for (const auto& bp : CodInfo)
		{
			RPRINTC(bp._data); RPRINTC(bp._val); RET;
		}
	}

	for (const BPAIR& bp : CodInfo)
	{
		cbi._xData = bp._data;
		cbi._bitLen = bp.bit_len;
		Cni_Info0.push_back(cbi);
		cbi = {};
	}

	if (_cCode == 'D') {
		PRINT("\nRaw Canonical Data Obtained from the Generated BPAIR. (Cni_Info0) ");
		PRINT("_xData <=> _bitLen");
		for (const auto& ce : Cni_Info0)
		{
			RPRINTC(ce._xData); RPRINTC(ce._bitLen); RET;
		}
	}
	
		mix::generic::fast_sort(Cni_Info0.begin(), Cni_Info0.end(), mix::generic::NLess<char>());
		mix::generic::fast_sort(Cni_Info0.begin(), Cni_Info0.end(), mix::generic::NLess<intmax_t>());
	
	if (_cCode == 'D')
	{
		PRINT("\nSorted Canonical Encoding Information Data.. (sorted Cni_Info0) " );
		PRINT("_xData <=> _bitLen");
		for (const auto& _cb : Cni_Info0)
		{
			RPRINTC(_cb._xData); RPRINTC(_cb._bitLen); RET;
		}

		RET;
	}

	vectorClean(Cni_Info1);

	_Gen_Canonical_Info(Cni_Info1, Cni_Info0); // obtaining codewords based on bit length info ..

	cni_enforce_unique(Cni_Info1); // codewords data updated

	if (_cCode == 'D') {
		PRINT("\nCanonical Huffman Encoding Information Generated ! [ Cni_Info1 ] " );
		PRINT("_xData <=> _codeWord <=> _bitLen");

		for (const auto& ci : Cni_Info1)
		{
			RPRINTC(ci._xData); RPRINTC(ci._codeWord); RPRINTC(ci._bitLen);
			RET;
		}
	}

	cbi = {}; 

	for (const _Canonical& cni : Cni_Info1)
	{
		cbi._xData = cni._xData;
		cbi._codeWord = cni._codeWord;
		cbi._bitLen = len_bit(intmax_t(cni._codeWord));
		CniBits.push_back(cbi);
		cbi = {};
	}

	cbi = {}; vectorClean(Cni_Info1);

	for (const auto& _cnb : CniBits) {
		cbi._xData = _cnb._xData;
		cbi._codeWord = _cnb._codeWord;
		cbi._bitLen = _cnb._bitLen;
		Cni_Info1.push_back(cbi);
		cbi = {};
	}
	
	mix::generic::t_sort(CniBits.begin(), CniBits.end(), 0.25, mix::generic::NLess<char>());

	if (_cCode == 'D') {
		PRINT(" \n Finalized Sorted(char) Canonical Encoding Information Generated .. [ Cni_Info1 ] ");
		PRINT(" _xData <=> _codeWord <=> _bitLen ");
		for (const auto& _cnb : CniBits) {
			RPRINTC(_cnb._xData); RPRINTC(_cnb._codeWord); RPRINTC(_cnb._bitLen); RET;
		}
	}

	RET;

	// gathering source data..
	for (size_t fz = 0; fz < T_SIZE; fz++)
	{
		char _ci = _Src[fz];

		if (_ci > 0)
		{
			if (mix::generic::vector_search(CniBits.begin(), CniBits.end(), _ci, mix::generic::NLess<char>(), CBiT))
			{
				PacInts.push_back(CBiT->_codeWord);
			}
		}
	}

	if (_cCode == 'D') {
		PRINT("\n Generated Code Symbols ..");
		mix::generic::STL_Print(PacInts.begin(), PacInts.end(), RPRINTC<intmax_t>); RET;
	}

		SqzInt = cni_bits_pack(PacResults,PacInts);

		if (_cCode == 'D') { RPRINTC("\n Packed Integer Symbols .. "); RPRINTC(SqzInt); } RET;
	
	
	return SqzInt;
}


// Generate a header data information ..
static const size_t Gen_Cni_Header_Info(std::vector<_Canonical>& header0_info, 
										std::vector<_Canonical>& header1_info,
										const std::vector<intmax_t>& pac_Ints,   // Code Symbols
										const std::vector<_Canonical>& CniSrc,  // Cni_Info0
										const std::vector<_Canonical>& CniInfo, // Cni_Info1
										char&& _iDebug = 'u')
{
	size_t header0_size = 0, header1_size = 0, _vi = 0;
	intmax_t _b = 0;
	std::vector<node> _vNods;
	node _e = 0;

	Can_Bit cni_head0 = {}, cni_head1 = {};

	 // constructs the bit-length & RLE of bit-length of the actual packed symbols..
	for (const auto& _ci : pac_Ints) // Code Symbols
	{
		_b = len_bit(intmax_t(_ci));
		// set up the frequency data for each bit length ..

		if (_vNods.empty())
		{
			goto addNode;
			continue;
		}

		_vi = _vNods.size();

		if (_b == _vNods[_vi - 1].Value())
		{
			_b = _vNods[_vi - 1].FrequencyData();
			_vNods[_vi - 1].setFrequencyData(intmax_t(++_b));
			continue;
		}
		else goto addNode;
		
	addNode:
			_e = node(UC(_b)); // '_e ' is a bit length node
			_e.setFrequencyData(_e.FrequencyData() + 1);  // frequency of a particular bit length node
			_vNods.push_back(_e);
	}

	// the required information (_bitLen & _rle_bit_len) for actual packed symbols is gathered.

	const size_t _InfoSize = _vNods.size();

	vectorClean(header1_info);

	// writing actual code symbols length..
	for (size_t d = 0; d < _InfoSize; d++)
	{
		_b = _vNods[d].Value();   // bit length info
		if (_b) {
			cni_head1._bitLen = _b;
			header1_info.push_back(cni_head1);
			++header1_size;
			cni_head1 = {};
		}
	}

	// writing RLE info for each actual code symbol length ..
	for (size_t f = 0; f < _InfoSize; f++)
	{
		_b = _vNods[f].FrequencyData();

		if (_b > 0)
		{
			header1_info[f]._rle_bit_len = _b;
			++header1_size;
		}
	}

	/* reconstructing information from the Initial huffman generated code symbols.. */
	char _C = 0;
	const size_t CndSize = CniSrc.size(); // from the 'Cni_Info0' data table

	vectorClean(header0_info);

	// acquiring the encoded character & the codeword for each encoded character..
	for (size_t d = 0; d < CndSize; d++)
	{
		_C = CniSrc[d]._xData;
		if (_C) {
			cni_head0._xData = CniSrc[d]._xData; // encoded character
			header0_info.push_back(cni_head0);
			++header0_size;
			cni_head0 = {};
		}
	}


	// writing bit lengths info ..
	for (size_t cx = 0; cx < CndSize; cx++)
	{
		_b = CniSrc[cx]._bitLen;
		if (_b) {
			header0_info[cx]._bitLen = _b;
			++header0_size;
		}
	}


	if (_iDebug == 'D')
	{
		PRINT("Header 0 : ");
		RPRINTC("Data: "); RPRINTC("Bit Length: "); RET;
		for (const auto& cn0 : header0_info)
		{
			RPRINTC(cn0._xData);  RPRINTC(cn0._bitLen); RET;
		}

		PRINT("\n\n");

		PRINT("Header 1 : ");
		RPRINTC("Bit Length: "); RPRINTC("RLE of Bit Length: "); RET;
		for (const auto& cn1 : header1_info)
		{
			RPRINTC(cn1._bitLen); RPRINTC(cn1._rle_bit_len); RET;
		}

		RET;
	}

	return header0_size + header1_size;
}


static inline const bool Compress(const std::string& _destF, const std::string& _srcF, const double& compRate, 
									unsigned char* _cBuff)
{
	bool _bDone = 0;
	UC _xt = 0;

	std::string _sqz_hex = "\0";

	std::vector<UC> _srcData = {}, xChars = {}, xBitLen = {},
					_bit_len = {}, rle_bit = {};

	std::vector<intmax_t> _pacInts = {}, _pacRes = {};

	std::vector<BPAIR> _CodeMap = {};
	std::vector<_Canonical> CniHead0 = {}, CniHead1 = {}, _CanSrc = {}, _CanInfo = {};
	
	std::FILE* _FO = std::fopen(_srcF.c_str(), "rb") , 
			  *_FT = std::fopen(_destF.c_str(), "wb");

	std::size_t F_SIZE = 0, HCN_SIZE = 0, PACKED_HEADER_SIZE = 0;
	intmax_t _sqzNum = 0;

	if (!_FO)
	{
		std::cerr << "\n Error Read Source File. " << "\n\n";
		goto finishedDone;
	}

	if (!(F_SIZE = readOriginal(_FO, _srcData, _cBuff)) )
	{
		RET;
		std::cerr << "\n Error read source file." << "\n\n";
		goto finishedDone;
	}
	
	
	_sqzNum = Gen_Encoding_Info(_srcData, _CodeMap, _CanSrc, _CanInfo, _pacRes,_pacInts, compRate);
	// _srcData is fully filled with correct data
	// _CodeMap is generated successfully
	// _CanSrc is fetch back with the correct data values	
	// _CanInfo is fully filled with correct data
	// _pacInts is fully filled with correct values
	/*
		mix::generic::STL_Print(_pacInts.begin(), _pacInts.end(), RPRINTC<intmax_t>); RET;
		return 0;
	*/ 
	
	/*_CanSrc  => Cni_Info0 in Gen_Encoding_Info() and
	  _CanInfo => Cni_Info1 in Gen_Encoding_Info()
	*/

	for (const auto& _ei : _pacRes) _sqz_hex = concat_str((char*)_sqz_hex.c_str(), To_HexF<int>::eval(_ei).c_str());
	
	// _sqz_hex is assigned with the correct hex string pattern

	vectorClean(CniHead0); vectorClean(CniHead1);

	HCN_SIZE = Gen_Cni_Header_Info(CniHead0,CniHead1, _pacInts, _CanSrc, _CanInfo);
	// CniHead0 & CniHead1 are fully filled with correct data

	//PRINT(_sqzNum);

	// Saving encoding information headers data ..
	for (const auto& cn0 : CniHead0)
	{
		_xt = cn0._xData;
		xChars.push_back(_xt);

		_xt = (int)cn0._bitLen;
		xBitLen.push_back(UC(_xt));

		_xt = 0;
	}

	// saving encoded chars ..
	if (!(F_SIZE = writePackInfo(_FT, xChars)))
	{
		std::cerr << "\n Error saving encoding characters ..";
		std::cerr << "\n Could not proceed ..";
		goto finishedDone;
	}

	// saving initial bit-lengths info ..
	if (!(F_SIZE = writePackInfo(_FT, xBitLen)))
	{
		std::cerr << "\n Error saving initial codes' length ..";
		std::cerr << "\n Could not proceed ..";
		goto finishedDone;
	}


	for (const auto& cnf : CniHead1)
	{
		_xt = (int)cnf._bitLen;
		_bit_len.push_back(UC(_xt));

		_xt = (int)cnf._rle_bit_len;
		rle_bit.push_back(UC(_xt));

		_xt = 0;
	}


	// saving actual code symbols bit length info ..
	if (!(F_SIZE = writePackInfo(_FT, _bit_len)))
	{
		std::cerr << "\n Error saving actual code symbols lengths ..";
		std::cerr << "\n Could not proceed ..";
		goto finishedDone;
	}

	// saving actual code symbols RLE information of bit-lengths..
	if (!(F_SIZE = writePackInfo(_FT, rle_bit)))
	{
		std::cerr << "\n Error saving RLE of actual codes' length..";
		std::cerr << "\n Could not proceed ..";
		goto finishedDone;
	}

	
	// writing packed data source into a file ( *.sqz ).
	if ( !(_bDone = writePack(_FT, _sqz_hex)))
	{
		RET;
		std::cerr << "\n Error writing compressed file !  \n\n";
		goto finishedDone;
	}


finishedDone:
	vectorClean(_srcData);
	vectorClean(_pacInts);
	vectorClean(_CodeMap);
	vectorClean(_CanSrc);
	vectorClean(CniHead0);
	vectorClean(CniHead1);
	vectorClean(xChars);
	vectorClean(xBitLen);
	
	if (!_SystemFile.empty()) _SystemFile.clear();
	if (_FO) std::fclose(_FO);
	if (_FT) std::fclose(_FT);

	_TREE::Clean();
	F_SIZE = 0;
	return _bDone;
}



static inline const std::size_t UnCompress(const std::string& _packedFile, const std::string& _unPackedFile, 
											const double& cmp_rate)
{
	UC _Bit = 0; Can_Bit _cbt;
	std::vector<UC> _rawData, _BitL;
	std::vector<Can_Bit> cnbt;
	std::vector<_Canonical> Cni_Info, Cni_Head0, Cni_Head1;
	std::vector<intmax_t> _Codes;
	intmax_t _SqzInt = 0, _bix = 0;
	size_t _rawSize = 0, header_size = 0;
	std::FILE* _FX = std::fopen(_packedFile.c_str(), "rb");
	std::string _read_hex = "\0";
	std::string::iterator hex_Itr;
	char* _hex_End = (char*)"\0";

	if (!_FX)
	{
		std::cerr << "\n Compressed Data Read Error !";
		std::cerr << "\n Could not proceed ..";
		goto EndPhase;
	}


	header_size = readPackInfo(_packedFile.c_str(), Cni_Head0, Cni_Head1);
	// Cni_Head0 & Cni_Head1 are successfully fetched with correct data

	if (!header_size)
	{
		std::cerr << "\n Header Info Data Read Error !";
		std::cerr << "\n Could not proceed ..";
		goto EndPhase;
	}
	
	/*
	   // Debugging Codes..
	    
	PRINT("\n Initial Huffman Information.. ");
	RPRINTC("Data: "); RPRINTC("Bit length: "); RET;
	for (const auto& cn0 : Cni_Head0)
	{
		RPRINTC(cn0._xData); RPRINTC(cn0._bitLen); RET;
	}

	PRINT("\n Actual Code Symbols Information.. ");
	RPRINTC("Bit length: "); RPRINTC("RLE of Bit length: "); RET;
	for (const auto& cn1 : Cni_Head1)
	{
		RPRINTC(cn1._bitLen); RPRINTC(cn1._rle_bit_len); RET;
	}

	RET;
	goto EndPhase;
	*/
	

	_Gen_Canonical_Info(Cni_Info, Cni_Head0);
	cni_enforce_unique(Cni_Info); // Cni_Info successfully fetched with correct data
	_SqzInt = readPack(_packedFile.c_str(), _Codes); // _Codes successfully fetched with correct data
	
	for (const auto& _e : _Codes) _read_hex = concat_str((char*)_read_hex.c_str(), To_HexF<int>::eval(_e).c_str());
	// _read_hex is assigned with the correct hex digits pattern

	_hex_End = (char*)rstr(_read_hex.c_str(), 1).c_str();
	
	_bix = strtoint(_hex_End);

	_read_hex = rtrimx(_read_hex.c_str(), 1, '\0');

	_read_hex = HxFs_To_Bin(_read_hex.c_str()); // _read_hex is assigned with the corret bits pattern

	_read_hex = (char*)concat_str((char*)_read_hex.c_str(), bit_str(intmax_t(_bix)).c_str());

	hex_Itr = trunc_left_zeroes(_read_hex); // hex_Itr is assigned with the correct truncated bits pattern

	PRINT(hex_Itr._Ptr); RET;

	goto EndPhase;

	header_size = Cni_Head1.size();
	// expands out RLE information into '_BitL' vector
	for (size_t x = 0; x < header_size; x++)
	{
		_Bit = (int)Cni_Head1[x]._bitLen;
		_bix = Cni_Head1[x]._rle_bit_len;

		// t = 0 -> number of repeated bit-lengths..
		for (intmax_t t = 0; t < _bix; t++)
		{
			_BitL.push_back(UC(_Bit));
		}
	} // _BitL is successfully fetched with correct data


  /*
		// Debugging codes..
		PRINT(_SqzInt); RET;
		PRINT(bit_str(intmax_t(_SqzInt))); RET;
		goto EndPhase;

		// '_SqzInt' and its evaluated bit string are come out to be correct
  */

/*  // Debugging codes.. 
	RPRINTC("Data: "); RPRINTC("Code: "); RET;
	for (const auto& _cni : Cni_Info)
	{
		RPRINTC(_cni._xData); RPRINTC(_cni._codeWord); RET;
	}
	goto EndPhase;

	// Finalized 'Encoding Information Header' (Cni_Info) is successfully fetched with correct data
*/

	for (const auto& _ci : Cni_Info)
	{
		_cbt._xData = _ci._xData;
		_cbt._codeWord = _ci._codeWord;

		cnbt.push_back(_cbt);
	}

	PRINT("\n rematching code symbols with data ..");

	if (!(_rawSize = ReSync_Int(_SqzInt, _BitL, cnbt, _rawData)))
		std::cerr << "\n code symbols mismatched ..";


	if (!(_rawSize = writeOriginal(_unPackedFile.c_str(), _rawData)))
		std::cerr << "\n Error writing original data to a file.";

EndPhase:
	if (_FX) std::fclose(_FX);

	vectorClean(_rawData);
	vectorClean(Cni_Info);
	vectorClean(_BitL);
	vectorClean(_Codes);
	vectorClean(Cni_Head0);
	vectorClean(Cni_Head1);
	vectorClean(cnbt);

	return _rawSize;
}


