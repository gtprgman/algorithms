#pragma once


#ifndef MX_HUFF_IMPLS
	#define MX_HUFF_IMPLS
	#include <fstream>
#endif


// ReSync the read bits versus the original packed one
inline static void ReSync(std::vector<BPAIR>& _readVec, const std::vector<int>& _Packed)
{
	const std::size_t _maxSz = _Packed.size();
	std::size_t j = 0;
	int _hi = 0, _mid = 0, _lo = 0, _Single = 0;


	for (std::size_t g = 0; g < _maxSz; g++)
	{
		j = g;
		while (_readVec[j]._val < 0) ++j;

		if ( _Packed[g] != _readVec[g]._val )
		{	
			j = g;
			MAX_BIT = proper_bits(_Packed[j] );
			_hi = _readVec[j]._val; 
			_lo = _readVec[j + 1]._val;

				if (MAX_BIT == WORD) {
					_hi <<= 8; // Shifts _hi to the WORD's MSB
					_Single = _hi | _lo;
					_readVec[j]._val = _Single;
					_readVec[j + 1]._val = -1;
				}

				else if (MAX_BIT == DWORD) {
					_hi <<= 16;
					_mid = _lo << 8;
					_lo = _readVec[j + 2]._val;

					_Single = (_hi | _mid | _lo);
					_readVec[j]._val = _Single;
					_readVec[j + 1]._val = -1;
					_readVec[j + 2]._val = -1;
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
	node _e = 0;
	double _fq = 100.00;

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
			_bt.assign(to_binary<UINT>::eval(_sameVal).data());
			_recurr = 0;
		}

		_bt = (char*)concat_str((char*)_bt.data(), inttostr(_Dir));

		_vPair.push_back({ _e.dataValue(),biXs.value_from_bitstr(LRTrim(_bt.data())) });
	}
}



inline void _TREE::schema_Iter(const std::vector<node>& _fpNods)
{
	std::string _bts = "0";
	const int _TreeSizes = (int)_fpNods.size();
	const int _halfTreeSz = (int)halfSz(_TreeSizes) + 1;

	if (!_vPair.empty()) _vPair.clear();
	create_encoding(0, _halfTreeSz, _bts, _fpNods);

	_bts = "10";
	create_encoding(_halfTreeSz, _TreeSizes, _bts, _fpNods);

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

