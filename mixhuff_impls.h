#pragma once


#ifndef MX_HUFF_IMPLS
	#define MX_HUFF_IMPLS

#endif



inline static void filter_pq_nodes(std::vector<node>& _target, NODE_T&& _NodT,
				   const std::size_t _maxLen)
{
	NODE_T _nt = _NodT; // fetch new node every time the function is called
	double _fqr = 0.00;
	static int _q = 0;
	int _p = _q;

	if ((_target.empty()) && (_nt._v != 0) )
	{
		++_nt._freq;
		_target.push_back(_nt);
		return;
	}

	if (_target[_p] == _nt)
	{
		_fqr = _target[_p].FrequencyData();
		++_fqr;
		_target[_p].setFrequencyData(_fqr);
	}
	else
	{
		++_nt._freq;
		_target.push_back(_nt);
		_q++; // increases the index in target vector
	}
}


inline void _TREE::extract_schema(const std::vector<node>& _fpNods)
{
	BPAIR _bpt = {0,0};
	double _frq = 100.00;
	int _Dir = 0, _recurr = 0;
	int _sameVal = 0;
	std::size_t _halfInit = 0;

	std::string _bts = "0";

	const std::size_t _TreeSizes = _fpNods.size();
	const std::size_t _halfTreeSz = halfSz(_TreeSizes) + 1;

	
	// Processing on the left branches...
	for (std::size_t i = 0; i < _halfTreeSz; i++)
	{
		NODE_T _e = (NODE_T)_fpNods.at(i);
		
		if (_frq > _e._freq)
			_Dir = L;

		else if (_frq < _e._freq)
			_Dir = R;

		else if (_frq == _e._freq)
		{
			_recurr++;
			_Dir = R;

			if (_recurr > 1)
			{
				_sameVal = biXs.value_from_bitstr(_bts.data());
				++_sameVal; _bts.clear();
				_bts.assign(biXs.toBits(_sameVal));
				_bts = LRTrim(_bts.data());
				RPRINT(_bts.data()); RPRINT("->"); RPRINT((char)_e._v);
				RET;
				_frq = _e._freq;
				continue;
			}
		}

		_frq = _e._freq;

		_bts = (char*)concat_str((char*)_bts.data(), inttostr(_Dir));
		RPRINT(_bts.data()); RPRINT("->"); RPRINT((char)_e._v);
		RET;
		_halfInit = i;
	}

	//Processing on the right branches...
	_Dir = R;
	_bts.clear();
	_bts = "1";
	_recurr = 0;

	for (std::size_t j = _halfInit + 2; j < _TreeSizes; j++)
	{
		NODE_T _nod = (NODE_T)_fpNods.at(j);
		
		if (_recurr > 1)
		{
			_sameVal = biXs.value_from_bitstr(_bts.data());
			_sameVal += 7;
			_bts.clear();

			_bts.assign(biXs.toBits(_sameVal).data());
			RPRINT(_bts.data()); RPRINT("->"); RPRINT((char)_nod._v);
			RET;
			continue;
		}

		_bts = (char*)concat_str((char*)_bts.data(), inttostr(_Dir));

		RPRINT(_bts.data()); RPRINT("->"); RPRINT((char)_nod._v);
		RET;

		++_recurr;
	}
}

