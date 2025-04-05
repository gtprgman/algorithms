#pragma once
/* Using License GPL v 3.0. */

#ifndef MX_HUFF_IMPLS
	#define MX_HUFF_IMPLS

#endif


extern "C" std::uintmax_t F_SIZE = 0;
static std::string _SystemFile = "\0";



static inline const int decompose(const int& _Packed, const int& _LeftNBits, const int& _RightNBits)
{
	int _v = 0;
	_v = unPack(_Packed, _LeftNBits, _RightNBits);

	return _v;
}


constexpr double COMP_RATE = 0.52; /* 0.52 is the default value, the users are allowed to tweak it 
				      in the command line */



/* Using the information in _srcPackInfo to decompose each bit in _destPack.
   NB: 'ReSync_Int_Of_Pack()' is assumed to be called first on _destPack before calling 'Unpack_Bits()' */
static inline const std::size_t UnPack_Bits(std::vector<int>& _destPack, const std::vector<packed_info>& _srcPackInfo)
{
	std::size_t UnPack_Sze = 0;
	const std::size_t _PckSize = _srcPackInfo.size(), _OriSize = 2 * _PckSize;
	int elem1 = 0, elem2 = 0, unpack_value = 0, _Msk = 0;
	

	_destPack.resize(_OriSize);
	
	for (size_t g = 0, j = 0; j < _PckSize; j++)
	{
		elem2 = _srcPackInfo[j]._PACKED;
		elem1 = unPack(elem2, _srcPackInfo[j].L_BIT, _srcPackInfo[j].R_BIT);
		unpack_value = elem1;

		_Msk = set_low_bit(_srcPackInfo[j].R_BIT);
		elem2 = _Msk & elem2;

		_destPack[g] = unpack_value;
		_destPack[g + 1] = elem2;

		g += 2;
		++UnPack_Sze;

		_Msk = 0;
	}

return UnPack_Sze;
}


// Resync the integers of packed_info read from *.tbi, it returns zero if all the packed integers is in byte ordered.
static inline const std::size_t ReSync_Int_Of_Pack(std::vector<packed_info>& _Pac)
{
	int _Hi = 0, _Lo = 0;
	std::size_t _Synced_Size = 0;
	const std::size_t _PacSz = _Pac.size();

	for (std::size_t z =0 ; z < _PacSz; z++)
	{
		if (!_Pac[z]._PACKED)
		{
			_Hi = (_Pac[z]._Reg._eax[0]) << 8 | _Pac[z]._Reg._eax[1];

			_Hi <<= 24; // Shift to 16 bit high

			_Lo = (_Pac[z]._Reg._eax[2]) << 8 | _Pac[z]._Reg._eax[3];

			_Pac[z]._PACKED = _Hi | _Lo;
			_Synced_Size += 1;
		}
	}
	return _Synced_Size;
}



// ReSync the read packed int (*.sqz) with the int read from information data table file (*.tbi)
static inline const std::size_t ReSync_Int(std::vector<int> _SqzInt, const std::vector<int>& _TbiInt)
{
	std::size_t _Synced_Szi = 0;
	const std::size_t _TbSz = _TbiInt.size();
	int _Byte = 0;
	
	for (std::size_t g = 0, j = 0; j < _TbSz; j++)
	{
		g = (g)? g : j;
		
		if (_TbiInt[j] != _SqzInt[g])
		{
			MAX_BIT = proper_bits(_TbiInt[j]);
			if (MAX_BIT > BYTE && MAX_BIT <= WORD) // 16 bit
			{
				_Byte = _SqzInt[g];
				_Byte <<= 8;
				_Byte |= _SqzInt[g + 1];
				_SqzInt[g] = _Byte;
				_SqzInt[g + 1] = 0;

				++_Synced_Szi;
				g += 2; // index to the next element
			}
			else if (MAX_BIT > WORD) // 32 bit
			{
				_Byte = (_SqzInt[g] << 24) | (_SqzInt[g + 1] << 16) | (_SqzInt[g + 2] << 8) |
					 _SqzInt[g + 3];

				_SqzInt[g] = _Byte;
				_SqzInt[g + 1] = 0;
				_SqzInt[g + 2] = 0;
				_SqzInt[g + 3] = 0;
				
				_Synced_Szi += 3;
				g += 4; // index to the next element
			}
		}
	}
	return _Synced_Szi;
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
static inline const std::size_t writePackInfo(const std::string& _fiName, const std::vector<BPAIR>& _pacData)
{
	std::size_t _blocksWritten = 0, _RecSz = 0;
	std::FILE* _fpT = std::fopen(_fiName.data(), "wb");

	int _Datum = 0, _DataBits = 0;
	BPAIR _bTemp = {};
	packed_info _PiF = {};
	_32Bit BIT_UNIT;
	
	const std::size_t n_BPAIR_blocks = _pacData.size(), // BPAIR Records' Size
					  n_PCKINF_blocks = n_BPAIR_blocks / 2;  // packed_info records' size


	if (!_fpT)
	{
		std::cerr << "Failed to Open File !! " << "\\n\n";
		goto PackageWritten;
	}

	// saving information about the number of 'BPAIR' blocks at the beginning of file.
	if (!(_RecSz = std::fwrite(&n_BPAIR_blocks, sizeof(n_BPAIR_blocks), 1, _fpT))) {
		std::cerr << "Error writing BPAIR records size number !!" << "\n\n";
		goto PackageWritten;
	}

	_RecSz = n_BPAIR_blocks;

	// saving table of encoded information ..
	for (BPAIR const& _pi : _pacData)
	{
		_Datum = _pi._data;
		
		if (_Datum > 0)
		{
			_bTemp = _pi;
			_bTemp._PacInfo = {};

			if (!std::fwrite(&_bTemp, sizeof(_bTemp), 1, _fpT))
				std::cerr << "Error writing Rec: #" << _RecSz << " of BPAIR." << "\n\n";
			else ++_blocksWritten;
		}
			
		_bTemp = {}; _Datum = 0; _RecSz -= 1;
	}

	// Saving 'packed_info' records' size next after 'BPAIR' data blocks in the file.
	if ( !(_RecSz = std::fwrite(&n_PCKINF_blocks, sizeof(n_PCKINF_blocks), 1, _fpT)) )
		std::cerr << "Error writing packed_info records size number !! " << "\n\n";

	_RecSz = n_PCKINF_blocks;

	// saving table of packed information..
	for (const BPAIR& _bpi : _pacData)
	{
		_PiF = _bpi._PacInfo;
		BIT_UNIT = _PiF._PACKED;
		MAX_BIT = BIT_UNIT.BitLength();

		if ((MAX_BIT > BYTE) && (MAX_BIT <= WORD))   // 16 bit
		{
			_Datum = _PiF._PACKED;
			_PiF._PACKED = 0;

			_PiF._Reg._eax[0] = 0;
			_PiF._Reg._eax[1] = 0;
			_PiF._Reg._eax[2] = BYTE_PTR_HI(_Datum) >> 8;
			_PiF._Reg._eax[3] = BYTE_PTR_LO(_Datum);

		}
		else if (MAX_BIT > WORD)  // 32 bit
		{
			_Datum = _PiF._PACKED;
			_PiF._PACKED = 0;
			_DataBits = WORD_PTR_HI(_Datum) >> 24;   // Moves 16 bit high to LSB

			_PiF._Reg._eax[0] = BYTE_PTR_HI(_DataBits) >> 8;
			_PiF._Reg._eax[1] = BYTE_PTR_LO(_DataBits);

			_DataBits = WORD_PTR_LO(_Datum); // 16 bit low

			_PiF._Reg._eax[2] = BYTE_PTR_HI(_DataBits) >> 8;
			_PiF._Reg._eax[3] = BYTE_PTR_LO(_DataBits);
		}
		
		if (BIT_UNIT.Value() > 0) // make sure the data to be saved is not null
		{
			if (!std::fwrite(&_PiF, sizeof(_PiF), 1, _fpT))
				std::cerr << "Error writing Rec: #" << _RecSz << " of packed_info table." << "\n\n";
			else ++_blocksWritten;
		}

		_PiF = {}; BIT_UNIT = 0; _Datum = 0;  _DataBits = 0; _RecSz -= 1;
	}

	
PackageWritten:
	if (_fpT)
	{
		std::fflush(_fpT);
		std::fclose(_fpT);
	}

	return _blocksWritten;
}



// Read the encoded information data table from a file.
static inline const std::size_t readPackInfo(const std::string& _inFile, std::vector<BPAIR>& _ReadVector)
{
	std::size_t _blocksRead = 0;
	std::FILE* _fiT = std::fopen(_inFile.data(), "rb");

	BPAIR _bpr = {};
	packed_info _pack = {};

	std::size_t bpair_blocks = 0, pckinfo_blocks = 0;
	
	// get the Number of Saved 'BPAIR' Records from a file
	if (!std::fread(&bpair_blocks, sizeof(size_t), 1, _fiT)) {
		std::cerr << "Error getting BPAIR records size number read from file !!" << "\n\n";
		if (_fiT) std::fclose(_fiT);
		return 0;
	}

	const std::size_t n_bpair_rec = bpair_blocks;

	// reads up number of 'BPAIR' blocks to the buffer _bpr.
	for(std::size_t j = 0; j < n_bpair_rec; j++)
	{
		if (std::fread(&_bpr, sizeof(BPAIR), 1, _fiT))
		{
			++_blocksRead;
			_ReadVector.push_back(_bpr);
		}
		else
			std::cerr << "Record #" << j << " of BPAIR is read error from file !!" << "\n\n";

		_bpr = {};
	}

	_bpr = {};

	if (!std::fread(&pckinfo_blocks, sizeof(size_t), 1, _fiT)) {
		std::cerr << "Error getting packed_info records size number read from file !!" << "\n\n";
		if (_fiT) std::fclose(_fiT);
		return 0;
	}

	const std::size_t n_pack_blocks = pckinfo_blocks;

	for (std::size_t k = 0; k < n_pack_blocks; k++)
	{
		if (std::fread(&_pack, sizeof(packed_info), 1, _fiT))
		{
			++_blocksRead;
			_ReadVector[k]._PacInfo = _pack;
		}
		else 
			std::cerr << "Error Read Rec: #" << pckinfo_blocks << " of packed_info table from the file." << "\n\n";
		

		_pack = {}; pckinfo_blocks -= 1;
	}

	
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
		std::cerr << "Error opened input source file." << "\n\n";
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
			else if ( (MAX_BIT > BYTE) && (MAX_BIT <= WORD) ) // 16 Bit
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
				if (_cByte > 0)
				{
					_Bits = WORD_PTR_HI(_cByte) >> 24;  // 16 bit high moved to LSB
					std::fputc(BYTE_PTR_HI(_Bits) >> 8, _FPack);
					std::fputc(BYTE_PTR_LO(_Bits), _FPack);
					++_packedSize;

					_Bits = WORD_PTR_LO(_cByte);  // 16 bit low
					std::fputc(BYTE_PTR_HI(_Bits) >> 8, _FPack);
					std::fputc(BYTE_PTR_LO(_Bits), _FPack);
					++_packedSize;
				}
			}
	}


DonePacking:
	if (_FPack) {
		std::fflush(_FPack);
		std::fclose(_FPack);
	}
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
static inline const std::size_t writeOriginal(const std::string& _OriginFile, const std::vector<int>& _intSrc, 
											  const std::vector<BPAIR>& _codeInfo)
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
		if ( (_i > 0) && mix::generic::vector_search(_codTab.begin(), _codTab.end(), _i, bitLess(), _bIt)  )
		{
			std::fputc((int)_bIt->_data, _fOrig);
			++_wordsWritten;
		}
		else if ( _i > 0 ) std::cerr << "Bit for integer: " << _i << " not Found!!" << "\n\n";
	}

EndWrite:
	
	if (_fOrig)
	{
		std::fflush(_fOrig);
		std::fclose(_fOrig);
	}

	_codTab.clear();

	return _wordsWritten;
}



// read the saved original uncompressed data from a file
static inline const std::size_t readOriginal(std::FILE*& _fHandle , std::vector<char>& _DataSrc)
{
	std::size_t _wordsRead = 0;
	const std::size_t dataSize = _DataSrc.capacity();
	int _rC = 0;

	if (!_fHandle)
	{
		std::cerr << "Error reading file.. !! " << "\n\n";
		goto EndRead;
	}

	for(std::size_t j = 0; j < dataSize; j++ )
	{
		if ((_rC = std::fgetc(_fHandle)) > 0)
		{
			_DataSrc[j] = _rC;
			++_wordsRead;
		}
	}

EndRead:
	return _wordsRead;
}




static inline const bool Compress(const std::string& _destF, const std::string& _srcF, const double& compRate, 
								  std::vector<char>& _iBuffer)
{
	bool _bDone = 0;
	int _cF = 0, _CountStr = 0;
	char* _copyOne = nullptr;
	const char* _sExt = "\0";
	bitInfo<int> _bIF = {};

	std::vector<node> nodes = {};
	std::vector<BPAIR> _CodeMap = {};
	std::vector<BPAIR>::iterator _BiT;
	std::vector<bitInfo<int>> _vbI = {};
	std::vector<packed_info> vpck0 = {};

	
	std::priority_queue<node, std::vector<node>, std::less<node>> _pq;
	std::priority_queue<node, std::vector<node>, fqLess> _fpq;

	std::FILE* _FO = std::fopen(_srcF.data(), "rb");


	if (!_FO)
	{
		std::cerr << "Error opened input source file. " << "\n\n";
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

	if (!(F_SIZE = readOriginal(_FO, _iBuffer)) )
	{
		std::cerr << "Error read source file." << "\n\n";
		goto finishedDone;
	}
	
	
	for (std::size_t _i = 0; _i < F_SIZE; _i++)
	{
		node _C = _iBuffer[_i];
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
		if (mix::generic::vector_search(_CodeMap.begin(), _CodeMap.end(), (char)_iBuffer[_g], chrLess(), _BiT))
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
		std::cerr << "The File's name for the compressed one could not be empty. " << "\n\n";
		goto finishedDone;
	}

	
	for (BPAIR const& _bpc : _CodeMap)
	{
		vpck0.push_back(_bpc._PacInfo);
	}


	if (!writePackInfo(_SystemFile.data(), _CodeMap))
	{
		std::cerr << "Error writing encoded's information data table to a file. " << "\n\n";
		goto finishedDone;
	}


	if ( !(_bDone = (writePack(_destF, vpck0)) > 0 ) )
	{
		std::cerr << "Error writing packed data source !" << "\n\n";
		goto finishedDone;
	}


finishedDone:
	if (_FO) {
		std::fflush(_FO);
		std::fclose(_FO);
	}

	if (!_iBuffer.empty()) _iBuffer.clear();
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
	std::vector<int> _ReadInt, _ReadIntBckup;
	std::vector<packed_info> _ReadPck;
	std::vector<BPAIR> _ReadCodeMap;

	int _Cnt = 0;
	std::size_t _UnSquezzed = 0;
	char* _OriginCopy = nullptr;

	// to obtain a *.tbi file
	_SystemFile = (char*)lstr(_packedFile.data(), _packedFile.size() - 3);
	_SystemFile = (char*)concat_str(_SystemFile.data(), "tbi");


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

	
	ReSync_Int_Of_Pack(_ReadPck);

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
	if (!_ReadIntBckup.empty()) _ReadIntBckup.clear();
	if (!_ReadCodeMap.empty()) _ReadCodeMap.clear();
	if (!_ReadPck.empty()) _ReadPck.clear();

	return _UnSquezzed;
}


