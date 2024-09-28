#pragma once



#ifndef MX_HUFF_IMPLS
	#define MX_HUFF_IMPLS

#endif

// proceed on the subnode right branches
inline static void iterRight(const node* const,const int, char*&, BPAIR&, std::map<int,char>&);


// to make sure that the traversed node is bit-differentiable
inline static void enforce_unique_bit_R(std::string& _s0, std::string& _s1, char*&);


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



inline void _TREE::traverse_encode(std::map<int, char>& _mPair, const node* const _fRoot,
								  const int _encNum)
{
	node* _pt = (node*)_fRoot;
	char* _bt = (char*)"0";
	BPAIR _bpt = {};

	// extracting left branches info..
	while (_pt != nullptr)
	{
		if (ASSERT_P(_pt->links[L])) _pt = _pt->links[L];
		else break;

		if (_pt->Value() > 0 && !_pt->Visited())
		{
			if (_encNum < 0)
			{
				_bt = (char*)concat_str(_bt, inttostr(L));
				RPRINT(_bt); RPRINT("->"); RPRINT(_pt->dataValue()); RET;
				_pt->setVisit(true);
			}
			else
			{
				_bt = (char*)concat_str(_bt, inttostr(L));
				_bpt._data = _pt->dataValue();
				_bpt._val = biXs.value_from_bitstr(_bt);
				_mPair.emplace(std::pair<int, char>{_bpt._val, _bpt._data});
				_bpt = {};
				_pt->setVisit(true);
			}
		}
		iterRight(_pt->links[R], _encNum,_bt, _bpt, _mPair);
		
	}
	// Encoding left branches succeeded !!

	// encoding bits pattern on the right branches..
	_pt = (node*)_fRoot;
	_bt = (char*)"1";
	_bpt = {};
	int _Dir = 0;


	while (_pt != nullptr)
	{
		if (ASSERT_P(_pt->links[_Dir])) _pt = _pt->links[_Dir];
		else break;

		if (_pt->Value() > 0 && !_pt->Visited())
		{
			if (_encNum < 0)
			{
				_bt = (char*)concat_str(_bt, inttostr(_Dir));
				RPRINT(_bt); RPRINT("->"); RPRINT(_pt->dataValue()); RET;
				_pt->setVisit(true);
			}
			else
			{
				_bt = (char*)concat_str(_bt, inttostr(_Dir));
				_bpt._data = _pt->dataValue();
				_bpt._val = biXs.value_from_bitstr(_bt);
				_mPair.emplace(std::pair<int, char>{_bpt._val, _bpt._data});
				_bpt = {};
				_pt->setVisit(true);
			}
		}

		if (ASSERT_P(_pt->links[!_Dir]) && _pt->links[!_Dir]->Value() > 0 &&
			!_pt->links[!_Dir]->Visited())
		{

			if (_encNum < 0)
			{
				_Dir = !_Dir;
				_pt = _pt->links[_Dir];
				_bt = (char*)concat_str(_bt, inttostr(_Dir));
				RPRINT(_bt); RPRINT("->"); RPRINT(_pt->dataValue()); RET;
				_bt = (char*)rtrim(_bt);
				_pt->setVisit(true);
			}
			else
			{
				_Dir = !_Dir; // changes direction
				_pt = _pt->links[_Dir]; // point to next node
				_bt = (char*)concat_str(_bt, inttostr(_Dir));
				_bpt._data = _pt->dataValue();
				_bpt._val = biXs.value_from_bitstr(_bt);
				_mPair.emplace(std::pair<int, char>{_bpt._val, _bpt._data});
				_bpt = {};
				_bt = (char*)rtrim(_bt);
				_pt->setVisit(true);
			}
		}

		_Dir = !_Dir; //changes direction
	}

	// Encoding Right Branches Succeeded !!
	_pt = nullptr;
}



inline static void iterRight(const node* const _pRoot, const int _enc, char*& _bt, BPAIR& _bpt,
							std::map<int,char>& _mPair)
{
	node* _pt = (node*)_pRoot;
	std::string _bts = _bt, _btc;

	while (_pt != nullptr)
	{
		if (ASSERT_P(_pt) && _pt->Value() > 0 && !_pt->Visited())
		{
			if (_enc < 0)
			{
				_bt = (char*)concat_str(_bt, inttostr(R));

				if (_pt->links[R] == nullptr) {
					std::strncpy(_btc.data(), _bt, _bts.size());
					enforce_unique_bit_R(_bts, _btc, _bt);
				}
				
				_pt->setVisit(true);
				RPRINT(_bt); RPRINT("->"); RPRINT(_pt->dataValue()); RET;
				_bt = (char*)rtrim(_bt);
			}
			else
			{
				_bt = (char*)concat_str(_bt, inttostr(R));
				
				if (_pt->links[R] == nullptr) {
					std::strncpy(_btc.data(), _bt, _bts.size());
					enforce_unique_bit_R(_bts, _btc, _bt);
				}

				_bpt._data = _pt->dataValue();
				_bpt._val = biXs.value_from_bitstr(_bt);
				_mPair.emplace(std::pair<int, char>{_bpt._val, _bpt._data});
				_bpt = {};
				_bt = (char*)rtrim(_bt);
				_pt->setVisit(true);
			}
		}
		_pt = _pt->links[R];
	}
	_pt = nullptr;
}


inline static void enforce_unique_bit_R(std::string& _s0, std::string& _s1, char*& _bt)
{
	const std::size_t _oldMax = _s0.size();

	if (!strncmp(_s0.data(), _s1.data(), _oldMax))
	{
		_bt = (char*)rtrim(_bt);
		_bt = (char*)concat_str(_bt, "1");
	}
}

