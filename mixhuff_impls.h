#pragma once


#ifndef MX_HUFF_IMPLS
	#define MX_HUFF_IMPLS
    
#endif

static std::string _SystemFile = "\0";

constexpr double COMP_RATE = 0.52; /* Amazing.. !!, further tweaking the calculation used to 
				      produce the best match has converged to this ideal value. */



// Using the information in _srcPackInfo to decompose each bit in _destPack.
static inline void UnPack_Bits(std::vector<int>& _destPack,
			       const std::vector<packed_info>& _srcPackInfo)
{
	std::size_t j = 0;
	std::string _str_mask;
	int elem1 = 0, elem2 = 0, unpack_value = 0;
	
	_destPack.clear();

	for (packed_info const& _Pfi : _srcPackInfo)
	{
		elem2 = _srcPackInfo[j++]._PACKED; // packed value
		elem1 = unPack(elem2, _Pfi.L_BIT, _Pfi.R_BIT);
		
		unpack_value = elem1;

		_str_mask = repl_char('1', _Pfi.R_BIT);

		elem1 = bin_to_dec<int>::eval(_str_mask.data());
		
		elem2 = elem1 & elem2;
		
		unpack_value = biXs.value_from_bitstr(to_binary<int>::eval(unpack_value) );
		elem2 = biXs.value_from_bitstr(to_binary<int>::eval(elem2) );

			_destPack.push_back(unpack_value);
			_destPack.push_back(elem2);

		_str_mask.clear();
	}
}



// ReSync the read packed data source versus the read packed table of information
static inline void ReSync_Int(std::vector<int>& _destPack, const std::vector<int>& _srcPack)
{
	int hi = 0, lo = 0;
	_Int_Bits _iReg = { 0,0,0,0 };
	const std::size_t packed_size = _srcPack.size();

	for (std::size_t j = 0; j < packed_size; j++)
	{
		MAX_BIT = proper_bits(_srcPack[j]);

		if (_srcPack[j] != _destPack[j])
		{
			if (_destPack[j] < 0)
			{
				_destPack[j] = _srcPack[j];
				continue;
			}
			
			
			if (MAX_BIT > BYTE && MAX_BIT <= WORD)
			{
				hi = _destPack[j] << 8;
				lo = _destPack[j + 1];

				_destPack[j] = hi | lo;
				_destPack[j + 1] = -1;
			}
			else if (MAX_BIT > WORD)
			{
				_iReg._eax[3] = _destPack[j];
				_iReg._eax[2] = _destPack[j + 1];
				_iReg._eax[1] = _destPack[j + 2];
				_iReg._eax[0] = _destPack[j + 3];

				hi = (_iReg._eax[3] | _iReg._eax[2]) << 16;
				lo = _iReg._eax[1] | _iReg._eax[0];

				_destPack[j] = hi | lo;

				for (std::size_t i = j + 1; i < (j + 4); i++)
					_destPack[i] = -1;
				
			}	
		}
	}
}



// ReSync the read Vector bits with the packed integers ('_Int_Bits')
inline static void ReSync(std::vector<packed_info>& _readVec)
{
	const std::size_t _maxSz = _readVec.size();
	int _hi = 0, _lo = 0, _Single = 0;
	_Int_Bits _read_bits;
	const std::size_t gMax = _maxSz;

	for (std::size_t g = 0; g < gMax; g++)
	{
		if (!_readVec[g]._PACKED)
		{
			_hi = _readVec[g].packed_bits._eax[3] |
				_readVec[g].packed_bits._eax[2];

			_lo = _readVec[g].packed_bits._eax[1] |
				_readVec[g].packed_bits._eax[0];

			_Single = _hi | _lo;
			_readVec[g]._PACKED = _Single;
		}
	}
}



inline static void filter_pq_nodes(std::vector<node>& _target, node&& _Nod,
				   const std::size_t _maxLen)
{
	node _nod = _Nod; /* fetches new node from the priority queue each time
						this function is called. */
	double _fqr = 0;
	static int _q = 0;
	int _p = _q;

	if ((_target.empty()) && (_nod.Value() != 0) )
	{
		_fqr = _nod.FrequencyData();
		++_fqr;
		_nod.setFrequencyData(_fqr);
		_target.push_back(_nod);
		return;
	}

	if (_target[_p].Value() == _nod.Value())
	{
		_fqr = _target[_p].FrequencyData();
		++_fqr;
		_target[_p].setFrequencyData(_fqr);
	}
	else
	{
		_fqr = _nod.FrequencyData();
		++_fqr;
		_nod.setFrequencyData(_fqr);
		_target.push_back(_nod);
		_q++; // increases the index in target vector
	}
}



inline void _TREE::create_encoding(const int _From, 
				   const int _To,
				   std::string& _bt,
				   const std::vector<node>& _Vn)
{
	node _e = 0;
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
			_bt.clear();
			_bt.assign(to_binary<int>::eval(_sameVal).data());
			_recurr = 0;
			goto filterPhase;
		}
		
		_bt = (char*)concat_str((char*)_bt.data(), inttostr(_Dir));

	filterPhase:

		_sameVal = biXs.value_from_bitstr(LRTrim(_bt.data()));
		_prevX = _sameVal;

		if (mix::generic::
				vector_search(_vPair.begin(), _vPair.end(), _sameVal, bitLess(), _iGet))
		//if (std::binary_search(_vPair.begin(), _vPair.end(),_bpr))
		{
			_sameVal = _iGet->_val;
			++_sameVal;
			_bt.clear();
			_bt.assign(to_binary<int>::eval(_sameVal).data());
			_prevX = biXs.value_from_bitstr(LRTrim(_bt.data()));
		}

		_vPair.push_back({ _e.dataValue(), _prevX });
		mix::generic::fast_sort(_vPair.begin(), _vPair.end(), bitLess());
		//std::stable_sort(_vPair.begin(), _vPair.end());
	}
}




inline void _TREE::schema_Iter(const std::vector<node>& _fpNods)
{
	const int _TreeSizes = (int)_fpNods.size();
	const double _CompRate = std::floor((COMP_RATE) * _TreeSizes);
	const double _fCompRate = std::ceil((double)_TreeSizes / _CompRate);
	int _DivSize = (int)_fCompRate;
	int _msk = 0, _BT = 2, _Dir = L;
	std::string _bts = inttostr(_msk);
	

	if (!_vPair.empty()) _vPair.clear();

	for (int t = 0; t < _TreeSizes; t += _DivSize)
	{
		if ( (_TreeSizes - t) < _DivSize ) _DivSize = 1;
		create_encoding(t, (t + _DivSize), _bts, _fpNods);

		_msk ^= _BT--;

		if (_BT < 1) 
			_BT = 2; 
			

		_Dir = _Dir ^ 1;
		_bts.clear();

		_bts.assign( concat_str((char*)inttostr(_Dir),to_binary<int>::eval(_msk).data() ) );
	}

	_TREE::enforce_unique(_vPair);
}



inline void _TREE::enforce_unique(std::vector<BPAIR>& _bPairs)
{
	const std::size_t _nMax = _bPairs.size() - 1;

	for (std::size_t i = 0; i < _nMax; i++)
	{
		if (_bPairs[i]._val == _bPairs[i + 1]._val)
			++_bPairs[i + 1]._val;
	}
}



// Save the encoded table information into a file.
inline static const std::size_t writePackInfo(const std::string& _fiName, 
					      const std::vector<packed_info>& _pacData)
{
	std::size_t _blocksWritten = 0;
	std::FILE* _fpT = std::fopen(_fiName.data(), "wb+");

	packed_info _PiF = {};
	_32Bit _Datum;
	_Int_Bits _saved_bits;
	int n_PiF_blocks = (int)_pacData.size();


	if (!_fpT)
	{
		std::cerr << "Failed to Open File !! " << "\\n\n";
		goto finishedDone;
	}

	// save information about the number of packed_info blocks at the beginning of file.
	std::fputc(n_PiF_blocks, _fpT);
	
	// saving table of encoded information ..
	for (packed_info const& _pi : _pacData)
	{
		_PiF = _pi;
		_Datum = _PiF._PACKED;
		MAX_BIT = _Datum.BitLength();

		if ( MAX_BIT > BYTE && MAX_BIT <= WORD )
		{
			_PiF._PACKED = 0;
			_PiF.packed_bits._eax[1] = _Datum.HiByte();
			_PiF.packed_bits._eax[0] = _Datum.LoByte();
		}
		else if (MAX_BIT > WORD )
		{
			_PiF._PACKED = 0;
			_PiF.packed_bits._eax[3] = HiPart(_Datum.HiWord() );
			_PiF.packed_bits._eax[2] = LoPart(_Datum.HiWord() );

			_PiF.packed_bits._eax[1] = HiPart(_Datum.LoWord() );
			_PiF.packed_bits._eax[0] = LoPart(_Datum.LoWord() );
		}

		std::fwrite(&_PiF, sizeof(_PiF), 1, _fpT);
		++_blocksWritten;
		
		_Datum = 0;
		MAX_BIT = 0;
	}

	
	finishedDone:
		if (_fpT) std::fclose(_fpT);

	return _blocksWritten;
}



// Read the encoded table information from a file.
inline static const std::size_t readPackInfo(const std::string& _inFile,
					      std::vector<packed_info>& _ReadVector)
{
	std::size_t _blocksRead = 0;
	std::FILE* _fiT = std::fopen(_inFile.data(), "rb+");

	packed_info _PIF = {};
	int pif_blocks = 0;
	

	if (!_fiT)
	{
		std::cerr << "Failed to Open File !!" << "\n\n";
		goto finishedRead;
	}

	// get the first integer value of file
	 pif_blocks = std::fgetc(_fiT);


	// reads up number of packed_info blocks to the buffer _PIF.
	for(int j = 0; j < pif_blocks; j++)
	{
		std::fread(&_PIF, sizeof(packed_info), 1, _fiT);
		++_blocksRead;
		_ReadVector.push_back(_PIF);
		_PIF = {};
	}

	
	finishedRead:
		if (_fiT) std::fclose(_fiT);
		return _blocksRead;
}



// Save the packed raw data source to a file.
static inline const std::size_t writePack(const std::string& _File,
					  const std::vector<packed_info>& _packedSrc)
{
	std::size_t _chunkWritten = 0;
	std::FILE* _fp = std::fopen(_File.data(), "wb+");
	int _cBit = 0,_bitC = 0,_bitF = 0, _cnt = 0;

	if (!_fp)
	{
		std::cerr << "Failed to open file !" << "\n\n";
		goto finishedWrite;
	}

	for (packed_info const& _pck : _packedSrc)
	{
		_cnt = 0;
		_bitC = _pck._PACKED; // Get packed bits
		MAX_BIT = proper_bits(_bitC);

		if (MAX_BIT > BYTE && MAX_BIT <= WORD) // 16 BIT
		{
			while ((_cBit = extract_byte(_bitC)) > 0) // extract by 8 bit basis
			{
				_cBit = (_cnt)? _cBit : _cBit >> 8; // is it a high 8 bit?
				std::fputc(_cBit, _fp);
				++_cnt; ++_chunkWritten;
			}
			continue;
		}
		else if (MAX_BIT > WORD) // 32 BIT
		{
			while ((_cBit = extract_byte(_bitC)) > 0) // extract by 16 bit basis
			{
				_cBit = (_cnt)? _cBit : _cBit >> 16; // is it a 16 bit high?

				_bitF = _cBit;

				_cnt = 0; // reset option-setter counter 

				while ((_cBit = extract_byte(_bitF)) > 0) // extract by 8 bit basis
				{
					_cBit = (_cnt)? _cBit : _cBit >> 8; // is it an 8 bit high?
					std::fputc(_cBit, _fp);
					++_cnt; ++_chunkWritten;
				}
				_cnt = 1;
			}
			continue;
		}
		

		if (_bitC > 0)
		{
			std::fputc(_bitC, _fp);
			++_chunkWritten;
		}
	}

	finishedWrite:
		if (_fp) std::fclose(_fp);
		return _chunkWritten;
}



// Read the packed data source to a int Vector.
static inline const std::size_t readPack(const std::string& _inFile, std::vector<int>& _inVec)
{
	std::size_t _readChunk = 0;
	std::FILE* _fi = std::fopen(_inFile.data(), "rb+");
	int _C = 0;

	if (!_fi)
	{
		std::cerr << "File Read Error.. !! " << "\n\n";
		goto finishedRead;
	}

	while ((_C = std::fgetc(_fi)) > 0)
	{
		_inVec.push_back(_C);
		++_readChunk;
	}


	finishedRead:
		if (_fi) std::fclose(_fi);
		return _readChunk;
}



// write the original uncompressed form of the data into a file.
static inline const std::size_t writeOriginal(const std::string& _OriginFile, const std::vector<int>& _intSrc, 
					      const std::vector<BPAIR>& _codeInfo)
{
	
	std::size_t _wordsWritten = 0;
	const std::size_t _codeSize = _codeInfo.size();
	std::FILE* _fOrig = std::fopen(_OriginFile.data(), "wb+");
	std::vector<BPAIR>::iterator _bIt;
	std::vector<BPAIR> _codTab = _codeInfo;

	if (!_fOrig)
	{
		std::cerr << "Error writing file.. !! " << "\n\n";
		goto EndWrite;
	}
	
	mix::generic::fast_sort(_codTab.begin(), _codTab.end(), bitLess());

	for (auto const& _i : _intSrc)
	{
		if (mix::generic::vector_search(_codTab.begin(), _codTab.end(), _i, bitLess(), _bIt))
		{
			std::fputc(_bIt->_data, _fOrig);
			++_wordsWritten;
		}

	}

EndWrite:
	if (_fOrig) std::fclose(_fOrig);
	return _wordsWritten;
}



// read the saved original uncompressed data from a file
static inline const std::size_t readOriginal(const std::string& _OrigFile, std::vector<char>& _readDat)
{
	std::size_t _wordsRead = 0;
	std::FILE* _fOri = std::fopen(_OrigFile.data(), "rb+");
	int _rC = 0;

	if (!_fOri)
	{
		std::cerr << "Error reading file.. !! " << "\n\n";
		goto EndRead;
	}

	while ((_rC = std::fgetc(_fOri)) > 0)
	{
		_readDat.push_back(_rC);
		++_wordsRead;
	}

EndRead:
	if (_fOri) std::fclose(_fOri);
	return _wordsRead;
}



// Reconstruct a BPAIR Table from the read encoded information data table
static inline void Construct_BPAIR_Table(std::vector<BPAIR>& _BPAIR, const std::vector<packed_info>& _PackInfo)
{
	BPAIR _bp = {};

	for (packed_info const& _pfo : _PackInfo)
	{
		_bp._data = _pfo.L_ALPHA;
		_bp._val = _pfo.L_BIT;

		_BPAIR.push_back(_bp);
		_bp = {};

		_bp._data = _pfo.R_ALPHA;
		_bp._val = _pfo.R_BIT;

		_BPAIR.push_back(_bp);
		_bp = {};
	}
}



static inline const bool Compress(const std::string& _srcF,
				  const std::string& _destF,
				  std::vector<char>& _vecSrc)
{
	bool _bDone = 0;
	int _cF = 0, _CountStr = 0;
	char* _copyOne = nullptr;
	const char* _sExt = "\0";
	
	bitInfo<int> _bIF = {};

	std::vector<node> nodes;
	std::vector<BPAIR> _CodeMap;
	std::vector<BPAIR>::iterator _BiT;
	std::vector<bitInfo<int>> _vbI;
	std::vector<packed_info> vpck0;

	
	std::priority_queue<node, std::vector<node>, std::greater<node>> _pq;
	std::priority_queue<node, std::vector<node>, fqLess> _fpq;

	std::FILE* _FO = std::fopen(_srcF.data(), "rb+");

	if (!_FO)
	{
		std::cerr << "Error in opening source file!" << "\n\n";
		goto finishedDone;
	}
	
	_SystemFile.assign(_srcF);


	if ((_cF = strNPos(_SystemFile.data(), '.')) > -1) // if source file has extension
	{
		_SystemFile.assign(lstr(_srcF.data(), _cF));
		_SystemFile = (char*)concat_str(_SystemFile.data(), ".tbi");
	}
	else
		_SystemFile = (char*)concat_str(_SystemFile.data(), ".tbi");
	


	if (_destF.empty())
	{
		_copyOne = (char*)lstr(_srcF.data(), _cF);
		_copyOne = (char*)concat_str(_copyOne, ".sqz");
		goto CoreProcesses;
	}


	if ( (_cF = strNPos(_destF.data(), '.') > -1)  )// if the file has an extension
	{
		_sExt = rstr(_destF.data(), 3);
		_sExt = reverse_str(_sExt);

		if (std::strncmp(_sExt, "sqz", 3)) // if extension string not equal 'sqz'
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

	if (!_vecSrc.empty()) _vecSrc.clear();

	while ((_cF = std::fgetc(_FO)) > 0)
		_vecSrc.push_back(_cF);


	if (_vecSrc.empty()) goto finishedDone;

	for (auto const& _e : _vecSrc)
		_pq.push(_e);


	while (!_pq.empty())
	{
		filter_pq_nodes(nodes, (node&&)_pq.top(), 0);
		_pq.pop();
	}
	
	for (auto const& _eV : nodes)
		_fpq.push(_eV);


	nodes.clear();

	for (node _nF = 0; !_fpq.empty(); )
	{
		_nF = _fpq.top();
		nodes.push_back(_nF);
		_fpq.pop();
	}

	_TREE::plot_tree(nodes);

	_CodeMap = _TREE::CodeMap();

	for (auto const& _eP : _CodeMap)
	{
		_bIF.X = _eP._val; 
		_bIF._Alpha = _eP._data;

		_bIF.numBits = oneAdder(num_of_bits<int>::eval(_bIF.X));
		_vbI.push_back(_bIF);
		_bIF = {};
	}
	
	bitsPack(vpck0, _vbI);


	if (writePackInfo(_SystemFile.data(), vpck0) < 0)
	{
		std::cerr << "Error writing encoded's information table !" << "\n\n";
		goto finishedDone;
	}

	
	// Working on the bare raw data source ..
	mix::generic::fast_sort(_CodeMap.begin(), _CodeMap.end(), chrLess());

	_bIF = {};
	_vbI.clear();
	vpck0.clear();


	for (auto const& _ci : _vecSrc)
	{
		if (mix::generic::vector_search(_CodeMap.begin(), _CodeMap.end(), _ci, chrLess(), _BiT))
		{
			_bIF.X = _BiT->_val;
			_bIF._Alpha = _BiT->_data;
			_bIF.numBits = oneAdder(num_of_bits<int>::eval(_bIF.X));

			_vbI.push_back(_bIF);
		}
	}


	bitsPack(vpck0, _vbI);

	if (!_destF.empty()) _copyOne = (char*)_destF.data();

	if (writePack(_copyOne, vpck0) < 0)
	{
		std::cerr << "Error writing packed data source !" << "\n\n";
		goto finishedDone;
	}


finishedDone:
	if (_FO) std::fclose(_FO);

	_vecSrc.clear();
	 nodes.clear();
	_CodeMap.clear();
	_vbI.clear();
	vpck0.clear();
	NULLP(_copyOne);
	_SystemFile.clear();
	_TREE::Clean();

	return _bDone;
}



static inline void UnCompress(const std::string& _packedFile, const std::string& _unPackedFile)
{
	
}

