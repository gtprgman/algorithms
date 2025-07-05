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
static inline const std::size_t ReSync_Int(std::vector<int>& _vecSqz, const int& _SqzInt, const std::vector<Can_Bit>& _ReadPac)
{
	

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
static inline const std::size_t writePackInfo(const std::string& _fiName, const std::vector<int>& _CodWords)
{
	int _b = 0;
	size_t _writtenSize = 0;
	const size_t _CodSize = _CodWords.size();

	std::FILE* _FPck = std::fopen(_fiName.data(), "wb");
	std::vector<int>& _vCods = (std::vector<int>&)_CodWords;
	std::vector<node> _vNods;

	if (!_FPck)
	{
		std::cerr << "\n Error Write-Accessed to File.";
		return 0;
	}

	for (size_t w = 0; w < _CodSize; w++)
	{
		_b = len_bit(_vCods[w]);
		_vCods[w] = _b;
	}

	_b = 0;

	// set up the frequency data for each bit length ..
	for (size_t i = 0,w = 0; w < _CodSize; w++)
	{
		_b = _vCods[w];

		if (_vNods.empty())
		{
			_vNods.push_back(_b);
			_vNods[0].setFrequencyData(1);
			++i;
			continue;
		}

		if (_b == _vNods[i - 1].Value())
		{
			_b = _vNods[i - 1].FrequencyData(); ++_b;
			_vNods[i - 1].setFrequencyData(_b);
			continue;
		}

		_vNods.push_back(_b);
		i = _vNods.size();
		_vNods[i-1].setFrequencyData(1);
	}

	
	// the required information is gathered

	const size_t _InfoSize = _vNods.size();

	if (!std::fwrite(&_InfoSize, sizeof(_InfoSize), 1, _FPck))
	{
		std::cerr << "\n Error writing header data.";
		if (_FPck) std::fclose(_FPck);
		return 0;
	}
	
	for (size_t f = 0; f < _InfoSize; f++)
	{
		// writing RLE info ..
		_b = _vNods[f].FrequencyData();
		std::fputc(_b, _FPck);
		++_writtenSize;
	}
 

	if (_FPck) std::fclose(_FPck);
	vectorClean(_vNods);

	return _writtenSize;
}



// Read the encoded information data table from a file.
static inline const std::size_t readPackInfo(const std::string& _inFile, std::vector<int>& _CodInfo)
{
	size_t _readSize = 0, infSize = 0;
	std::FILE* _Fr = std::fopen(_inFile.data(), "rb");

	if (!_Fr)
	{
		std::cerr << "\n Error read accessed to file. ";
		return 0;
	}

	int _x = 0;
	vectorClean(_CodInfo);

	if (!std::fread(&infSize, sizeof(size_t), 1, _Fr))
	{
		std::cerr << "\n Error read header size information.";
		if (_Fr) std::fclose(_Fr);
		return 0;
	}

	const size_t header_size = infSize;

	for (size_t f = 0; f < header_size; f++)
	{
		_x = std::fgetc(_Fr);
		_CodInfo.push_back(_x);
		++_readSize;
	}

	if (_Fr) std::fclose(_Fr);
	return _readSize;
}




// Packed the raw data source and saves it to a file.
static inline const std::size_t writePack(const std::string& _File, const int& _vPacked)
{
	size_t _writtenSize = 0;
	
	_writtenSize = save_cni_bit(_File.data(), _vPacked);


	return _writtenSize;
}



// Read the packed data source to a int Vector.
static inline const std::size_t readPack(const std::string& _inFile, std::vector<int>& vInts)
{
	int bx = 0;
	std::size_t _readBytes = 0;
	std::FILE* _Fp = std::fopen(_inFile.data(), "rb");

	if (!_Fp)
	{
		std::cerr << "\n Error Read-Accessed to File. ";
		return 0;
	}

	while ((bx = std::fgetc(_Fp)) > 0)
	{
		vInts.push_back(bx);
		++_readBytes;
	}

	if (_Fp) std::fclose(_Fp);

return _readBytes;
}



// write the original uncompressed form of the data into a file.
static inline const std::size_t writeOriginal(const std::string& _OriginFile, const std::vector<int>& _intSrc, const std::vector<Can_Bit>& _codeInfo)
{
	std::size_t _wordsWritten = 0;
	const std::size_t _codeSize = _codeInfo.size();
	std::FILE* _fOrig = std::fopen(_OriginFile.data(), "wb");
	std::vector<Can_Bit>::iterator _bIt;
	std::vector<Can_Bit>& _codTab = (std::vector<Can_Bit>&)_codeInfo;

	if (!_fOrig)
	{
		std::cerr << "\n Error writing file.. !! " << "\n\n";
		goto EndWrite;
	}
	
	
	PRINT("\n Rematching integer with data ..");
	for (const int& _i : _intSrc)
	{
		if ( _i > -1 )
		{
			if (mix::generic::vector_search(_codTab.begin(), _codTab.end(), _i, mix::generic::NLess<int>(), _bIt))
			{
				std::fputc((int)_bIt->_xData, _fOrig);
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
	std::string _xniBit;

	std::vector<char> _srcData = {};
	std::vector<int> _pacInts = {};

	std::vector<node> nodes = {};
	std::vector<BPAIR> _CodeMap = {};

	_Canonical _Canic = {};
	Can_Bit BitCan = {};

	std::vector<_Canonical> _CanSrc = {}, _CanDat = {};
	std::vector<Can_Bit> _CanBit = {};

	std::vector<Can_Bit>::iterator _CBiT;

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

	if (!(F_SIZE = readOriginal(_FO, _srcData, _cBuff)) )
	{
		RET;
		std::cerr << "\n Error read source file." << "\n\n";
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
			_Canic._xData = _bic._data;
			_Canic._bitLen = _bic.bit_len;
			_CanSrc.push_back(_Canic);
			_Canic = {};
		}
	}

	_Gen_Canonical_Info(_CanDat, _CanSrc); 

	for (const _Canonical& _Cnd : _CanDat)
	{
		BitCan._xData = _Cnd._xData;
		BitCan._codeWord = _Cnd._codeWord;
		_CanBit.push_back(BitCan);
		BitCan = {};
	}

	
	mix::generic::t_sort(_CanBit.begin(), _CanBit.end(), 0.25, mix::generic::NLess<char>());

	
	// gathering source data..
	for (size_t fz = 0; fz < F_SIZE; fz++)
	{
		char _ci = _srcData[fz];

		if (_ci > 0)
		{
			if (mix::generic::vector_search(_CanBit.begin(), _CanBit.end(), _ci, mix::generic::NLess<char>(), _CBiT))
			{
				_pacInts.push_back(_CBiT->_codeWord);
			}
		}
	}

	
	// writing encoding information table ..
	if (!writePackInfo(_SystemFile.data(), _pacInts))
	{
		RET;
		std::cerr << "\n Error writing encoding information to a file. \n\n";
		goto finishedDone;
	}


	_xniBit = cni_bits_pack(_pacInts);
	_cF = _Int_from_Bit_Str(_xniBit.data());
	
	// writing packed data source into a file ( *.sqz ).
	if ( !(_bDone = writePack(_destF, _cF)) )
	{
		RET;
		std::cerr << "\n Error writing compressed file !  \n\n";
		goto finishedDone;
	}


finishedDone:
	if (!_xniBit.empty()) _xniBit.clear();

	vectorClean(_srcData);
	vectorClean(_pacInts);
	vectorClean(nodes);
	vectorClean(_CodeMap);
	vectorClean(_CanSrc);
	vectorClean(_CanDat);
	vectorClean(_CanBit);
	
	NULLP(_copyOne);
	if (!_SystemFile.empty()) _SystemFile.clear();
	_TREE::Clean();
	F_SIZE = 0;
	return _bDone;
}



static inline const std::size_t UnCompress(const std::string& _packedFile, const std::string& _unPackedFile)
{
	int _bi = 0;

	std::vector<int> _xBitLen;
	
	std::size_t _Cnt = 0;
	std::size_t _UnSquezzed = 0;

	// to obtain a *.tbi file
	_SystemFile = (char*)lstr(_packedFile.data(), _packedFile.size() - 3);
	_SystemFile = (char*)concat_str((char*)_SystemFile.data(), "tbi");


	if ( !readPackInfo(_SystemFile.data(), _xBitLen) )
	{
		std::cerr << "\n Error read encoding information ! " << "\n\n";
		return 0;
	}

	for (const int& _i : _xBitLen) RPRINTC(_i);


	RET;

	
	

	
	vectorClean(_xBitLen);

	return _UnSquezzed;
}


