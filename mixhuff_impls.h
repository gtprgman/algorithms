#pragma once


#ifndef MX_HUFF_IMPLS
	#define MX_HUFF_IMPLS

#endif



constexpr double COMP_RATE = 0.49; /* Amazing.. !!, further tweaking the calculation used to 
				   produce the best match has converged to this ideal value. */



// ReSync the read bits versus the original packed one
inline static void ReSync(std::vector<packed_info>& _readVec, const std::vector<packed_info>& _Packed)
{
	const std::size_t _maxSz = _Packed.size();
	int _hi = 0, _lo = 0, _Single = 0;
	_Int_Bits _read_bits;
	const int gMax = (int)_maxSz;

	for (int g = 0; g < gMax; g++)
	{
		if ( _Packed[g]._PACKED != _readVec[g]._PACKED)
		{	
			MAX_BIT = proper_bits(_Packed[g]._PACKED );
			_read_bits = _readVec[g].packed_bits;
				
			if (MAX_BIT > BYTE && MAX_BIT <= WORD)
			{
				_hi = _read_bits._eax[1];
				_lo = _read_bits._eax[0];
				_Single = _hi | _lo;
				_readVec[g]._PACKED = _Single;
			}
			else if (MAX_BIT > WORD && MAX_BIT <= DWORD)
			{
				_hi = _read_bits._eax[3] | _read_bits._eax[2];
				_lo = _read_bits._eax[1] | _read_bits._eax[0];
				_Single = _hi | _lo;
				_readVec[g]._PACKED = _Single;
			}
			_read_bits = { 0,0,0,0 };	
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
	int _Dir = 0, _recurr = 0, _sameVal = 0;
	node _e = 0; BPAIR _bpr = {};
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
			_sameVal = biXs.value_from_bitstr(_bt.data());
			++_sameVal;
			_bt.clear();
			_bt.assign(to_binary<int>::eval(_sameVal).data());
			_recurr = 0; _sameVal = 0;
		}
		
		_bt = (char*)concat_str((char*)_bt.data(), inttostr(_Dir));

		_bpr = biXs.value_from_bitstr(_bt.data());
		_sameVal = _bpr;

		if (mix::generic::
				vector_search(_vPair.begin(), _vPair.end(), _bpr, bitLess(), _iGet))
		//if (std::binary_search(_vPair.begin(), _vPair.end(),_bpr))
		{
			++_sameVal; _bt.clear();
			_bt.assign(to_binary<int>::eval(_sameVal).data());
			_sameVal = 0; _bpr = {};
		}

		_vPair.push_back({ _e.dataValue(),biXs.value_from_bitstr(LRTrim(_bt.data())) });
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
			

		_Dir = !_Dir;
		_bts.clear();

		_bts.assign( concat_str((char*)inttostr(_Dir),to_binary<int>::eval(_msk).data() ) );
	}
}




inline static const std::size_t writePack(const std::string& _fiName, const std::vector<packed_info>& _pacData)
{
	std::size_t _blocksWritten = 0;
	std::FILE* _fp = std::fopen(_fiName.data(), "wb+");
	packed_info _PiF = {};
	_32Bit _Datum;
	_Int_Bits _saved_bits;
	int n_PiF_blocks = (int)_pacData.size();


	if (!_fp)
	{
		std::cerr << "Failed to Open File !! " << "\\n\n";
		goto finishedDone;
	}

	// save information about the number of packed_info blocks at the beginning of file.
	std::fputc(n_PiF_blocks, _fp);
	
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
		else if (MAX_BIT > WORD && MAX_BIT <= DWORD)
		{
			_PiF._PACKED = 0;
			_PiF.packed_bits._eax[3] = HiPart(_Datum.HiWord() );
			_PiF.packed_bits._eax[2] = LoPart(_Datum.HiWord() );

			_PiF.packed_bits._eax[1] = HiPart(_Datum.LoWord() );
			_PiF.packed_bits._eax[0] = LoPart(_Datum.LoWord() );
		}

		std::fwrite(&_PiF, sizeof(_PiF), 1, _fp);
		++_blocksWritten;

		_Datum = 0;
		MAX_BIT = 0;
	}

	// saving real encoded bits of the raw input data source ..
	for (packed_info const& _pf : _pacData)
	{
		if (_pf._PACKED > 0)
		{
			std::fputc(_pf._PACKED, _fp);
			++_blocksWritten;
		}
		else
		{
			_saved_bits = _pf.packed_bits;
			std::fwrite(&_saved_bits, sizeof(_saved_bits), 1, _fp);
			_blocksWritten += sizeof(_saved_bits);
		}
	}

	finishedDone:
		std::fclose(_fp);

	return _blocksWritten;
}




inline static const std::size_t readPack(const std::string& _inFile,
					std::vector<packed_info>& _ReadVector, 
					std::vector<int>& _readBits)
{
	std::size_t _blocksRead = 0;
	std::FILE* _fi = std::fopen(_inFile.data(), "rb+");
	packed_info _PIF = {};
	int pif_blocks = 0;
	
	if (!_fi)
	{
		std::cerr << "Failed to Open File !!" << "\n\n";
		goto finishedRead;
	}

	// get the first integer value of file
	 pif_blocks = std::fgetc(_fi);

	// reads up number of packed_info blocks to the buffer _PIF.
	for(int j = 0; j < pif_blocks; j++)
	{
		std::fread(&_PIF, sizeof(packed_info), 1, _fi);
		++_blocksRead;
		_ReadVector.push_back(_PIF);
		_PIF = {};
	}

	// reads up number of encoded integers to the end of file.
	while ((pif_blocks = std::fgetc(_fi)) > 0)
		_readBits.push_back(pif_blocks);


	finishedRead:
		std::fclose(_fi);
		return _blocksRead;
}


