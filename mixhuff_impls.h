#pragma once


#ifndef MX_HUFF_IMPLS
	#define MX_HUFF_IMPLS
	#include <fstream>

#endif

constexpr double COMP_RATE = 0.36; /* 0.36 is the ideal one without subject to double-duplicate bits.
				   and we get a reasonable shorter-frequency least of bits. */

// ReSync the read bits versus the original packed one
inline static void ReSync(std::vector<BPAIR>& _readVec, const std::vector<int>& _Packed)
{
	const std::size_t _maxSz = _Packed.size();
	int _hi = 0, _mid = 0, _lo = 0, j = 0, k = 0, _Single = 0;
	const int gMax = (int)_maxSz;

	for (int g = 0; g < gMax; g++)
	{
		j = (!k)? g : k;

		if ( _Packed[g] != _readVec[j]._val )
		{	
			MAX_BIT = proper_bits(_Packed[g] );

			_hi = _readVec[j]._val;
			_lo = _readVec[j + 1]._val;

				if (MAX_BIT == WORD) {
					_Single = ((_hi << 8) | _lo );
					_readVec[j]._val = _Single;
					_readVec[j + 1]._val = -1;
					k = j + 2;
				}

				else if (MAX_BIT == DWORD) {
					_mid = ( (_readVec[j + 1]._val) << 8 );
					_lo = _readVec[j + 2]._val;
					_Single = (_hi | _mid | _lo);

					_readVec[j]._val = _Single;
					_readVec[j + 1]._val = -1;
					_readVec[j + 2]._val = -1;
					k = j + 3;
				}
		}
		continue;
	}
}



inline static void filter_pq_nodes(std::vector<node>& _target, node&& _Nod,
				  const std::size_t _maxLen)
{
	node _nod = _Nod; /*  fetches new node from the priority queue each time
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
	node _e = 0; BPAIR _bpr = { 0,0 };
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

		if (!_vPair.empty())
		{
			if (generic::vector_search(_vPair.begin(), _vPair.end(), _bpr, bitLess(), _iGet))
			{
				++_sameVal; _bt.clear();
				_bt.assign(to_binary<int>::eval(_sameVal).data());
				_sameVal = 0; _bpr = {};
			}
		}
		
		_vPair.push_back({ _e.dataValue(),biXs.value_from_bitstr(LRTrim(_bt.data())) });
		generic::fast_sort(_vPair.begin(), _vPair.end(), bitLess(), _vPair.size());
	}
}




inline void _TREE::schema_Iter(const std::vector<node>& _fpNods)
{
	const int _TreeSizes = (int)_fpNods.size();
	const double _CompRate = ((COMP_RATE) * _TreeSizes);
	int _DivSize = (_TreeSizes / (int)_CompRate);
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




inline static const std::size_t writePack(const std::string& _fiName, const std::vector<int>& _pacData)
{
	std::size_t _numWritten = 0;
	const std::size_t _packSz = _pacData.size();
	std::ofstream _out{ _fiName.data(), std::ios::out | std::ios::ate | std::ios::binary };
	std::vector<int> _DB;

	if (!_out) {
		std::cerr << "Failed to open file !!" << "\n\n";
		_out.close();
		return _numWritten;
	}

	for (const int _i : _pacData)
	{
		MAX_BIT = proper_bits(_i);

		if (MAX_BIT > BYTE)
		{
			parseByte(_i, _DB);

			for (const int _e : _DB)
				_out.put(_e);

			++_numWritten;
			_DB.clear();
			continue;
		}
		
		_out.put(_i);
		++_numWritten;
	}

	_out.close();
	_DB.clear();

	return _numWritten;
}




inline static const std::size_t readPack(const std::string& _inFile,
					std::vector<BPAIR>& _ReadVector)
{
	std::size_t _totReads = 0;
	std::ifstream _inf{ _inFile.data(),std::ios::in | std::ios::binary };
	int _C = 0;
	BPAIR _Rec = { 0,0 };

	if (!_inf)
	{
		std::cerr << "Can't open file to read !!" << "\n\n";
		_inf.close();
		return _totReads;
	}

	while( _C >= 0 )
	{
		_C = _inf.get();
		_Rec._val = _C;
		_ReadVector.push_back(_Rec);
		_totReads++; 
		_Rec = {0,0};
	}

	_inf.close();

	return _totReads;
}

