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
static inline const std::size_t ReSync_Int(std::vector<int64_t>& _vecSqz, std::vector<char>& ReSynced_Data,
											const std::vector<int64_t>& Bits_Len,  const int64_t& IntSqz, 
											const std::vector<Can_Bit>& _CniInfo)
{
	int64_t w = 0, bx = 0;
	size_t synced_size = 0;
	const std::string xsbit = bit_str(int64_t(IntSqz) );

	const size_t bitSize = xsbit.size();
	const size_t BitsInfoSize = Bits_Len.size();

	char* xs = (char*)xsbit.c_str();
	xs[bitSize] = 0;

	std::vector<Can_Bit>& CNF = (std::vector<Can_Bit>&)_CniInfo;

	std::vector<Can_Bit>::iterator CBT;

	vectorClean(_vecSqz);

	for (size_t i = 0; i < BitsInfoSize; i++)
	{
		w = Bits_Len[i];
		bx = int_bit(lstr(xs, w));
		_vecSqz.push_back(bx);
		xs += w;
	}

	const size_t SqzSize = _vecSqz.size();

	for (size_t q = 0; q < SqzSize; q++)
	{
		if (mix::generic::vector_search(CNF.begin(), CNF.end(), _vecSqz[q],
											mix::generic::NLess<int64_t>(), CBT))
		{
			ReSynced_Data.push_back(CBT->_xData); ++synced_size;
		}
	}

	NULLP(xs);
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
static inline const std::size_t readPackInfo(const std::string& _inFile, std::vector<_Canonical>& Canie ,std::vector<_Canonical>& CnSrc)
{
	intmax_t _x = 0;
	Can_Bit cbi = {};
	size_t _readSize = 0, infSize = 0;
	std::FILE* _Fr = std::fopen(_inFile.data(), "rb");

	if (!_Fr)
	{
		std::cerr << "\n Error read accessed to file. ";
		return 0;
	}

	// Read the records's size number
	if (!std::fread(&infSize, sizeof(size_t), 1, _Fr))
	{
		std::cerr << "\n Error read header size information.";
		if (_Fr) std::fclose(_Fr);
		return 0;
	}

	const size_t header_size = infSize;

	// picking up codewords length ..
	for (size_t f = 0; f < header_size; f++)
	{
		cbi._bitLen = std::fgetc(_Fr);
		if (cbi._bitLen > 0)
		{
			Canie.push_back(cbi); ++_readSize;
		}
		cbi = {};
	}

	// read the records' size
	if (!std::fread(&infSize, sizeof(size_t), 1, _Fr))
	{
		std::cerr << "\n Error read header size information.";
		if (_Fr) std::fclose(_Fr);
		return 0;
	}

	// picking up RLE Info for each codeword length ..
	for (size_t r = 0; r < header_size; r++)
	{
		_x = std::fgetc(_Fr);
		if (_x > 0)
		{
			Canie[r]._rle_bit_len = _x;
			++_readSize;
		}
	}


	// Reads up size information ..
	if (!std::fread(&infSize, sizeof(size_t), 1, _Fr))
	{
		std::cerr << "\n Error read header info data.";
		if (_Fr) std::fclose(_Fr);
		return 0;
	}

	const size_t cni_header_size = infSize;

	cbi = {};

	// picking up encoded data ..
	for (size_t j = 0; j < cni_header_size; j++)
	{
		cbi._xData = std::fgetc(_Fr);
		CnSrc.push_back(cbi); ++_readSize;
		cbi = {};
	}

	if (_Fr) std::fclose(_Fr);
	return _readSize;
}



// Packed the raw data source and saves it to a file.
static inline const std::size_t writePack(std::FILE*& _fHandle, const intmax_t& _vPacked)
{
	size_t _writtenSize = 0;
	size_t _PckSize = len_bit(intmax_t(_vPacked)) / 8;

	if (!_fHandle) {
		std::cerr << "\n Error Open File !! \n";
		return 0;
	}

	if (!(_writtenSize = std::fwrite(&_PckSize, sizeof(_PckSize), 1, _fHandle))) {
		std::cerr << "\n Error saving number of total bytes' size \n";
		if (_fHandle) std::fclose(_fHandle);
		return 0;
	}

	_writtenSize += save_cni_bit(_fHandle, _vPacked);

	return _writtenSize;
}



// Read the packed data source to a int Vector.
static inline const std::size_t readPack(std::FILE*& _fHandle, std::vector<int64_t>& vInts)
{
	std::size_t _readBytes = 0;

	_readBytes = read_cni_bit(_fHandle, vInts);

return _readBytes;
}



// write the original uncompressed form of the data into a file.
static inline const std::size_t writeOriginal(const std::string& _OriginFile, const std::vector<char>& raw_dat)
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
static inline const int64_t Gen_Encoding_Info(std::vector<unsigned char>& _Src, std::vector<BPAIR>& CodInfo, 
										      std::vector<_Canonical>& Cni_Dat, std::vector<intmax_t>& PacInts,
										      const double& cmp_rate, const char& _cCode = 'u')
{
	intmax_t SqzInt = 0;
	std::priority_queue<node, std::vector<node>, std::less<node>> _pq = {};
	std::priority_queue<node, std::vector<node>, fqLess> _fpq = {};
	std::vector<node> PNodes = {};

	Can_Bit cbi = {};

	std::vector<_Canonical> Cni_Info = {};
	std::vector<Can_Bit> CniBits = {};
	std::vector<Can_Bit>::iterator CBiT = {};

	std::string xs_bit = "\0";

	const size_t T_SIZE = _Src.size();

	for (size_t t = 0; t < T_SIZE; t++)
	{
		PNodes.push_back(node(UC(_Src[t])));
	}

	if (_cCode == 'D') {
		PRINT("Nodes data..");
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
		PRINT("Filtered Nodes Data..");
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
		PRINT("Frequency nodes data..");
		for (const auto& _e : PNodes) RPRINTC(_e.dataValue());
		RET;
	}

	
	_TREE::plot_tree(PNodes, cmp_rate);

	CodInfo = _TREE::CodeMap(); // huffman encoding info generated

	if (_cCode == 'D')
	{
		PRINT("Generated Huffman Encoding Information. (BPAIR)");
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
		Cni_Dat.push_back(cbi);
		cbi = {};
	}

	if (_cCode == 'D') {
		PRINT("Raw Canonical Data Obtained from the Generated BPAIR. (_Canonical) ");
		PRINT("_xData <=> _bitLen");
		for (const auto& ce : Cni_Dat)
		{
			RPRINTC(ce._xData); RPRINTC(ce._bitLen); RET;
		}
	}
	
		mix::generic::fast_sort(Cni_Dat.begin(), Cni_Dat.end(), mix::generic::NLess<char>());
		mix::generic::fast_sort(Cni_Dat.begin(), Cni_Dat.end(), mix::generic::NLess<intmax_t>());
	
	if (_cCode == 'D')
	{
		PRINT("Sorted Canonical Encoding Information Data.. (sorted _Canonical)" );
		PRINT("_xData <=> _bitLen");
		for (const auto& _cb : Cni_Dat)
		{
			RPRINTC(_cb._xData); RPRINTC(_cb._bitLen); RET;
		}

		RET;
	}

	_Gen_Canonical_Info(Cni_Info, Cni_Dat); // obtaining codewords based on bit length info ..

	cni_enforce_unique(Cni_Info); // codewords data updated

	if (_cCode == 'D') {
		PRINT("Canonical Huffman Encoding Information Generated ! (_Gen_Canonical_Info() called.)" );
		PRINT("_xData <=> _codeWord <=> _bitLen");

		for (const auto& ci : Cni_Info)
		{
			RPRINTC(ci._xData); RPRINTC(ci._codeWord); RPRINTC(ci._bitLen);
			RET;
		}
	}

	cbi = {};

	for (const _Canonical& cni : Cni_Info)
	{
		cbi._xData = cni._xData;
		cbi._codeWord = cni._codeWord;
		CniBits.push_back(cbi);
		cbi = {};
	}

	mix::generic::t_sort(CniBits.begin(), CniBits.end(), 0.25, mix::generic::NLess<char>());

	if (_cCode == 'D') {
		PRINT(" Finalized Sorted(char) Canonical Encoding Information Generated .. [ std::vector<Can_Bit>]");
		PRINT(" _xData <=> _codeWord <=> _bitLen");
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

	SqzInt = cni_bits_pack(PacInts);
	
	return SqzInt;
}


// Generate a header data information ..
static const size_t Gen_Cni_Header_Info(std::vector<_Canonical>& header0_info, 
										std::vector<_Canonical>& header1_info,
										const std::vector<_Canonical>& CniSrc)
{
	size_t header0_size = 0, header1_size = 0, _vi = 0;
	intmax_t _b = 0;
	std::vector<node> _vNods;
	node _e = 0;

	Can_Bit cni_head0 = {}, cni_head1 = {};

	for (const auto& _ci : CniSrc)
	{
		_b = _ci._bitLen;
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

	// the required information (_bitLen & _rle_bit_len) is gathered.

	const size_t _InfoSize = _vNods.size();

	vectorClean(header0_info);

	// writing codewords length..
	for (size_t d = 0; d < _InfoSize; d++)
	{
		_b = _vNods[d].Value();   // bit length info
		cni_head0._bitLen = _b;

		header0_info.push_back(cni_head0);
		++header0_size;
		cni_head0 = {};
	}

	// writing RLE info for each codeword length ..
	for (size_t f = 0; f < _InfoSize; f++)
	{
		_b = _vNods[f].FrequencyData();

		if (_b > 0)
		{
			header0_info[f]._rle_bit_len = _b;
			++header0_size;
		}
	}

	const size_t CndSize = CniSrc.size();

	vectorClean(header1_info);

	// acquiring the encoded character & the bit length for each encoded character..
	for (size_t d = 0; d < CndSize; d++)
	{
		cni_head1._xData = CniSrc[d]._xData; // encoded character
		header1_info.push_back(cni_head1);
		++header1_size;
		cni_head1 = {};
	}


	// writing bit length info ..
	for (size_t cx = 0; cx < CndSize; cx++)
	{
		_b = CniSrc[cx]._bitLen;
		header1_info[cx]._bitLen = _b;
		++header1_size;
	}

	return header0_size + header1_size;
}


// Writing header information to a file.
static inline const size_t Write_Header( const std::string& _sqzFile,
										 std::FILE*& _fHandleRef,
										 const std::vector<_Canonical>& cn_head0,
										 const std::vector<_Canonical>& cn_head1,
										 char&& _xDebug = 'u')
{
	 _fHandleRef = std::fopen(_sqzFile.c_str(), "wb");

	if (!_fHandleRef)
	{
		std::cerr << "\n Error Open Write-Access To File. \n";
		return 0;
	}
	
	std::size_t _fSize = 0 , i_Size = 0;
	std::vector<unsigned char> _bit_length_info = {}, _codew_info = {};
	int _uc = 0;

	// acquiring bit lengths information ..
	for (const auto& _hd0 : cn_head0) {
		_uc = (int)_hd0._bitLen;
		_bit_length_info.push_back( _uc ); // bit length of the previous generated huffman codes.

	}

	if (_xDebug == 'D') {
		RET; mix::generic::STL_Print(_bit_length_info.begin(), _bit_length_info.end(), RPRINTC<int>); RET;
	}


	// saving bit lengths information ..
	if (!(i_Size = writePackInfo(_fHandleRef, _bit_length_info))) {
		std::cerr << "\n Error saving bit-length information.. \n";
		if (_fHandleRef) std::fclose(_fHandleRef);
		return 0;
	}

	_fSize += i_Size;
	vectorClean(_bit_length_info);

	// acquiring RLE of bit lengths ..
	for (const auto& _h0d : cn_head0) {
		_uc = (int)_h0d._rle_bit_len;
		_bit_length_info.push_back(_uc);
	}

	if (_xDebug == 'D') {
		RET; mix::generic::STL_Print(_bit_length_info.begin(), _bit_length_info.end(), RPRINTC<int>); RET;
	}

	// saving RLE information of bit lengths ..
	if (!(i_Size = writePackInfo(_fHandleRef, _bit_length_info))) {
		std::cerr << "\n Error saving RLE Information of bit lengths .. \n";
		if (_fHandleRef) std::fclose(_fHandleRef);
		return 0;
	}

	_fSize += i_Size; vectorClean(_bit_length_info);

	// Acquiring the encoded characters ..

	vectorClean(_codew_info);

	for (const auto& _hd1 : cn_head1)
		_codew_info.push_back(_hd1._xData); // previous generated huffman encoded character.

	if (_xDebug == 'D') {
		RET; mix::generic::STL_Print(_codew_info.begin(), _codew_info.end(), RPRINTC<char>); RET;
	}

	
	// saving encoded characters information ..
	if (!(i_Size = writePackInfo(_fHandleRef, _codew_info))) {
		std::cerr << "\n Error saving encoded characters information .. \n ";
		if (_fHandleRef) std::fclose(_fHandleRef);
		return 0;
	}

	_fSize += i_Size; vectorClean(_codew_info);
	
	return _fSize;
}



// extracting a saved encoding data into 'CnBit' and 'CnRaw' vectors
static inline const int extract_encoding_info(const std::string& xFile,std::vector<_Canonical>& CnBit, std::vector<_Canonical>& CnRaw)
{
	if (!readPackInfo(xFile.c_str(), CnBit, CnRaw))
	{
		std::cerr << "\n Error read encoding information ! " << "\n\n";
		return 0;
	}

	return -1;
}



static inline const bool Compress(const std::string& _destF, const std::string& _srcF, const double& compRate, unsigned char* _cBuff)
{
	bool _bDone = 0;
	int _cF = 0, _CountStr = 0;
	char* _copyOne = nullptr;
	const char* _sExt = "\0";

	std::vector<unsigned char> _srcData = {};
	std::vector<intmax_t> _pacInts = {};

	std::vector<BPAIR> _CodeMap = {};
	std::vector<_Canonical> CniHead0 = {}, CniHead1 = {}, _CanSrc = {};
	
	std::FILE* _FO = std::fopen(_srcF.data(), "rb"), *_FHandleRef = nullptr;
	std::size_t F_SIZE = 0, HCN_SIZE = 0, PACKED_HEADER_SIZE = 0;
	intmax_t _sqzNum = 0;

	if (!_FO)
	{
		std::cerr << "\n Error Read Source File. " << "\n\n";
		goto finishedDone;
	}

	_cF = strNPos(_srcF.c_str(), '.');

	if (_destF.empty())
	{
		_copyOne = (char*)lstr(_srcF.data(), _cF).c_str();
		_copyOne = (char*)concat_str(_copyOne, ".sqz");
		goto CoreProcesses;
	}


	if ( (_cF = strNPos(_destF.data(), '.')) > -1)  // if the file has an extension
	{
		_sExt = rstr(_destF.data(), 3).c_str();

		if (std::strncmp(_sExt, "sqz", 3)) // if the extension string not equal 'sqz'
		{
			_sExt = "sqz";
			_CountStr = (int)(_destF.size() - 3); // Locate the first encountered position of the extension's name
			_copyOne = (char*)tapStrBy(_destF.data(), _sExt, _CountStr - 1).c_str();
		}
	}
	else {

		// if no extension specified to the file
		_sExt = ".sqz";
		_copyOne = (char*)concat_str((char*)_destF.data(), _sExt);
	}


	CoreProcesses:

	if (!(F_SIZE = readOriginal(_FO, _srcData, _cBuff)) )
	{
		RET;
		std::cerr << "\n Error read source file." << "\n\n";
		goto finishedDone;
	}
	
	
	_sqzNum = Gen_Encoding_Info(_srcData, _CodeMap, _CanSrc, _pacInts, COMP_RATE);
	// _srcData is fully filled with correct data
	// _CodeMap is generated successfully
	// _pacInts is fully filled with correct values

	HCN_SIZE = Gen_Cni_Header_Info(CniHead0,CniHead1,_CanSrc);
	// CniHead0 & CniHead1 are fully filled with correct data
	
	PACKED_HEADER_SIZE = Write_Header(_destF.c_str(), _FHandleRef, CniHead0, CniHead1);
	// encoding information successfully saved !
	

	// writing packed data source into a file ( *.sqz ).
	if ( !(_bDone = writePack(_FHandleRef, _sqzNum)))
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

	
	NULLP(_copyOne);
	if (!_SystemFile.empty()) _SystemFile.clear();
	if (_FO) std::fclose(_FO);
	if (_FHandleRef) std::fclose(_FHandleRef);

	_TREE::Clean();
	F_SIZE = 0;
	return _bDone;
}



static inline const std::size_t UnCompress(const std::string& _packedFile, const std::string& _unPackedFile)
{
	intmax_t cmb_bit = 0, _bi = 0, _x = 0;
	Can_Bit cnbi = {};
	
	std::vector<char> RawDat = {};
	std::vector<_Canonical> _Canonic = {}, _Canine = {}, _CanDat = {};
	std::vector<Can_Bit> vCnbi = {};

	std::size_t ReckonSize = 0, _UnSquezzed = 0;
	std::vector<intmax_t> cniBitLen, _SqzInts;

	char* _OriginFile = (char*)_unPackedFile.c_str();

	extract_encoding_info(_packedFile.c_str(), _Canonic, _Canine);
	// _Canonic & _Canine are successfully filled with the correct data.
	
	const size_t _CniSize = _Canonic.size(); // filled with bit length & RLE of bit length
	const size_t _CneSize = _Canine.size(); // filled with encoded chars 

	for (size_t i = 0; i < _CneSize; i++) {
		if (i > (_CniSize - 1) ) _Canonic.push_back(_Canine[i]);
		else _Canonic[i]._xData = _Canine[i]._xData;
	}

	// _Canonic is fully filled with correct data (_xData + _bitLen + _rle_bit_len);

	const size_t _CanSize = _Canonic.size();

	// RLE method unravels each number of repeated bit-length ..
	for (size_t g = 0, f = 0; f < _CanSize ; f++)
	{
		_bi = _Canonic[f]._bitLen;
		_x = _Canonic[f]._rle_bit_len;

		// t = 0 -> number of repeated bit-length ..
		for (intmax_t t = 0; t < _x; t++)
		{
			if (g < _CanSize) cniBitLen.push_back(intmax_t(_bi));
			++g;
		}
	}

	for (size_t z = 0; z < _CanSize; z++) _Canonic[z]._bitLen = cniBitLen[z];

	mix::generic::fast_sort(_Canonic.begin(), _Canonic.end(), mix::generic::NLess<char>());
	mix::generic::fast_sort(_Canonic.begin(), _Canonic.end(), mix::generic::NLess<intmax_t>());
	/*
		for (const auto& _cn : _Canonic) {
		RPRINTC(_cn._xData); RPRINTC(_cn._bitLen); RET;
		}

		RET;
		return 0;
	*/

	_Gen_Canonical_Info(_CanDat, _Canonic); cni_enforce_unique(_CanDat);
	/*
		for (const auto& _cda : _CanDat) {
			RPRINTC(_cda._xData); RPRINTC(_cda._codeWord); RET;
		}

		RET;
		return 0;
	*/


	// filling up the most important data needed in 'ReSync_Int() ' calls..
	for (const _Canonical& cnd : _CanDat)
	{
		cnbi._xData = cnd._xData;
		cnbi._codeWord = cnd._codeWord;
		vCnbi.push_back(cnbi);
		cnbi = {};
	}

	for (const auto& _cbi : vCnbi) {
		RPRINTC(_cbi._xData); RPRINTC(_cbi._codeWord); RET;
	}

	RET;

	return 0;

	PRINT("\n sorting integers of symbols.. ");
	mix::generic::t_sort(vCnbi.begin(), vCnbi.end(), 0.25, mix::generic::NLess<intmax_t>());

	const size_t cnfSize = _Canonic.size();

	
	

	
	PRINT("\n rematching integers with data .. ");

	ReckonSize = ReSync_Int(_SqzInts, RawDat, cniBitLen, cmb_bit, vCnbi);

	
	if (!ReckonSize)
		std::perror("\n\n it seems like something error has happened .. \n\n");


	if (!(_UnSquezzed = writeOriginal(_OriginFile, RawDat)))
	{
		std::cerr << "\n Error writing uncompressed data format.";
	}



	vectorClean(_Canonic);
	vectorClean(_Canine);
	vectorClean(_CanDat);
	vectorClean(vCnbi);
	vectorClean(RawDat);
	vectorClean(_SqzInts);
	vectorClean(cniBitLen);


	return _UnSquezzed;
}


