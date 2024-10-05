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


inline void _TREE::extract_schema()
{
	node* _pt = (node* const)_TREE::_Root;
	char* _bt = (char*)"0";
	BPAIR _bpt = {0,0};

	// processing left branches..
	while (_pt != nullptr)
	{
		if (ASSERT_P(_pt->links[L])) _pt = _pt->links[L]; else break;

		if (_pt->Value() > 0 && !_pt->Visited())
		{
			_bt = (char*)concat_str(_bt, inttostr(L));
			RPRINT(_bt); RPRINT("->"); RPRINT(_pt->dataValue()); RET;
			_pt->setVisit(true);
		}
		iterSiblings(_pt, ENCODE_SCHEMA, R, _bt, _bpt, _TREE::_mPair);
	}

	_pt = (node* const)_TREE::_Root;
	_bt = (char*)"1";
	int _Dir = 0;

	// processing on the right branches..
	while (_pt != nullptr)
	{
		if (ASSERT_P(_pt->links[_Dir])) _pt = _pt->links[_Dir]; else break;

		if ( (_pt->Value() > 0) && (!_pt->Visited()) )
		{
			_bt = (char*)concat_str(_bt, inttostr(_Dir));
			RPRINT(_bt); RPRINT("->"); RPRINT(_pt->dataValue()); RET;
			_pt->setVisit(true);
		}

		_Dir = !_Dir; //changes direction
	}
	_pt = nullptr;
}


inline void _TREE::extract_code() 
{
	node* _pt = (node* const)_TREE::_Root;
	char* _bt = (char*)"0";
	BPAIR _bpt = {0,0};

		// processing left branches..
		while (_pt != nullptr)
		{
			if (ASSERT_P(_pt->links[L])) _pt = _pt->links[L]; else break;

			if ( (_pt->Value() > 0 ) && (!_pt->Visited()) )
			{
				_bt = (char*)concat_str(_bt, inttostr(L));
				_bpt._data = _pt->dataValue();
				_bpt._val = biXs.value_from_bitstr(_bt);
				_mPair.emplace(std::pair<int, char>{_bpt._val, _bpt._data});
				_bpt = { 0,0 };
				_pt->setVisit(true);
			}
			iterSiblings(_pt, ENCODE_TREE, R, _bt, _bpt, _mPair);
		}
	
	// processing right branches..
	_pt = (node* const)_TREE::_Root;
	_bt = (char*)"1";
	_bpt = {};
	int _Dir = R;

	while (_pt != nullptr)
	{
		if (ASSERT_P(_pt->links[_Dir])) _pt = _pt->links[_Dir]; else break;
	
		if ( (_pt->Value() > 0) && (!_pt->Visited()) )
		{
			_bt = (char*)concat_str(_bt, inttostr(_Dir)); 
			_bpt._data = _pt->dataValue();
			_bpt._val = biXs.value_from_bitstr(_bt);
			_mPair.emplace(std::pair<int, char>{_bpt._val, _bpt._data});
			_bpt = {0,0};
			_pt->setVisit(true);
		}
		
		_Dir = !_Dir; //changes direction
	}
	_pt = nullptr;
}



inline void _TREE::iterSiblings(const node* const _pRoot, const int _enc, const int _Dirs,char*& _bt, BPAIR& _bpt,
	std::map<int, char>& _mPair)
{
	node* _pt = (node*)_pRoot;
	std::string _bts = _bt, _btc = "\0";

	while (_pt != nullptr)
	{
		if (ASSERT_P(_pt) && _pt->Value() > 0 && !_pt->Visited())
		{
			if (_enc == ENCODE_SCHEMA)
			{
				_bt = (char*)concat_str(_bt, inttostr(R));

				if (!_btc.empty()) {
					enforce_unique(_btc, _bt);
					_bt = _btc.data();
				}

				_pt->setVisit(true);
				RPRINT(_bt); RPRINT("->"); RPRINT(_pt->dataValue()); RET;
				_btc.assign(_bt);
			}
			else
			{
				_bt = (char*)concat_str(_bt, inttostr(R));

				if (!_btc.empty()) {
					enforce_unique(_btc, _bt);
					_bt = _btc.data();
				}

				_bpt._data = _pt->dataValue();
				_bpt._val = biXs.value_from_bitstr(_bt);
				_mPair.emplace(std::pair<int, char>{_bpt._val, _bpt._data});
				_btc.assign(_bt);

				_bpt = {0,0};
				_pt->setVisit(true);
			}
		}
		_pt = _pt->links[_Dirs];
	}

	_pt = nullptr;
}


inline void _TREE::enforce_unique(std::string& _s0, char*& _bt)
{
	int _bitV = 0;
	const std::size_t _lenMax = _s0.size();

	if (!std::strncmp(_s0.data(), _bt,_lenMax))
	{
		_bitV = bin_to_dec<int>::eval(_bt);
		++_bitV;
		_s0 = to_binary<int>::eval(_bitV);
	}
}

