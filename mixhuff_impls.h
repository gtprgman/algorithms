#pragma once
/* Using License GPL v 3.0. */

#ifndef MX_HUFF_IMPLS
	#define MX_HUFF_IMPLS

#endif


const size_t _ROWSZ = 80;
static std::string _SystemFile = "\0";


constexpr double COMP_RATE = 0.52; /* 0.52 is the default value, the users are allowed to tweak it
					in the command line */


// ReSync the integers of *.sqz using information from the canonical symbols table *.tbi.
static inline const std::size_t ReSync_Int(std::vector<int>& _SqzInt, const std::vector<_Canonical>& _ReadPac)
{
	int len_bit = 0, cod_len = 0, _dat = 0;
	std::size_t _Synced_Sz = 0;
	const std::size_t _SqzSize = _SqzInt.size();
	const std::size_t _TbiSize = _ReadPac.size();

	for (size_t i = 0, j = 0; (i < _SqzSize && j < _TbiSize); i++, j++)
	{
		_dat = _SqzInt[i];

		if (!_dat) continue;

		len_bit = _Get_Num_of_Bits(_dat);
		cod_len = _Get_Num_of_Bits(_ReadPac[j]._codeWord);

		RPRINTC(_Get_Binary_Str(_dat)); RPRINTC(_Get_Binary_Str(_ReadPac[j]._codeWord));
		RET;

	}
	return _Synced_Sz;
}



static inline void filter_pq_nodes(std::vector<node>& _target, std::priority_queue<node>& _Pqueue)
{
	std::size_t _Cnt = 0;
	node _nod = 0;
	int _fqr = 0;


	while (!_Pqueue.empty())
	{
		_nod = _Pqueue.top();
		_Pqueue.pop();

		if (_target.empty() && _nod() != 0)
		{
			_fqr = _nod.FrequencyData();
			if (!_fqr) ++_fqr;
			_nod.setFrequencyData(_fqr);
			_target.push_back(_nod);
			continue;
		}

		if (_target[_Cnt] == _nod)
		{
			_fqr = _target[_Cnt].FrequencyData();
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
	static int _fq = 100;
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

		_vPair.push_back(BPAIR{ _e.dataValue(), _prevX });
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
static inline const std::size_t writePackInfo(const std::string& _fiName, const std::vector<_Canonical>& _CanPack)
{
	size_t pckSize = 0;
	
	std::vector<node> _vnodes;
	std::vector<_Canonical> _CniPck = _CanPack;
	const size_t _CniSize = _CniPck.size();

	std::priority_queue<node, std::vector<node>, std::less<node>> _Pqn;
	std::priority_queue<node, std::vector<node>, std::less<node>> _fPqn;

	std::FILE* _FCni = std::fopen(_fiName.data(), "wb");

	if (!_FCni)
	{
		std::cerr << "\n Error Write Accessed to File. \n\n";
		return 0;
	}


	mix::generic::t_sort(_CniPck.begin(), _CniPck.end(), 0.25, mix::generic::NLess<char>());
	mix::generic::t_sort(_CniPck.begin(), _CniPck.end(), 0.25, mix::generic::NLess<int>());
	

	for (const _Canonical& _cni : _CniPck)
	{
		_vnodes.push_back(_cni._bitLen);
	}

	
	for (const node& _vn : _vnodes)
	{
		_Pqn.emplace(_vn);
	}

	vectorClean(_vnodes);

	filter_pq_nodes(_vnodes, _Pqn);

	
	for (const node& _vni : _vnodes)
	{
		_fPqn.emplace(_vni);
	}

	vectorClean(_vnodes);

	for (node _nf = 0; !_fPqn.empty(); )
	{
		_nf = _fPqn.top();
		_fPqn.pop();
		_vnodes.push_back(_nf);
		_nf = {};
	}
	
	
	if (!std::fwrite(&_CniSize, sizeof(_CniSize), 1, _FCni))
	{
		std::cerr << "\n Error write table records' size. \n\n";
		if (_FCni) std::fclose(_FCni);
		return 0;
	}


	mix::generic::t_sort(_vnodes.begin(), _vnodes.end(), 0.25,mix::generic::NLess<int>());

	
	// save encoded character
	for (size_t t = 0; t < _CniSize; t++)
	{
		std::fputc(_CniPck[t]._xData, _FCni);
		++pckSize;
	}

	const size_t _vnSize = _vnodes.size();

	if (!fwrite(&_vnSize, sizeof(_vnSize), 1, _FCni))
	{
		std::cerr << "\n Error write table records' size \n\n";
		if (_FCni) std::fclose(_FCni);
		return 0;
	}

	// save encoded bit length
	for (size_t z =0; z < _vnSize; z++)
	{
		std::fputc(_vnodes[z].Value(), _FCni);
		++pckSize;
	}
	
	// save the frequency of each encoded bit length (need for RLE)
	for (size_t k = 0; k < _vnSize; k++)
	{
		std::fputc(_vnodes[k].FrequencyData(), _FCni);
		++pckSize;
	}

	if (_FCni) std::fclose(_FCni);

	vectorClean(_vnodes);
	vectorClean(_CniPck);

	return pckSize;
}



// Read the encoded information data table from a file.
static inline const std::size_t readPackInfo(const std::string& _inFile, std::vector<_Canonical>& _Canoe)
{
	int _xc = 0;
	size_t _TSize = 0, _readSize = 0;
	std::FILE* _FInfo = std::fopen(_inFile.data(), "rb");
	_Canonical _ciTemp = {};

	if (!_FInfo)
	{
		std::cerr << "\n\n Error read-accessed to file. \n\n";
		return 0;
	}
	
	if (!std::fread(&_TSize, sizeof(size_t), 1, _FInfo))
	{
		std::cerr << "\n Error read table records' size ! \n\n";
		if (_FInfo) std::fclose(_FInfo);
		return 0;
	}

	const size_t _tbs = _TSize;

		for (size_t sz = 0; sz < _tbs; sz++)
		{
			if ((_xc = std::fgetc(_FInfo)) > 0)
			{
				_ciTemp._xData = _xc;
				_Canoe.push_back(_ciTemp);
				++_readSize;
				_ciTemp = {};
			}
		}

		_TSize = 0;

		// number of bit length records
		if (!std::fread(&_TSize, sizeof(size_t), 1, _FInfo))
		{
			std::cerr << "\n Error read table records' size ! \n\n";
			if (_FInfo) std::fclose(_FInfo);
			return 0;
		}

		const size_t _Recs = _TSize;

		for (size_t t = 0; t < _Recs; t++)
		{
			if ((_xc = std::fgetc(_FInfo)) > 0) {
				_Canoe[t]._bitLen = _xc;
				++_readSize;
			}
		}

		// number of frequency records
		for (size_t r = 0; r < _Recs; r++)
		{
			if ((_xc = std::fgetc(_FInfo)) > 0) {
				_Canoe[r]._rle_bit_len = _xc;
				++_readSize;
			}
		}

		if (_FInfo) std::fclose(_FInfo);

	return _readSize;
}




// Packed the raw data source and saves it to a file.
static inline const std::size_t writePack(const std::string& _File, const std::vector<_Canonical>& _PackNfo)
{
	int bit_len = 0, xBit = 0;
	size_t _writtenSize = 0;
	std::vector<int> _IntsPac;
	const std::size_t _NfoSize = _PackNfo.size();
	
	for (size_t _i = 0; _i < _NfoSize; _i++)
		_IntsPac.push_back(_PackNfo[_i]._codeWord);
	
		
	const std::string _sBitPck = cni_bits_pack(_IntsPac);
	const int xBitPck = _Int_from_Bit_Str(_sBitPck.data());
	_writtenSize = save_cni_bit(_File.data(), xBitPck);
	
	
	return _writtenSize;
}



// Read the packed data source to a int Vector.
static inline const std::size_t readPack(const std::string& _inFile, std::vector<int>& _inVec)
{
	std::size_t _readBytes = 0;
	
	_readBytes = read_cni_bit(_inFile.data(), _inVec);

	
return _readBytes;
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
	
	
	PRINT("\n Rematching integer with data ..");
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
		std::cerr << "\n Error read source file.. !! " << "\n\n";
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

	std::vector<char> _srcData = {};
	std::vector<int> _pacInts = {};

	std::vector<node> nodes = {};
	std::vector<BPAIR> _CodeMap = {};

	_Canonical _Canic = {};
	std::vector<_Canonical> _CanSrc = {}, _CanDat = {};
	
	std::priority_queue<node, std::vector<node>, std::less<node>> _pq;
	std::priority_queue<node, std::vector<node>, fqLess> _fpq;

	std::FILE* _FO = std::fopen(_srcF.data(), "rb");
	std::size_t F_SIZE = 0;


	if (!_FO)
	{
		std::cerr << "\n Error Read Source File. " << "\n\n";
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
	
	vectorClean(_srcData);

	filter_pq_nodes(nodes, _pq);
	
	for (auto const& _eV : nodes)
		_fpq.push(_eV);


	vectorClean(nodes);


	for (node _nF = 0; !_fpq.empty(); )
	{
		_nF = _fpq.top();
		nodes.push_back(_nF);
		_fpq.pop();
	}

	_TREE::plot_tree(nodes, compRate);

	_CodeMap = _TREE::CodeMap(); // encoding information generated

	
	for (const BPAIR& _bic : _CodeMap)
	{
		if (_bic._data > 0) {
			_srcData.push_back(_bic._data);
			_pacInts.push_back(_bic.bit_len);
		}
	}


	// set up the data for generating the canonical encoding information ..
	for (const char& _ci : _srcData)
	{
		if (_ci > 0)
		{
			_Canic._xData = _ci;
			_CanSrc.push_back(_Canic);
			_Canic = {};
		}
	}

	for (size_t _zi = 0; _zi < _pacInts.size(); _zi++)
	{
		if (_pacInts[_zi] > 0)
		{
			_CanSrc[_zi]._bitLen = _pacInts[_zi];
		}
	}

	
	_Gen_Canonical_Info(_CanDat, _CanSrc);

	
	if (!writePackInfo(_SystemFile.data(), _CanDat))
	{
		RET;
		std::cerr << "\n Error writing encoded's information data table to a file. \n\n";
		goto finishedDone;
	}


	if ( !(_bDone = writePack(_destF, _CanDat)) )
	{
		RET;
		std::cerr << "\n Error writing packed data source !  \n\n";
		goto finishedDone;
	}


finishedDone:
	if (!_srcData.empty()) _srcData.clear();

	vectorClean(_srcData);
	vectorClean(_pacInts);
	vectorClean(nodes);
	vectorClean(_CodeMap);
	vectorClean(_CanSrc);
	vectorClean(_CanDat);
	NULLP(_copyOne);
	if (!_SystemFile.empty()) _SystemFile.clear();
	_TREE::Clean();
	F_SIZE = 0;
	return _bDone;
}



static inline const std::size_t UnCompress(const std::string& _packedFile, const std::string& _unPackedFile)
{
	int _bi = 0;
	std::vector<char> _ReadChar;
	std::vector<int> _xBitLen;
	std::vector<_Canonical> _Canie = {}, _Candy = {};
	
	std::size_t _Cnt = 0;
	std::size_t _UnSquezzed = 0;

	std::string _xniBit;

	// to obtain a *.tbi file
	_SystemFile = (char*)lstr(_packedFile.data(), _packedFile.size() - 3);
	_SystemFile = (char*)concat_str((char*)_SystemFile.data(), "tbi");


	if ( !readPackInfo(_SystemFile.data(), _Canie) )
	{
		std::cerr << "\n Error read encoding information ! " << "\n\n";
		return 0;
	}

	for (const _Canonical& _Ce : _Canie)
	{
		_xBitLen.push_back(_Ce._bitLen);
	}
	

	const size_t _CnSize = _xBitLen.size();
	size_t _Ci = 0;


	// RLE method arranges the bit length information ..
	for (size_t _Zi = 0,  _Ci = 0; _Ci < _CnSize; _Ci++ )
	{
		_bi = _xBitLen[_Ci];
		_Cnt = _Canie[_Ci]._rle_bit_len;

		for (size_t _ni = 0; _ni < _Cnt; _ni++)
		{
			if (_Zi < _CnSize) _Canie[_Zi]._bitLen = _bi;
			++_Zi;
		}
	}
	
	
	_Gen_Canonical_Info(_Candy, _Canie);

	vectorClean(_xBitLen);

	
	if (!readPack(_packedFile.data(), _xBitLen))
	{
		std::cerr << "Error Read *.sqz File. \n\n";
		return 0;
	}

	
	_bi = 0;

	merge_cni_bit(_xBitLen, _bi);
		
	RET;

	PRINT(_bi);

	RET;


	if (!_SystemFile.empty()) _SystemFile.clear();
	if (!_xniBit.empty()) _xniBit.clear();


	vectorClean(_ReadChar);
	vectorClean(_Canie);
	vectorClean(_Candy);
	vectorClean(_xBitLen);

	return _UnSquezzed;
}


