#pragma once



#ifndef MX_HUFF_IMPLS
	#define MX_HUFF_IMPLS
#endif


template <class N >
inline static void sort_Nodes(std::vector<node>& vn, const std::size_t _Len) {
	UINT j = 0, k = 0, r = 0, t = 0;
	const UINT	_Max = (UINT)_Len;
	N _v1, _v3, _v2, _v4;
	NODE_T _n1, _n3, _n2, _n4;

	if (vn.empty()) return;

	// apply quick sort on each half of the set
	for (UINT n = 0, e = (_Max / 2), u = _Max, d = e, l = e;
		(n < e) && (u > e); n++, l++, d--, u--) {

		_v1 = VALT<N>(vn[n]);
		_v3 = VALT<N>(vn[d]);

		if (_v1 < _v3) {
			_n1 = vn[n];
			vn[n] = vn[d];
			vn[d] = _n1;
		}

		_v2 = VALT<N>(vn[l]);
		_v4 = VALT<N>(vn[u]);

		if (_v2 < _v4) {
			_n2 = vn[l];
			vn[l] = vn[u];
			vn[u] = _n2;
		}
	}


	// apply n times insertion sort to each half of the set
	for (UINT g = 0; g < _Max; g++)
		for (UINT i = 0, m = (_Max / 2); (i < m) && (m < _Max); i++, m++) {
			t = i; r = t + 1;
			j = m; k = j + 1;

			_v1 = VALT<N>(vn[t]); // smaller
			_v3 = VALT<N>(vn[r]); // greater

			_v2 = VALT<N>(vn[j]); // smaller
			_v4 = VALT<N>(vn[k]); // greater


			while (t >= 0 && j >= m) {

				if (_v1 < _v3) {
					_n1 = vn[t]; // preserve the smaller
					vn[t] = vn[r];  // assign greater to the preceding position
					vn[r] = _n1;
				}

				if (_v2 < _v4) {
					_n2 = vn[j];
					vn[j] = vn[k];
					vn[k] = _n2;
				}

				--t; r = t + 1;
				--j; k = j + 1;
			}
		}
}



template <class N>
inline static void range_sort(std::vector<node>& _vn, const int L, const int R) {
	NODE_T tiny;
	int q = 0, p = 0, lim = 0;
	int mid = (L + R) / 2;

	while ((R - mid) < (mid - L)) --mid;

	for (int i = 0; i < mid; i++)
	{
		lim = mid + i;
		if (lim > R) break;

		q = i; p = q + 1;

		while (q >= 0) {
			if (VALT<N>(_vn[q]) < VALT<N>(_vn[p])) {
				tiny = _vn[q];
				_vn[q] = _vn[p];
				_vn[p] = tiny;
			}

			if (VALT<N>(_vn[q]) < VALT<N>(_vn[lim])) {
				tiny = _vn[q];
				_vn[q] = _vn[lim];
				_vn[lim] = tiny;
			}

			q--; p--;
		}
	} // ..phase 1 passed

	lim = R;

	for (int t = 0; t < mid; t++, lim--) {
		p = t; q = p + 1;

		if (lim < 0) break;

		if (VALT<N>(_vn[p]) < VALT<N>(_vn[q])) {
			tiny = _vn[p];
			_vn[p] = _vn[q];
			_vn[q] = tiny;
		}

		if (VALT<N>(_vn[p]) < VALT<N>(_vn[lim])) {
			tiny = _vn[p];
			_vn[p] = _vn[lim];
			_vn[lim] = tiny;
		}
	}
}




#ifdef MIXHUFF_USE_THREAD
template < class N >
inline static void merge_sort(std::vector<node>& _vn, const std::size_t _Size) {
	if (_Size < (std::size_t)10) return;

	int l = 0, r = 0;
	int nDivs = (int)_Size / 4;
	std::thread* pt = new std::thread[4];

	for (std::size_t k = 0; k < 4; k++) {
		r = l + nDivs;

		pt[k] = std::thread{ [&_vn,l,r]() {
			range_sort<N>(_vn, l, r);
		} };

		pt[k].join();

		l += nDivs;
	}
}
#endif



inline static void add_Nodes(std::vector<node>& _nodes, const NODE_T _nod)
{
	int val = _nod._v;
	node _tmp;

	if (_nodes.empty()) {
		if (_nod() != 0)
		{
			HS<NODE_T> = cElem<NODE_T>(_nod._v);
			_nodes.push_back(_nod);
		}
		return;
	}

	// if an element has been added before..
	if (HS<NODE_T>.get(_nod._v)._v > 0) return;
	else
	{
		_nodes.push_back(_nod);
	}

	return;
}



inline static void filter_Nodes(std::vector<node>& _dest, const std::vector<node>& _src) {

	double _count = 0.00, fc = 0.00;
	std::size_t i = 0;
	const std::size_t LENZ = _src.size();
	NODE_T nc;
	CSIZE = (LONGFLOAT)LENZ;

	if (_src.empty()) return;

	for (i = 0; i < LENZ; i++) {
		if (_src[i].Value() == 0) continue;
		else {
			nc = _src[i];
			break;
		}
	}

	for (; i < LENZ; i++) {
		// comparing value of both side
		if (nc._v == _src[i].Value()) {
			++_count;
			fc = (_count / CSIZE) * 100.00;
		}
		else {
			nc._freq = fc;
			ZEROES(_count, fc); // clear temporary vars
			add_Nodes(_dest, nc);

			if (_src[i].Value() != _src[i + 1].Value()) {
				nc = _src[i];
				nc._freq = (1.00 / CSIZE) * 100.00;
				add_Nodes(_dest, nc);
				nc._v = _src[i + 1].Value();
				continue;
			}
			else {
				nc = _src[i];
				++_count;
			}
		}
	}
	HS<NODE_T>.Sweep();
}



inline void _TREE::traverse_encode(std::map<int, char>& _mPair, const node* const _fRoot)
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
			if (ENCODE_SCHEMA)
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

		if (ASSERT_P(_pt->links[R]) && _pt->links[R]->Value() > 0 && !_pt->links[R]->Visited())
		{
			if (ENCODE_SCHEMA)
			{
				_bt = (char*)concat_str(_bt, inttostr(R));
				_pt->links[R]->setVisit(true);
				RPRINT(_bt); RPRINT("->"); RPRINT(_pt->links[R]->dataValue()); RET;
				_bt = (char*)rtrim(_bt);
			}
			else
			{
				_bt = (char*)concat_str(_bt, inttostr(R));
				_bpt._data = _pt->links[R]->dataValue();
				_bpt._val = biXs.value_from_bitstr(_bt);
				_mPair.emplace(std::pair<int, char>{_bpt._val, _bpt._data});
				_bpt = {};
				_bt = (char*)rtrim(_bt);
				_pt->links[R]->setVisit(true);
			}
		}
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
			if (ENCODE_SCHEMA)
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

			if (ENCODE_SCHEMA)
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

