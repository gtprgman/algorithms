#pragma once
/* Using License GPL v 3.0. */

#ifndef MX_HUFF_IMPLS
	#define MX_HUFF_IMPLS

#endif


const size_t _ROWSZ = 80;
static std::string _SystemFile = "\0";


constexpr double COMP_RATE = 0.52; /* 0.52 is the default value, the users are allowed to tweak it
				      in the command line */


// ReSync the integers data of the read *.sqz using information from the read packed symbols table *.tbi.
static inline const std::size_t ReSync_Int(std::vector<int>& _SqzInt, const std::vector<packed_info>& _ReadPac)
{
	int len_bit = 0, _dat = 0;
	std::size_t _Synced_Sz = 0;
	const std::size_t _SqzSize = _SqzInt.size();

	for (size_t i = 0,j = 0; (i < _SqzSize && j < _SqzSize); i++,j++)
	{
		_dat = _SqzInt[i];

		if (!_dat) continue;

		len_bit = num_of_bits<int>::eval(_dat);

		if (len_bit != _ReadPac[j]._BITLEN)
		{
			if ((_ReadPac[j]._BITLEN > BYTE ) && 
				(_ReadPac[j]._BITLEN <= WORD) ) // 16 bits
			{
				_dat = _SqzInt[i] << 8;
				_dat |= _SqzInt[i + 1];
				_SqzInt[i] = _dat;
				_SqzInt[i + 1] = 0;
				i += 2;
				_Synced_Sz += 2;
			}
			else if ( (_ReadPac[j]._BITLEN > WORD) &&
				      (_ReadPac[j]._BITLEN <= DWORD) )
			{
				_dat = _SqzInt[i] << 24;
				_dat |= _SqzInt[i + 1] << 16; _SqzInt[i + 1] = 0;
				_dat |= _SqzInt[i + 2] << 8; _SqzInt[i + 2] = 0;
				_dat |= _SqzInt[i + 3]; _SqzInt[i + 3] = 0;
				_SqzInt[i] = _dat;

				i += 4;
				_Synced_Sz += 4;
			}
		}
		_dat = 0;
	}

	return _Synced_Sz;
}




/* Using the information in _SrcPck to decompose each bit in _SqzPack.
   NB: 'ReSync_Int()' is assumed to be called first on _SqzPack before calling 'Unpack_Bits()' */
static inline const std::size_t UnPack_Bits(std::vector<int>& _SqzPack, std::vector<packed_info>& _SrcPck)
{
	std::size_t unPck_Size = 0;
	const std::size_t _SqzSize = _SqzPack.size();
	packed_info _tmpPck = {};
	std::vector<int> _CopyBits = {};

	int l_bit = 0, r_bit = 0, elem1 = 0, elemX = 0, elem2 = 0,
		tot_bit = 0, bit_difft = 0;

	for (size_t q = 0; q < _SqzSize; q++)
	{
		if (!_SqzPack[q]) continue;

		_tmpPck = _SrcPck[q];

		if (_tmpPck._BITLEN > 0) l_bit = _tmpPck.L_BIT; else continue;

		tot_bit = num_of_bits<int>::eval(_SqzPack[q]);
		bit_difft = tot_bit - l_bit;
		r_bit = bit_difft;

		elem1 = unPack(_SqzPack[q], l_bit, bit_difft);
		elemX = elem1 << bit_difft;
		elem2 = elemX ^ _SqzPack[q];

		_CopyBits.push_back(elem1);
		_CopyBits.push_back(elem2);
		unPck_Size += 2;

		_tmpPck = {};
		tot_bit = 0; l_bit = 0; r_bit = 0; bit_difft = 0;
		elemX = 0; elem1 = 0; elem2 = 0;
	}

	if (!_SqzPack.empty()) _SqzPack.clear();
	_SqzPack = {};

	for (const int& _e : _CopyBits)
		_SqzPack.push_back(_e);

	if (!_CopyBits.empty()) _CopyBits.clear();
	_CopyBits = {};


return unPck_Size;
}



static inline void filter_pq_nodes(std::vector<node>& _target, std::priority_queue<node>& _Pqueue)
{
	std::size_t _Cnt = 0;
	node _nod = 0;
	double _fqr = 0;


	while (!_Pqueue.empty())
	{
		_nod = _Pqueue.top();
		_Pqueue.pop();

		if (_target.empty() && _nod() != 0)
		{
			_target.push_back(_nod);
			continue;
		}

		if (_target[_Cnt] == _nod)
		{
			_target[_Cnt].setFrequencyData(++_fqr);
		}
		else
		{
			if (_nod != 0)
			{
				_fqr = 1;
				_nod.setFrequencyData(_fqr);
				_target.push_back(_nod);
				_Cnt = _target.size() - 1;
			}
		}
	}
		
}



inline void _TREE::create_encoding(const int _From, 
				   const int _To,
				   int _bt,
				   const std::vector<node>& _Vn)
{
	node _e = '0';
	int _Dir = 0, _recurr = 0, _sameVal = 0, _prevX = 0;
	static double _fq = 100.00;
	std::vector<BPAIR>::iterator _iGet;

	// Processing the Encoding from vector data
	for (int i = _From; i < _To; i++)
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

	filterPhase:

		_sameVal = _bt;
		_prevX = _sameVal;

		if (mix::generic::
			vector_search(_vPair.begin(), _vPair.end(), _sameVal, bitLess(), _iGet))
			//if (std::binary_search(_vPair.begin(), _vPair.end(),_bpr))
		{
			_sameVal = _iGet->_val;
			_sameVal += (int)((_iGet - _vPair.begin()) / 2);
			_bt = _sameVal;
			_prevX = _sameVal;
		}

		_vPair.push_back({ _e.dataValue(), _prevX });
		mix::generic::fast_sort(_vPair.begin(), _vPair.end(), bitLess());
		//std::stable_sort(_vPair.begin(), _vPair.end());
	}
	_fq = 0;
}




inline void _TREE::schema_Iter(const std::vector<node>& _fpNods, const double _cmpRate = 0)
{
	const int _TreeSizes = (int)_fpNods.size();

	const double _CompRate = (_cmpRate)? std::floor(_cmpRate*_TreeSizes) :
					     std::floor(COMP_RATE*_TreeSizes);

	const double _fCompRate = std::ceil((double)_TreeSizes / _CompRate);
	int _DivSize = (int)_fCompRate;
	int _msk = 0, _BT = 2, _Dir = L;
	

	if (!_vPair.empty()) _vPair.clear();

	for (int t = 0; t < _TreeSizes; t += _DivSize)
	{
		if ( (_TreeSizes - t) < _DivSize ) _DivSize = 1;
		create_encoding(t, (t + _DivSize), _msk, _fpNods);

		_msk ^= _BT--;

		if (_BT < 1) 
			_BT = 2; 
			

		_Dir = _Dir ^ 1;
		_msk = 0;
		_msk |= _Dir;
	}

	mix::generic::t_sort(_vPair.begin(), _vPair.end(), 0.25, bitLess());
	_TREE::enforce_unique(_vPair); 
}




inline void _TREE::enforce_unique(std::vector<BPAIR>& _bPairs)
{
	int _Addend = 0;
	int _MaxSz = (int)_bPairs.size();

	for (int n = 0, m = 0; m < _MaxSz; m++)
	{
		_Addend = m;
		_bPairs[m]._val += (_Addend + 1);
		_Addend = 0;
	}
}



// Save the encoded's information data table into a file.
static inline const std::size_t writePackInfo(const std::string& _fiName, const std::vector<BPAIR>& _pacData, const std::vector<packed_info>& _nfoPac)
{
	packed_info _Pack = {};

	std::size_t _codesWritten = 0;
	const std::size_t _CODESIZE = _pacData.size(), _PACKSIZE = _nfoPac.size();
	std::FILE* _FPack = std::fopen(_fiName.data(), "wb");

	if (!_FPack)
	{
		std::cerr << "\n Error writing packed information! Could not proceed. \n\n";
		return 0;
	}
	else if (!std::fwrite(&_CODESIZE, sizeof(_CODESIZE), 1, _FPack)) {
		std::cerr << "\n Incorrect 'BPAIR' Records' size! Could not proceed \n\n";
		if (_FPack) std::fclose(_FPack);
		return 0;
	}

	// Writing 'BPAIR' Records to file
	for (size_t c = 0; c < _CODESIZE; c++)
	{
		if (((int)_pacData[c]._data) > 0)
		{
			std::fputc(_pacData[c]._data, _FPack);
			std::fputc(_pacData[c]._val, _FPack);
			++_codesWritten;
		}
	}
	
	if (!std::fwrite(&_PACKSIZE, sizeof(_PACKSIZE), 1, _FPack))
	{
		std::cerr << "\n  Incorrect 'packed_info' records' size! Could not proceed. \n\n";
		if (_FPack) std::fclose(_FPack);
		return 0;
	}

	// Writing 'packed_info' records right after the written blocks of 'BPAIR' data
	for (size_t x = 0; x < _PACKSIZE; x++)
	{
		if (_nfoPac[x]._BITLEN > 0)
		{
			std::fputc(_nfoPac[x]._BITLEN, _FPack);
			std::fputc(_nfoPac[x].L_BIT, _FPack);
			++_codesWritten;
		}
	}
	
	if (_FPack) std::fclose(_FPack);

	return _codesWritten;
}



// Read the encoded information data table from a file.
static inline const std::size_t readPackInfo(const std::string& _inFile, std::vector<BPAIR>& _ReadVector)
{
	BPAIR _Bpr = {};
	int l_bit = 0,bit_len = 0;

	std::size_t _symbolsRead = 0, _bpair_Recs = 0, _packed_Recs = 0;
	std::FILE* _FR = std::fopen(_inFile.data(), "rb");
	
	if (!std::fread(&_bpair_Recs, sizeof(size_t), 1, _FR))
	{
		std::cerr << "\n 'BPAIR' records' size read error." << "\n\n";
		if (_FR) std::fclose(_FR);
		return 0;
	}

	const std::size_t _bpair_size = _bpair_Recs;

	for (size_t c = 0; c < _bpair_size; c++)
	{
		_Bpr._data = std::fgetc(_FR);
		_Bpr._val = std::fgetc(_FR);
		if ( ((int)_Bpr._data) > 0 ) _ReadVector.push_back(_Bpr);
		_Bpr = {}; ++_symbolsRead;

	}

	if (!std::fread(&_packed_Recs, sizeof(size_t), 1, _FR))
	{
		std::cerr << "\n 'packed_info' records size read error. " << "\n\n";
		if (_FR) std::fclose(_FR);
		return 0;
	}

	const std::size_t _PackedInfo_Size = _packed_Recs;
	_ReadVector.resize(_PackedInfo_Size + _bpair_size);

	for (size_t k = 0; k < _PackedInfo_Size; k++)
	{
		bit_len = std::fgetc(_FR);
		l_bit = std::fgetc(_FR);

		if (bit_len > 0) {
			_ReadVector[k]._PacInfo._BITLEN = bit_len;
			_ReadVector[k]._PacInfo.L_BIT = l_bit;
			++_symbolsRead;
		}
		bit_len = 0; l_bit = 0;
	}


	if (_FR) std::fclose(_FR);

return _symbolsRead;
}




// Packed the raw data source and saves it to a file.
static inline const std::size_t writePack(const std::string& _File, const std::vector<packed_info>& _PackNfo)
{
	std::size_t _packedSize = 0;
	const std::size_t _PckSz = _PackNfo.size();
	std::FILE* _FC = std::fopen(_File.data(), "wb");

	if (!_FC)
	{
		std::cerr << "\n Error opened " << _File.data() << " to write. " << "\n\n";
		return 0;
	}
	
	int data_bits = 0,bit_len = 0 ,_Dat = 0;


	for (size_t x = 0; x < _PckSz; x++)
	{
		_Dat = _PackNfo[x]._PACKED;
		bit_len = _PackNfo[x]._BITLEN;

		if (bit_len <= BYTE)
		{
			if (_Dat > 0) {
				std::fputc(_Dat, _FC);
				++_packedSize;
			}
		}
		else if ((bit_len > BYTE) && (bit_len <= WORD)) // 16 bit
		{
			std::fputc(BYTE_PTR_HI(_Dat) >> 8, _FC);
			std::fputc(BYTE_PTR_LO(_Dat), _FC);
			++_packedSize;
		}
		else if (bit_len > WORD) // 32 bit
		{
			data_bits = WORD_PTR_HI(_Dat) >> 16;

			std::fputc(BYTE_PTR_HI(data_bits) >> 8, _FC);
			std::fputc(BYTE_PTR_LO(data_bits), _FC);
			++_packedSize;

			data_bits = WORD_PTR_LO(_Dat);

			std::fputc(BYTE_PTR_HI(data_bits) >> 8, _FC);
			std::fputc(BYTE_PTR_LO(data_bits), _FC);
			++_packedSize;
		}
		else if (bit_len <= 0) continue;

		_Dat = 0; bit_len = 0;
	}

	if (_FC) std::fclose(_FC);

	return _packedSize;
}



// Read the packed data source to a int Vector.
static inline const std::size_t readPack(const std::string& _inFile, std::vector<int>& _inVec)
{
	std::size_t _readByte = 0;
	std::FILE* _FRP = std::fopen(_inFile.data(), "rb");
	int _C = 0;

	if (!_FRP)
	{
		std::cerr << "\n Error opened " << _inFile.data() << "to read. " << "\n\n";
		return 0;
	}

	while ((_C = std::fgetc(_FRP)) > 0)
	{
		_inVec.push_back(_C);
		++_readByte;
	}
	
	if (_FRP) std::fclose(_FRP);

return _readByte;
}



// write the original uncompressed form of the data into a file.
static inline const std::size_t writeOriginal(const std::string& _OriginFile, const std::vector<int>& _intSrc, const std::vector<BPAIR>& _codeInfo)
{
	
	std::size_t _wordsWritten = 0;
	const std::size_t _codeSize = _codeInfo.size();
	std::FILE* _fOrig = std::fopen(_OriginFile.data(), "wb");
	std::vector<BPAIR>::iterator _bIt;
	std::vector<BPAIR> _codTab = _codeInfo;

	if (!_fOrig)
	{
		std::cerr << "\n Error writing file.. !! " << "\n\n";
		goto EndWrite;
	}
	
	mix::generic::t_sort(_codTab.begin(), _codTab.end(), 0.25, bitLess());

	for (int const& _i : _intSrc)
	{
		if ( _i > 0 )
		{
			if (mix::generic::vector_search(_codTab.begin(), _codTab.end(), _i, bitLess(), _bIt))
			{
				std::fputc((int)_bIt->_data, _fOrig);
				++_wordsWritten;
			}
			else std::cerr << "\n Bit for integer: " << _i << " not Found!!" << "\n\n";
		}
	}

EndWrite:
	
	if (_fOrig) std::fclose(_fOrig);
	_codTab.clear();

	return _wordsWritten;
}



// read the source original uncompressed form of a file
static inline const std::size_t readOriginal(std::FILE*& _fHandle , std::vector<char>& _vecData, char*& _DataSrc)
{
	std::size_t _wordsRead = 0;
	
	if (!_fHandle)
	{
		std::cerr << "\n Error reading source file.. !! " << "\n\n";
		goto EndRead;
	}

	if (!_DataSrc) _DataSrc = new char[_ROWSZ];
	
	while (std::fgets(_DataSrc, _ROWSZ, _fHandle))
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




static inline const bool Compress(const std::string& _destF, const std::string& _srcF, const double& compRate, char* _cBuff)
{
	bool _bDone = 0;
	int _cF = 0, _CountStr = 0;
	char* _copyOne = nullptr;
	const char* _sExt = "\0";
	bitInfo<int> _bIF = {};

	std::vector<char> _srcData = {};
	std::vector<node> nodes = {};
	std::vector<BPAIR> _CodeMap = {};
	std::vector<BPAIR>::iterator _BiT;
	std::vector<bitInfo<int>> _vbI = {};
	std::vector<packed_info> vpck0 = {};

	
	std::priority_queue<node, std::vector<node>, std::less<node>> _pq;
	std::priority_queue<node, std::vector<node>, fqLess> _fpq;

	std::FILE* _FO = std::fopen(_srcF.data(), "rb");
	std::size_t F_SIZE = 0;


	if (!_FO)
	{
		std::cerr << "Error opened input source file. " << "\n\n";
		goto finishedDone;
	}


	_SystemFile.assign(_srcF);


	if ((_cF = strNPos(_SystemFile.data(), '.')) > -1) // if source file has extension
	{
		_SystemFile.assign(lstr(_srcF.data(), _cF));
		_SystemFile = (char*)concat_str((char*)_SystemFile.data(), ".tbi");
	}
	else
		_SystemFile = (char*)concat_str((char*)_SystemFile.data(), ".tbi");
	


	if (_destF.empty())
	{
		_copyOne = (char*)lstr(_srcF.data(), _cF);
		_copyOne = (char*)concat_str(_copyOne, ".sqz");
		goto CoreProcesses;
	}


	if ( (_cF = strNPos(_destF.data(), '.')) > -1)  // if the file has an extension
	{
		_sExt = rstr(_destF.data(), 3);
		_sExt = reverse_str(_sExt);

		if (std::strncmp(_sExt, "sqz", 3)) // if the extension string not equal 'sqz'
		{
			_sExt = "sqz";
			_CountStr = (int)(_destF.size() - 3); // Locate the first encountered position of the extension's name
			_copyOne = (char*)tapStrBy(_destF.data(), _sExt, _CountStr - 1);
		}
	}
	else {

		// if no extension specified to the file
		_sExt = ".sqz";
		_copyOne = (char*)concat_str((char*)_destF.data(), _sExt);
	}


	CoreProcesses:

	if (!(F_SIZE = readOriginal(_FO,_srcData, _cBuff)) )
	{
		RET;
		std::cerr << "Error read source file." << "\n\n";
		goto finishedDone;
	}
	
	if (_FO) std::fclose(_FO);

	
	for (std::size_t _i = 0; _i < F_SIZE; _i++)
	{
		node _C = _srcData[_i];
		_pq.emplace(_C);
	}
	

	filter_pq_nodes(nodes, _pq);
	
	for (auto const& _eV : nodes)
		_fpq.push(_eV);


	if (!nodes.empty()) nodes.clear();

	for (node _nF = 0; !_fpq.empty(); )
	{
		_nF = _fpq.top();
		nodes.push_back(_nF);
		_fpq.pop();
	}

	_TREE::plot_tree(nodes, compRate);

	_CodeMap = _TREE::CodeMap();

	_bIF = {};
	_vbI = {};
	vpck0 = {};

	/* Working on the bare raw data source ..
	   '_CodeMap '  is the previous generated encoding information data table. */
	mix::generic::t_sort(_CodeMap.begin(), _CodeMap.end(),0.25,chrLess());

	// Find the encoding match to pair to each data in '_iBuffer' and save the encoded data to a file.
	for (std::size_t _g = 0; _g < F_SIZE; _g++)
	{
		if (mix::generic::vector_search(_CodeMap.begin(), _CodeMap.end(), _srcData[_g], chrLess(), _BiT))
		{
			_bIF.X = _BiT->_val; // encoded bit
			_bIF._Alpha = _BiT->_data; // data
			_bIF.numBits = num_of_bits<int>::eval(_bIF.X);
			_vbI.push_back(_bIF);
			_bIF = {};
		}
	}

	bitsPack(_CodeMap, _vbI); // packed the raw data source

	if (_destF.empty())
	{
		RET;
		std::cerr << "The File's name for the compressed one could not be empty. " << "\n\n";
		goto finishedDone;
	}

	
	for (BPAIR const& _bpc : _CodeMap)
	{
		vpck0.push_back(_bpc._PacInfo);
	}

	
	if (!writePackInfo(_SystemFile.data(), _CodeMap,vpck0))
	{
		RET;
		std::cerr << "Error writing encoded's information data table to a file. " << "\n\n";
		goto finishedDone;
	}


	if ( !(_bDone = writePack(_destF, vpck0)) )
	{
		RET;
		std::cerr << "Error writing packed data source !" << "\n\n";
		goto finishedDone;
	}


finishedDone:
	if (!_srcData.empty()) _srcData.clear();
	if (!nodes.empty()) nodes.clear();
	if (!_CodeMap.empty()) _CodeMap.clear();
	if (!_vbI.empty()) _vbI.clear();
	if (!vpck0.empty()) vpck0.clear();
	NULLP(_copyOne);
	if (!_SystemFile.empty()) _SystemFile.clear();
	_TREE::Clean();
	F_SIZE = 0;
	return _bDone;
}



static inline const std::size_t UnCompress(const std::string& _packedFile, const std::string& _unPackedFile)
{
	std::vector<int> _ReadInt;
	std::vector<packed_info> _ReadPck;
	std::vector<BPAIR> _ReadCodeMap;
	
	int _Cnt = 0;
	std::size_t _UnSquezzed = 0;
	char* _OriginCopy = nullptr;

	// to obtain a *.tbi file
	_SystemFile = (char*)lstr(_packedFile.data(), _packedFile.size() - 3);
	_SystemFile = (char*)concat_str((char*)_SystemFile.data(), "tbi");


	if ( !readPackInfo(_SystemFile.data(), _ReadCodeMap) ) // Read a *.tbi into vector<BPAIR>
	{
		std::cerr << "Error reading encoded's information data table ! " << "\n\n";
		goto EndStage;
	}


	for (BPAIR const& _bp : _ReadCodeMap) {
		_ReadPck.push_back(_bp._PacInfo);
	}

	
	// read a *.sqz file
	if ( !readPack(_packedFile.data(), _ReadInt) ) // read a *.sqz into vector<int>
	{
		std::cerr << "Error reading compressed file. " << "\n\n";
		goto EndStage;
	}

	
	ReSync_Int(_ReadInt, _ReadPck);

	UnPack_Bits(_ReadInt, _ReadPck);

	if (_unPackedFile.empty())
	{
		_OriginCopy = (char*)lstr(_packedFile.data(), _packedFile.size() - 3);
		_OriginCopy = (char*)concat_str(_OriginCopy, "dat");
	}
	else _OriginCopy = (char*)_unPackedFile.data();

	
	if ( !(_UnSquezzed = writeOriginal(_OriginCopy, _ReadInt, _ReadCodeMap) ) )
	{
		std::cerr << "Error writing decoded format of a file. " << "\n\n";
		goto EndStage;
	}
	

EndStage:
	NULLP(_OriginCopy);
	if (_SystemFile.size()) _SystemFile.clear();
	if (!_ReadInt.empty()) _ReadInt.clear();
	if (!_ReadCodeMap.empty()) _ReadCodeMap.clear();
	if (!_ReadPck.empty()) _ReadPck.clear();

	return _UnSquezzed;
}


