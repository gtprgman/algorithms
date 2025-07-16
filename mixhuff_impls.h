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
static inline const std::size_t ReSync_Int(std::vector<int>& _vecSqz, std::vector<char>& ReSynced_Data,
					   const std::vector<int>& Bits_Len,  const int64_t& IntSqz,  
						const std::vector<Can_Bit>& _CniInfo)
{
	int w = 0, bx = 0;
	size_t synced_size = 0;
	const std::string xsbit = bit_str(IntSqz);

	const size_t bitSize = xsbit.size();
	const size_t BitsInfoSize = Bits_Len.size();

	char* xs = (char*)xsbit.data();
	xs[bitSize] = 0;

	std::vector<Can_Bit>& CNF = (std::vector<Can_Bit>&)_CniInfo;

	std::vector<Can_Bit>::iterator CBT;

	vectorClean(_vecSqz);

	for (size_t i = 0; i < BitsInfoSize; i++)
	{
		w = Bits_Len[i];
		bx = (int)int_bit(lstr(xs, w));
		_vecSqz.push_back(bx);
		xs += w;
	}

	const size_t SqzSize = _vecSqz.size();

	for (size_t q = 0; q < SqzSize; q++)
	{
		if (mix::generic::vector_search(CNF.begin(), CNF.end(), _vecSqz[q],
						 mix::generic::NLess<int>(), CBT))
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
static inline const std::size_t writePackInfo(const std::string& _fiName, std::vector<_Canonical>& CniSrc, 
					      const std::vector<int>& _CodWords)
{
	int _b = 0;
	size_t _writtenSize = 0;
	const size_t _CodSize = _CodWords.size();
	int* wordsLen = new int[_CodSize];

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
		wordsLen[w] = _b;
	}

	_b = 0;

	// set up the frequency data for each bit length ..
	for (size_t i = 0,w = 0; w < _CodSize; w++)
	{
		_b = wordsLen[w];

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
	
	// writing codewords length..
	for (size_t d = 0; d < _InfoSize; d++)
	{
		std::fputc(_vNods[d].Value(), _FPck);
		++_writtenSize;
	}

	// writing RLE info for each codeword length ..
	for (size_t f = 0; f < _InfoSize; f++)
	{
		_b = _vNods[f].FrequencyData();
		std::fputc(_b, _FPck);
		++_writtenSize;
	}
 
	
	const size_t CndSize = CniSrc.size();

	// set up the RLE Info for each bit length of the source canonical..
	for (size_t h = 0, g = 0; g < CndSize; g++)
	{
		h = g;
		_b = CniSrc[h]._bitLen;

		for (size_t j = h; j < CndSize; j++)
		{
			if (_b != CniSrc[j]._bitLen) break;
			else{
				++CniSrc[h]._rle_bit_len;
			}
			g = j;
		}
	}


	if (!std::fwrite(&CndSize, sizeof(CndSize), 1, _FPck))
	{
		std::cerr << "\n Error writing header info data.";
		if (_FPck) std::fclose(_FPck);
		return 0;
	}

	// writing encoded data ..
	for (size_t d = 0; d < CndSize; d++)
	{
		std::fputc(CniSrc[d]._xData, _FPck);
		++_writtenSize;
	}

	
	// writing rle info for each source bit length ..
	for (size_t r = 0; r < CndSize; r++)
	{
		std::fputc(CniSrc[r]._rle_bit_len, _FPck);
		++_writtenSize;
	}

	// writing bit length info ..
	for (size_t cx = 0; cx < CndSize; cx++)
	{
		if (CniSrc[cx]._rle_bit_len > 0) {
			std::fputc(CniSrc[cx]._bitLen, _FPck);
			++_writtenSize;
		}
	}

	if (_FPck) std::fclose(_FPck);
	vectorClean(_vNods);

	delete[_CodSize] wordsLen;

	return _writtenSize;
}



// Read the encoded information data table from a file.
static inline const std::size_t readPackInfo(const std::string& _inFile, std::vector<_Canonical>& Canie ,std::vector<_Canonical>& CnSrc)
{
	Can_Bit cbi = {};
	size_t _readSize = 0, infSize = 0;
	std::FILE* _Fr = std::fopen(_inFile.data(), "rb");

	if (!_Fr)
	{
		std::cerr << "\n Error read accessed to file. ";
		return 0;
	}


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
		Canie.push_back(cbi);
		cbi = {};
		++_readSize;
	}

	// picking up RLE Info for each codeword length ..
	for (size_t r = 0; r < header_size; r++)
	{
		Canie[r]._rle_bit_len = std::fgetc(_Fr);
		++_readSize;
	}


	// Picking up canonical header size ..
	if (!std::fread(&infSize, sizeof(size_t), 1, _Fr))
	{
		std::cerr << "\n Error read header info data.";
		if (_Fr) std::fclose(_Fr);
		return 0;
	}

	const size_t cni_header_size = infSize;

	cbi = {};

	// encoded data ..
	for (size_t j = 0; j < cni_header_size; j++)
	{
		cbi._xData = std::fgetc(_Fr);
		CnSrc.push_back(cbi);
		cbi = {};
	}

	// rle of bit lengths ..
	for (size_t k = 0; k < cni_header_size; k++)
		CnSrc[k]._rle_bit_len = std::fgetc(_Fr);

	// bit length
	for (size_t i = 0; i < cni_header_size; i++)
	{
		if (CnSrc[i]._rle_bit_len > 0)
			CnSrc[i]._bitLen = std::fgetc(_Fr);
		else CnSrc[i]._bitLen = 0;
	}
	

	if (_Fr) std::fclose(_Fr);
	return _readSize;
}



// Packed the raw data source and saves it to a file.
static inline const std::size_t writePack(const std::string& _File, const int64_t& _vPacked)
{
	size_t _writtenSize = 0;
	
	_writtenSize = save_cni_bit(_File.data(), _vPacked);


	return _writtenSize;
}



// Read the packed data source to a int Vector.
static inline const std::size_t readPack(const std::string& _inFile, std::vector<int>& vInts)
{
	std::size_t _readBytes = 0;

	_readBytes = read_cni_bit(_inFile, vInts);

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


// generates huffman encoding information ..
static inline const int64_t Gen_Encoding_Info(std::vector<char>& _Src, std::vector<BPAIR>& CodInfo, 
						std::vector<_Canonical>& Cni_Dat, std::vector<int>& PacInts,
						int64_t& SqzInt, const double& cmp_rate)
{
	std::priority_queue<node, std::vector<node>, std::less<node>> _pq;
	std::priority_queue<node, std::vector<node>, fqLess> _fpq;
	std::vector<node> PNodes;

	_Canonical _Canoe = {};
	Can_Bit cbi = {};

	std::vector<_Canonical> Cni_Info;
	std::vector<Can_Bit> CniBits;
	std::vector<Can_Bit>::iterator CBiT;

	std::string xs_bit;

	const size_t T_SIZE = _Src.size();

	for (size_t t = 0; t < T_SIZE; t++)
	{
		PNodes.push_back(_Src[t]);
	}

	for (const node& e : PNodes)
	{
		_pq.emplace(e);
	}

	PNodes.clear();

	filter_pq_nodes(PNodes, _pq);

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

	_TREE::plot_tree(PNodes, cmp_rate);

	CodInfo = _TREE::CodeMap(); // huffman encoding info generated

	for (const BPAIR& bp : CodInfo)
	{
		_Canoe._xData = bp._data;
		_Canoe._bitLen = bp.bit_len;
		Cni_Dat.push_back(_Canoe);
		_Canoe = {};
	}

	_Gen_Canonical_Info(Cni_Info, Cni_Dat);

	cni_enforce_unique(Cni_Info); // codewords data updated

	for (const _Canonical& cni : Cni_Info)
	{
		cbi._xData = cni._xData;
		cbi._codeWord = cni._codeWord;
		CniBits.push_back(cbi);
		cbi = {};
	}

	mix::generic::t_sort(CniBits.begin(), CniBits.end(), 0.25, mix::generic::NLess<char>());

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

	xs_bit = cni_bits_pack(PacInts);
	SqzInt = int_bit(xs_bit.data());
	
	PRINT(xs_bit.data());
	PRINT(SqzInt);
	
	return SqzInt;
}

// extracting a saved encoding data into 'CnBit' and 'CnRaw' vectors
static inline const int extract_encoding_info(const std::string& xFile,std::vector<_Canonical>& CnBit, std::vector<_Canonical>& CnRaw)
{
	if (!readPackInfo(xFile.data(), CnBit, CnRaw))
	{
		std::cerr << "\n Error read encoding information ! " << "\n\n";
		return 0;
	}

	return -1;
}


static inline const bool Compress(const std::string& _destF, const std::string& _srcF, const double& compRate, char* _cBuff)
{
	bool _bDone = 0;
	int _cF = 0, _CountStr = 0;
	char* _copyOne = nullptr;
	const char* _sExt = "\0";

	std::vector<char> _srcData = {};
	std::vector<int> _pacInts = {};

	std::vector<BPAIR> _CodeMap = {};
	std::vector<_Canonical> _CanSrc = {}, vnb = {};
	
	std::FILE* _FO = std::fopen(_srcF.data(), "rb");
	std::size_t F_SIZE = 0;
	int64_t _sqzNum = 0;

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
	
	
	_sqzNum = Gen_Encoding_Info(_srcData, _CodeMap, _CanSrc, _pacInts, _sqzNum, COMP_RATE);

	
	// writing encoding information table ..
	if (!writePackInfo(_SystemFile.data(), _CanSrc, _pacInts))
	{
		RET;
		std::cerr << "\n Error writing encoding information to a file. \n\n";
		goto finishedDone;
	}

	
	// writing packed data source into a file ( *.sqz ).
	if ( !(_bDone = writePack(_destF, _sqzNum)) )
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

	NULLP(_copyOne);
	if (!_SystemFile.empty()) _SystemFile.clear();
	if (_FO) std::fclose(_FO);

	_TREE::Clean();
	F_SIZE = 0;
	return _bDone;
}



static inline const std::size_t UnCompress(const std::string& _packedFile, const std::string& _unPackedFile)
{
	int64_t cmb_bit = 0;
	int _bi = 0, _x = 0;
	Can_Bit cnbi = {};
	
	std::vector<char> RawDat = {};
	std::vector<_Canonical> _Canonic = {}, _Canine = {}, _CanDat = {};
	std::vector<Can_Bit> vCnbi = {};

	
	std::size_t ReckonSize = 0, _UnSquezzed = 0;
	std::vector<int> cniBitLen, _SqzInts;

	char* _OriginFile = (char*)_unPackedFile.data(), * _sExt = (char*)"\0";

	// to obtain a *.tbi file
	_SystemFile = (char*)lstr(_packedFile.data(), _packedFile.size() - 3);
	_SystemFile = (char*)concat_str((char*)_SystemFile.data(), "tbi");

	extract_encoding_info(_SystemFile, _Canonic, _Canine);

	if (!readPack(_packedFile, _SqzInts))
	{
		std::cerr << "\n Error read compressed file. ";
		return 0;
	}
	
	for (const int& q : _SqzInts) RPRINTC(q);

	RET;

	merge_cni_bit(_SqzInts, cmb_bit);

	PRINT(cmb_bit);

	RET;

	return 0;
	const size_t CnSize = _Canine.size();

	// RLE method brings back the bit length info in _Canine..
	for (size_t y = 0, z = 0; z < CnSize; z++)
	{
		y = z;
		_x = _Canine[z]._bitLen;
		_bi = _Canine[z]._rle_bit_len; // number of repeated bit length

		for (int r = 0; r < _bi && y < CnSize; r++)
			_Canine[y++]._bitLen = _x;
	}

	
	_Gen_Canonical_Info(_CanDat, _Canine);
	cni_enforce_unique(_CanDat);

	// filling up the most important data needed in 'ReSync_Int() ' calls..
	for (const _Canonical& cnd : _CanDat)
	{
		cnbi._xData = cnd._xData;
		cnbi._codeWord = cnd._codeWord;
		vCnbi.push_back(cnbi);
		cnbi = {};
	}

	PRINT("\n sorting integers of symbols.. ");
	mix::generic::t_sort(vCnbi.begin(), vCnbi.end(), 0.25, mix::generic::NLess<int>());

	/*
	for (const auto& cdi : vCnbi)
	{
		RPRINTC(cdi._xData); RPRINTC(cdi._codeWord); RET;
	}
	
	return 0;
	*/

	const size_t cnfSize = _Canonic.size();

	// RLE method unravels each bit length info of a packed integer..
	for (size_t g = 0,f = 0; f < cnfSize; f++)
	{
		_bi = _Canonic[f]._bitLen;
		_x = _Canonic[f]._rle_bit_len;

		for (int t = 0; t < _x; t++)
			cniBitLen.push_back(_bi);
	} 

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


