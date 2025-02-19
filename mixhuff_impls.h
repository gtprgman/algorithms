#pragma once
/* Using License GPL v 3.0. */

#ifndef MX_HUFF_IMPLS
	#define MX_HUFF_IMPLS
	namespace FS = std::filesystem;

#endif


static std::uintmax_t F_SIZE = 0;
static std::string _SystemFile = "\0";

constexpr double COMP_RATE = 0.52; /* 0.52 is the default value, the users are allowed to tweak it 
				      in the command line */



// Using the information in _srcPackInfo to decompose each bit in _destPack.
static inline void UnPack_Bits(std::vector<int>& _destPack, const std::vector<packed_info>& _srcPackInfo)
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
	std::vector<int>::iterator _Itr;

	for (std::size_t j = 0, k = 0; j < packed_size; j++)
	{
		MAX_BIT = proper_bits(_srcPack[j]);

		if (_srcPack[j] != _destPack[j])
		{
			if (MAX_BIT > BYTE && MAX_BIT <= WORD)
			{
				hi = _destPack[j] << 8;
				lo = _destPack[j + 1];

				_destPack[j] = hi | lo;
				_Itr = _destPack.begin() + j;
				_destPack.erase(_Itr);
				
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

				k = j + 3;
				for (std::size_t i = j; i < k; i++)
				{
					_Itr = _destPack.begin() + i;
					_destPack.erase(_Itr);
				}
			}	
		}
	}
}



// ReSync the read Vector bits with the packed integers ('_Int_Bits')
static inline void ReSync(std::vector<packed_info>& _readVec)
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



static inline void filter_pq_nodes(std::vector<node>& _target, node&& _Nod, const std::size_t _maxLen)
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
				   int _bt,
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
static inline const std::size_t writePackInfo(const std::string& _fiName,  const std::vector<packed_info>& _pacData)
{
	std::size_t _blocksWritten = 0;
	std::FILE* _fpT = std::fopen(_fiName.data(), "wb");

	packed_info _PiF = {};
	_32Bit _Datum;
	_Int_Bits _saved_bits;
	int n_PiF_blocks = (int)_pacData.size();


	if (!_fpT)
	{
		std::cerr << "Failed to Open File !! " << "\\n\n";
		goto finishedDone;
	}

	// save information about the number of 'packed_info' blocks at the beginning of file.
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
		std::fflush(_fpT);
		if (_fpT) std::fclose(_fpT);

	return _blocksWritten;
}



// Read the encoded information data table from a file.
static inline const std::size_t readPackInfo(const std::string& _inFile, std::vector<packed_info>& _ReadVector)
{
	std::size_t _blocksRead = 0;
	std::FILE* _fiT = std::fopen(_inFile.data(), "rb");

	packed_info _PIF = {};
	int pif_blocks = 0;
	

	if (!_fiT)
	{
		std::cerr << "Failed to Open File !!" << "\n\n";
		goto finishedRead;
	}

	// get the first integer value of file
	 pif_blocks = std::fgetc(_fiT);


	// reads up number of 'packed_info' blocks to the buffer _PIF.
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


// Packed the raw data source and saves it to a file.
static inline const std::size_t writePack(const std::string& _File, const std::vector<packed_info>& _PackNfo)
{
	std::size_t _packedSize = 0;
	const std::size_t _PckSz = _PackNfo.size();
	std::FILE* _FPack = std::fopen(_File.data(), "wb");
	int _cByte = 0, _Bits = 0;

	if (!_FPack)
	{
		std::cerr << "Error initiates input source file." << "\n\n";
		goto DonePacking;
	}

	
	for (std::size_t _g = 0; _g < _PckSz; _g++)
	{
		_cByte = _PackNfo[_g]._PACKED;
		
			MAX_BIT = proper_bits(_cByte);

			if (MAX_BIT <= BYTE)
			{
				if (_cByte > 0)
				{
					std::fputc(_cByte, _FPack);
					++_packedSize;
				}
			}
			else if (MAX_BIT > BYTE && MAX_BIT <= WORD) // 16 Bit
			{
				if (_cByte > 0)
				{
					std::fputc(BYTE_PTR_HI(_cByte) >> 8, _FPack);
					std::fputc(BYTE_PTR_LO(_cByte), _FPack);
					++_packedSize;
				}
			}
			else if (MAX_BIT > WORD) // 32 Bit
			{
				_Bits = WORD_PTR_HI(_cByte); // 16 bit high

				if (_cByte > 0)
				{
					std::fputc(BYTE_PTR_HI(_Bits) >> 8, _FPack);
					std::fputc(BYTE_PTR_LO(_Bits), _FPack);
					++_packedSize;
				}

				_Bits = WORD_PTR_LO(_cByte); // 16 bit low

				if (_cByte > 0)
				{
					std::fputc(BYTE_PTR_HI(_Bits) >> 8, _FPack);
					std::fputc(BYTE_PTR_LO(_Bits), _FPack);
					++_packedSize;
				}
			}
		}


		
DonePacking:
	if (_FPack) std::fclose(_FPack);
	return _packedSize;
}



// Read the packed data source to a int Vector.
static inline const std::size_t readPack(const std::string& _inFile, std::vector<int>& _inVec)
{
	std::size_t _readChunk = 0;
	std::FILE* _fi = std::fopen(_inFile.data(), "rb");
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
static inline const std::size_t writeOriginal(const std::string& _OriginFile, const std::vector<int>& _intSrc, const std::vector<BPAIR>& _codeInfo)
{
	
	std::size_t _wordsWritten = 0;
	const std::size_t _codeSize = _codeInfo.size();
	std::FILE* _fOrig = std::fopen(_OriginFile.data(), "wb");
	std::vector<BPAIR>::iterator _bIt;
	std::vector<BPAIR> _codTab = _codeInfo;

	if (!_fOrig)
	{
		std::cerr << "Error writing file.. !! " << "\n\n";
		goto EndWrite;
	}
	
	mix::generic::t_sort(_codTab.begin(), _codTab.end(), 0.25, bitLess());

	for (int const& _i : _intSrc)
	{
		if (mix::generic::vector_search(_codTab.begin(), _codTab.end(), _i, bitLess(), _bIt))
		{
			std::fputc((int)_bIt->_data, _fOrig);
			++_wordsWritten;
		}
	}

EndWrite:
	std::fflush(_fOrig);
	if (_fOrig) std::fclose(_fOrig);
	
	return _wordsWritten;
}



// read the saved original uncompressed data from a file
static inline const std::size_t readOriginal(const std::string& _OrigFile, void* _mBuffer,const std::size_t _bufLen)
{
	std::size_t _wordsRead = 0;
	std::FILE* _fOri = std::fopen(_OrigFile.data(), "rb");
	const std::size_t _BufLen = _bufLen;
	int _rC = 0;

	if (!_fOri)
	{
		std::cerr << "Error reading file.. !! " << "\n\n";
		goto EndRead;
	}

	for(std::size_t _zi = 0; _zi < _BufLen; _zi++)
	{
		_rC = std::fgetc(_fOri);
		((char*)_mBuffer)[_zi] = _rC;
		++_wordsRead;
	}

EndRead:
	if (_fOri) std::fclose(_fOri);
	return _wordsRead;
}



// Recreate a BPAIR Table from the read encoded information data table
static inline void Construct_BPAIR_Table(std::vector<BPAIR>& _BPAIR, const std::vector<packed_info>& _PackInfo)
{
	BPAIR _bp = {};
	int _v1 = 0, _v2 = 0, _Orig = 0;
	std::string _cMask = "\0";
	const std::size_t _Max = _PackInfo.size();

	for (std::size_t i = 0; i < _Max; i++)
	{
		_v2 = _PackInfo[i]._PACKED;
		_v1 = unPack(_v2, _PackInfo[i].L_BIT, _PackInfo[i].R_BIT);
		_Orig = _v1;
		_cMask = repl_char('1', _PackInfo[i].R_BIT);
		_v1 = bin_to_dec<int>::eval(_cMask.data());
		_v2 = _v1 & _v2;

		_Orig = biXs.value_from_bitstr(to_binary<int>::eval(_Orig));
		_v2 = biXs.value_from_bitstr(to_binary<int>::eval(_v2));

		_bp._data = _PackInfo[i].L_ALPHA;
		_bp._val = _Orig;

		_BPAIR.push_back(_bp);

		_bp = {};

		_bp._data = _PackInfo[i].R_ALPHA;
		_bp._val = _v2;

		_BPAIR.push_back(_bp);

		_bp = {};
		_cMask.clear();
	}
}




static inline const bool Compress(const std::string& _destF, const std::string& _srcF, const double& compRate, char*& _iBuffer)
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

	std::FILE* _FO = std::fopen(_srcF.data(), "rb");

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

	F_SIZE = FS::file_size(_srcF.data());
	_iBuffer = new char[F_SIZE];
	std::memset(_iBuffer, 0, F_SIZE);

	if (!_iBuffer) goto finishedDone;

	// tops-up the buffer
	for (std::uintmax_t _iu = 0; _iu < F_SIZE; _iu++)
	{
		if ((_cF = std::fgetc(_FO)) > 0)
			_iBuffer[_iu] = _cF;
	}


	if (!std::strlen(_iBuffer)) goto finishedDone;

	for (std::uintmax_t _ix = 0; _ix < F_SIZE; _ix++)
	{
		_pq.push(_iBuffer[_ix]);
	}

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

	
	_TREE::plot_tree(nodes, compRate);

	_CodeMap = _TREE::CodeMap();

	
	/* Working on the bare raw data source ..
	   '_CodeMap '  is the previous generated encoding information data table. */
	mix::generic::t_sort(_CodeMap.begin(), _CodeMap.end(),0.25,chrLess());

	// Find the encoding match to pair to each data in '_iBuffer' and save the encoded data to a file.
	for (std::size_t _g = 0; _g < F_SIZE; _g++)
	{
		if (mix::generic::vector_search(_CodeMap.begin(), _CodeMap.end(), (char)_iBuffer[_g], chrLess(), _BiT))
		{
			_bIF.X = _BiT->_val;
			_bIF._Alpha = _BiT->_data;
			_bIF.numBits = oneAdder(num_of_bits<int>::eval(_bIF.X));
			_vbI.push_back(_bIF);
			_bIF = {};
		}
	}

	bitsPack(vpck0, _vbI); // packed the raw data source

	if ( !writePackInfo(_SystemFile.data(), vpck0) )
	{
		std::cerr << "Error writing encoded's information of the input file. " << "\n\n";
		goto finishedDone;
	}

	if (_destF.empty())
	{
		std::cerr << "The File's name for the compressed one could not be empty. " << "\n\n";
		goto finishedDone;
	}

	if ( !writePack(_destF, vpck0) )
	{
		std::cerr << "Error writing packed data source !" << "\n\n";
		goto finishedDone;
	}

	_bDone = true;

finishedDone:
	if (_FO) std::fclose(_FO);

	if (_iBuffer) delete[] _iBuffer;
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
	std::vector<int> _ReadIntBckup, _ReadInt;
	std::vector<packed_info> _ReadPck;
	std::vector<BPAIR> _ReadCodeMap;
	char* _OriginCopy = nullptr;
	int _Cnt = 0;

	// to obtain a *.tbi file
	_OriginCopy = (char*)lstr(_packedFile.data(), _packedFile.size() - 3);
	_OriginCopy = (char*)concat_str(_OriginCopy, "tbi");

	_SystemFile.assign(_OriginCopy);
	_OriginCopy = (char*)"\0";


	if ( readPackInfo(_SystemFile.data(), _ReadPck) < 0) // *.tbi
	{
		std::cerr << "Error reading encoded's information data table ! " << "\n\n";
		goto EndStage;
	}

	
	ReSync(_ReadPck);

	for (const packed_info& _pik : _ReadPck)
	{
		_ReadIntBckup.push_back(_pik._PACKED);
	}


	// read a *.sqz file
	if (readPack(_packedFile.data(), _ReadInt) < 0) // *.sqz
	{
		std::cerr << "Error reading compressed file. " << "\n\n";
		goto EndStage;
	}

	
	ReSync_Int(_ReadInt, _ReadIntBckup);

	UnPack_Bits(_ReadInt, _ReadPck);

	Construct_BPAIR_Table(_ReadCodeMap, _ReadPck); 

	if (_unPackedFile.empty())
	{
		_OriginCopy = (char*)lstr(_packedFile.data(), _packedFile.size() - 3);
		_OriginCopy = (char*)concat_str(_OriginCopy, "dat");
	}
	else _OriginCopy = (char*)_unPackedFile.data();

	
	if (writeOriginal(_OriginCopy, _ReadInt, _ReadCodeMap) < 0)
	{
		std::cerr << "Error writing decoded format of a file. " << "\n\n";
		goto EndStage;
	}
	

EndStage:
	NULLP(_OriginCopy);
	_ReadInt.clear();
	_ReadIntBckup.clear();
	_ReadCodeMap.clear();
	_ReadPck.clear();
}



