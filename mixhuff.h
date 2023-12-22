#pragma once

using UINT = unsigned int;
using ULONG = unsigned long;
using Byte = unsigned long;

enum class huffDir : std::int8_t { UNKNOWN = -1, ZERO = 0, ONE = 1 };
enum class huffTree : std::int8_t { UNKNOWN = 0, DATA_TREE = 1, FREQ_TREE = 2 };

enum class tDir : std::int8_t {
	UNKNOWN = -1,
	LEFT = static_cast<std::int8_t>(huffDir::ZERO),
	RIGHT = static_cast<std::int8_t>(huffDir::ONE)
};

constexpr std::int8_t ZERO = static_cast<std::int8_t>(tDir::LEFT);
constexpr std::int8_t ONE = static_cast<std::int8_t>(tDir::RIGHT);

struct HF_REC {
	char _code;
	double _freq;
	double _fdata;
	Byte _data;
};



struct node {
	node();
	
	node(const Byte); // for data tree
	node(const double); // for frequency tree
	node(const node&); // copy
	node(node&&) noexcept;	// move

	const node& operator= (const node&);
	node&& operator= (node&&) noexcept;
	~node();


	void Add(const Byte);
	void Add(const double);
	void Add(const node&);

	static void setSize(std::size_t const);
	static void setRoot(const node** const);
	static void setRoot_stack(const node*);

	template < class N >
	constexpr const node* Find(const N);

	void setCode(const char);
	void setData(const Byte);
	void setFrequencyData(const double);
	void setCount(const double);
	void setFreq(const double);
	
	void setVisit(const bool);
	void setDelete(const bool);

	const Byte Value() const;
	const char dataValue() const;
	const double FrequencyData() const;
	const double Freq() const;
	const double Count() const;
	const double Capacity() const;
	const char Code() const;

	const bool Visited() const;
	const bool Deleted() const;
	const node Release() const;
	
	void Print() const;
	static void Dispose();

	tDir xDir;
	node* links[2];

	static node* _main, *_recent;
	static node** _ppRoot;
	static double _totSizes;
	static huffTree xTree;
	
private:
	char cDir;
	double nCount,freq, _fdata;
	Byte _data;
	bool _visited, _deleted;
};

double node::_totSizes = 0.00;
huffTree node::xTree = huffTree::UNKNOWN;
node* node::_main = nullptr;
node* node::_recent = nullptr;
node** node::_ppRoot = nullptr;



struct simple_avl {
	static ULONG const LT_Count();
	static ULONG const RT_Count();
	static ULONG const balance_value();

	static void set_LT(ULONG const);
	static void set_RT(ULONG const);
	static void reset_data();
	static void compute_balance();
	static void make_balance();

	static void rotate_Right();
	static void rotate_Left();


	static node*_root1, *_root2;

private:
	static ULONG LT;
	static ULONG RT;
	static ULONG BAL;
};

node* simple_avl::_root1 = nullptr;
node* simple_avl::_root2 = nullptr;

ULONG simple_avl::LT = 0;
ULONG simple_avl::RT = 0;
ULONG simple_avl::BAL = 0;



#ifndef _DEALLOCS_H
#define _DEALLOCS_H

#ifndef HUFF_TREE
	#include <vector>
	std::vector<std::unique_ptr<node>> _repo;

#endif


static struct _Deallocator {
	
	void push(node* _del) {
		_repo.emplace_back(_del);
	}

	void dispose_all() {
		if (_repo.empty()) return; 
		for (std::vector<std::unique_ptr<node>>::iterator vi = _repo.begin();
			vi != _repo.end(); vi++) vi->reset(nullptr);
			
	}

	 
} _Deleter;

#endif




#ifndef HUFF_TREE
#define HUFF_TREE

#ifndef _DEALLOCS_H
#include <vector>
#endif


#ifndef _TYPE_INFO_H
#define _TYPE_INFO_H
	#include <typeinfo>
#endif

#define RET std::cout << "\n";

#define PRINT(_t) std::cout << (_t) << "\n";
#define RPRINT(_t) std::cout << (_t) << ", ";
#define ISNULL(p) ( p == nullptr )

#define NULLP(p) ( p = nullptr )

#define NULL2P(p1,p2) {\
	p1 = nullptr; \
	p2 = nullptr; \
}

#define CONST_PTR node* const
#define NODE2P const node** const
#define ROOT2P(_p) (NODE2P)(_p)


template <class N>
constexpr inline const node* const ALLOC_N(N const v) {
	node* _p = new node((const N)v);
	return (_p);
}


inline const node* const ALLOC_N(node* _nod) {
	node* tmp = new node(_nod->Release());
	return (tmp);
}


inline bool ASSERT_P(const node* _ptr = nullptr) {
	return (_ptr != nullptr);
}


inline node* const LEFT(const node* const _any) {
	return ISNULL(_any)? nullptr : _any->links[0];
}


inline node* const RIGHT(const node* const _any) { 
	return ISNULL(_any)? nullptr : _any->links[1];
}


inline ULONG L_HEIGHT(const node* const _pRoot ) {
	ULONG cnt = 0;
	node* _Curr = (node* const)_pRoot;
	if (nullptr == _Curr) return 0;
	
	while (nullptr != _Curr) {
		_Curr = LEFT(_Curr); 
		if (!ASSERT_P(_Curr) ) break;
			else cnt++;
	};

	return cnt;
}


inline ULONG R_HEIGHT(const node* const _pRoot) {
	ULONG nums = 0;
	node* _curr = (node* const)_pRoot;
	if (nullptr == _curr) return 0;

	while (nullptr != _curr) {
		_curr = RIGHT(_curr); 
		if (!ASSERT_P(_curr) ) break;
			else nums++;
	};

	return nums;
}



// returns the value of a node as specified by a reference to any node in the tree.
template < class N = double >
constexpr inline const N VALT(const node& _Nod) {
	N _vt;
	
	_vt = (std::strcmp("unsigned long", typeid(_vt).name()))?
		(N)_Nod.FrequencyData() : (N)_Nod.Value();

	return std::remove_all_extents_t<N>(_vt);
}


// returns the value of a node specified by a pointer to any node in the tree.
template < class N = double> 
constexpr inline const N VALX(const node* const _p) {
	N _v;

	_v = (std::strcmp("unsigned long", typeid(_v).name()))? (N)_p->FrequencyData() : (N)_p->Value();

	return std::remove_all_extents_t<N>(_v);
}



/* returns the bit code of a specified node's value '_v',
 'setRoot()' must be initiated first before use
*/ 
template <class N>
constexpr inline const char BIT(const N _v) {
	node* tmp = (CONST_PTR)node::_main;
	char ch = (tmp->Find<N>(_v))->Code();
	NULLP(tmp);
	return ch;
}



/* returns the data representation of a specified node's value '_v',
 'setRoot()' must be initiated first before use
*/
template <class N>
constexpr inline const N DATA(const N _v) {
	node* tmp = (CONST_PTR)node::_main;
	const char data_value = tmp->Find<N>(_v)->dataValue();
	NULLP(tmp);
	return data_value;
}



/* extracts information of a node with a specified value '_v'
   ,this function needs to initiate the call to 'setRoot()' before use
*/ 
template < class N >
constexpr inline void NODE(const N _v) {
	return (node::_main)->Find<N>(_v)->Print(); RET;
}


/* Get a pointer to any node of the tree with a specified value '_v'
  'setRoot()' must be called first before use
*/ 
template < class N >
constexpr const node* PNODE(const N _v) {
	return (node::_main)->Find<N>(_v);
}



// returns the last modified node's data
inline const node* RECENT() {
	return (node::_recent);
}



/* returns the frequency of a data node's value '_v',
  'setRoot()' must be initiated first before use
*/ 
inline double FREQ(const Byte _v) {
	node* tmp = (CONST_PTR)node::_main;
	double fqr = (tmp->Find<Byte>(_v))->Freq();
	NULLP(tmp);
	return fqr;
}



// evaluates the sum of total elements in the array with '_Count' elements
template <class T = char* >
constexpr std::size_t inline total_values(const void* _any, const std::size_t _Count) {
	std::size_t nums = 0;
	for (std::size_t j = 0; j < _Count; j++)
		nums = nums + (std::size_t)((const T)_any)[j];

	return nums;
}



// puts a mark-up on a node as 'visited'
inline void passed_by(const node* const _p = nullptr ) {
	if (!_p->Visited())
		((node* const)_p)->setVisit(true);
}


inline void print_vf(const node* const _p) {
	if (ASSERT_P(_p))
		printf("(%.2f, %.2f %%Fqr)\n", VALX(_p), (const double)(_p->Freq()));
	else std::cout << 0.00 << "\n";
}



// searchs a particular node's value relative to the root node
template < class N >
constexpr inline const node* seek_n(const node* const uRoot, const N uc) {
	node* tmp = (node* const)uRoot;
	node* bckup = tmp;

	while (nullptr != tmp) {
		if (VALX<N>(tmp) > uc) tmp = LEFT(tmp);
		else if (VALX<N>(tmp) < uc) tmp = RIGHT(tmp);

		else if ISNULL(tmp) {
			bckup = nullptr;
			break;
		}
		else if (VALX<N>(tmp) == uc) {
			bckup = tmp;
			break;
		}

	}

	bckup = tmp;
	NULLP(tmp);
	return (bckup);
}

constexpr char const* RET2() {
	return "\n\n";
}

#define LCOUNT(_p) std::cout << "L_H: " << L_HEIGHT(_p) << "\n";
#define RCOUNT(_p) std::cout << "R_H: " << R_HEIGHT(_p) << "\n";

#define ROW_FDATA_PRINT(_p) {\
	RPRINT(_p->FrequencyData()); RPRINT(","); RPRINT(_p->Freq());\
	RET;\
}


#define ROW_DATA_PRINT(_p) { \
	RPRINT(_p->Value()); RPRINT(","); RPRINT(_p->Freq()); \
	RET;\
}



inline const node TO_NODE(const Byte _c) {
	node _Nod = _c;
	return _Nod;
}



inline const node TO_FREQ_NODE(const node& _nod) { 
	node _fNod = std::floor(_nod.Freq()); // construct frequency node
	_fNod.setData(_nod.Value()); 
	return _fNod; 
}



#endif




// marks a node for deletion
inline const bool DELETED(const node* const _p) {
	bool flDel = 0;
	if (!_p->Deleted()) {
		((node* const)_p)->setDelete(true);
		flDel = 1;
	}
	return flDel;
}


// puts a node to the garbage list
inline void COLLECT(const node* const _p) {
	_Deleter.push((node* const)_p);
}


/* Forward Declaration of any needed functs .. */
void data_tree_add(const node*, const Byte);
void freq_add_from_node(const node*, const node&);


// sorts the frequency nodes in ascending order
template <class N>
constexpr void nodesSort(std::vector<node>&, const std::size_t);


/* constructs a huffman tree from the gathered frequency nodes,
	the frequency nodes in the list must be sorted before a huffman tree
	could properly be constructed, this is crucial.
*/
void huff_tree_create(const std::vector<node>&, const std::size_t);



// simple_avl class Impl..
	ULONG const simple_avl::LT_Count() { return LT;  }
	ULONG const simple_avl::RT_Count() { return RT;  }
	ULONG const simple_avl::balance_value() { return BAL;  }

	void simple_avl::set_LT(ULONG const _lc) {
		LT = _lc;
	}

	void simple_avl::set_RT(ULONG const _rc) {
		RT = _rc;
	}

	void simple_avl::reset_data() {
		LT = 0; RT = 0; BAL = 0;
	}

	void simple_avl::compute_balance() {
		BAL = (LT > RT)? LT - RT : RT - LT;
	}

	void simple_avl::make_balance() {
		if (LT > RT) rotate_Right();
		else rotate_Left();
	}

	

	// Node Class Impl..
	node::node() : xDir(tDir::UNKNOWN), _data(0), _fdata(0.00),
		nCount(0.00), freq(0.00),cDir('0'), _visited(0), _deleted(0)
	{
		node::xTree = huffTree::UNKNOWN;

		this->links[0] = nullptr;
		this->links[1] = nullptr;
	}


	node::node(const double frq_data):xDir(tDir::UNKNOWN), _fdata(frq_data),
		_data(0),nCount(0.00), freq(0.00), cDir('0'), _visited(0), _deleted(0)
	{
		node::xTree = huffTree::FREQ_TREE;

		this->links[0] = nullptr;
		this->links[1] = nullptr;
	}


	node::node(const Byte c): xDir(tDir::UNKNOWN), 
		_data(c),nCount(0.00), freq(0.00), _fdata(0.00), cDir('0'),  
		_visited(0), _deleted(0)
	{
		node::xTree = huffTree::DATA_TREE;

		this->links[0] = nullptr;
		this->links[1] = nullptr;
	}


	node::node(const node& rNod) {
		if (this == &rNod) return;
		*this = rNod;
	}


	node::node(node&& rvNod) noexcept {
		if (this == &rvNod) return;
		*this = rvNod;
	}


	const node& node::operator= (const node& rNod) {
		if (this == &rNod) return (*this);

		cDir = rNod.cDir;
		nCount = rNod.nCount;
		_data = rNod._data;
		_fdata = rNod._fdata;
		freq = rNod.freq;
		xDir = rNod.xDir;
		_visited = rNod._visited;
		_deleted = rNod._deleted;

		links[0] = rNod.links[0];
		links[1] = rNod.links[1];

		return (*this);
	}


	node&& node::operator= (node&& rvNod) noexcept {
		if (this == &rvNod) return std::move(*this);

		cDir = rvNod.cDir;
		nCount = rvNod.nCount;
		_data = rvNod._data;
		_fdata = rvNod._fdata;
		freq = rvNod.freq;
		xDir = rvNod.xDir;
		_visited = rvNod._visited;
		_deleted = rvNod._deleted;

		links[0] = rvNod.links[0];
		links[1] = rvNod.links[1];

		rvNod.cDir = -1;
		rvNod.nCount = 0.00;
		rvNod._data = 0;
		rvNod._fdata = 0.00;
		rvNod.freq = 0.00;
		xDir = tDir::UNKNOWN;

		rvNod._visited = 0;
		rvNod._deleted = 0;

		rvNod.links[0] = nullptr;
		rvNod.links[1] = nullptr;
		rvNod.~node();

		return std::move(*this);
	}


	node::~node() {
		 cDir = -1;
		 nCount = 0.00;
		 _data = 0;
		 freq = 0.00;
		 _fdata = 0.00;
		 xDir = tDir::UNKNOWN;

		 _visited = 0;
		 _deleted = 0;

		 this->links[0] = nullptr;
		 this->links[1] = nullptr;
	}


	void node::Add(const Byte uc) {
		node* _p = nullptr;
		_p = (CONST_PTR)(this);
		data_tree_add(_p, uc);
		
		simple_avl::reset_data();
		simple_avl::set_LT(L_HEIGHT(_main));
		simple_avl::set_RT(R_HEIGHT(_main));
		simple_avl::compute_balance();

		if (simple_avl::balance_value() > 1)
			simple_avl::make_balance();

		NULLP(_p);
	}


	void node::Add(const double dv) {
		node* _p = nullptr;
		_p = (CONST_PTR)(this);
		node nodTmp = node(dv);
		freq_add_from_node(_p, nodTmp);

		NULLP(_p);
	}


	void node::Add(const node& _Node) {
		node* _p = nullptr;
		_p = (CONST_PTR)(this);
		freq_add_from_node(_p, _Node);
	}


	
	// the total size of a data source
	void node::setSize(std::size_t const _sizes) {
		_totSizes = (const double)_sizes;
	}


	void node::setRoot(const node** const _uRoot = nullptr) {
		if (node::_ppRoot == nullptr) node::_ppRoot = (node**)_uRoot;
		if (*_ppRoot != *_uRoot) *_ppRoot = (node*)*_uRoot;
		(*node::_ppRoot)->setCode('0');
		_main = (CONST_PTR)*_ppRoot;
	}



	void node::setRoot_stack(const node* const _root) {
		node::_main = (CONST_PTR)_root;
	}



	template < class N>
	constexpr const node* node::Find(const N uc) {
		return seek_n<N>(this, uc);
	}
		
	
	void node::setData(const Byte uc) {
		_data = uc;
	}


	void node::setFrequencyData(const double fc) {
		_fdata = fc;
	}


	void node::setCount(const double fc) {
		nCount = fc;
	}


	void node::setFreq(const double dv) {
		freq = dv;
	}


	void node::setCode(const char ch) {
		cDir = ch;
	}

	void node::setVisit(const bool _v) {
		_visited = _v;
	}

	void node::setDelete(const bool _d) {
		_deleted = _d;
	}


	// Get Accessor Methods..
	const Byte node::Value() const {
		return _data;
	}


	const char node::dataValue() const {
		return (char)_data;
	}


	const double node::FrequencyData() const {
		return _fdata;
	}

	const double node::Freq() const {
		return this->freq;
	}


	const double node::Count() const {
		return nCount;
	}
	
	const double node::Capacity() const {
		return _totSizes;
	}


	const char node::Code() const {
		return cDir;
	}

	
	const bool node::Visited() const { return _visited;  }

	const bool node::Deleted() const { return _deleted;  }


	const node node::Release() const {
		node&& tmp = node(*this);
		this->~node();
		return std::remove_all_extents_t<node&&>(tmp);
	}



	void node::Print() const {
		std::cout << "Root:   "; 
		ASSERT_P(this)? print_vf(this) : print_vf(nullptr);

		std::cout << "(L)eft: ";
		ASSERT_P(LEFT(this))? print_vf(LEFT(this)) : print_vf(nullptr);

		std::cout << "(R)ight: "; 
		ASSERT_P(RIGHT(this))? print_vf(RIGHT(this)) : print_vf(nullptr);
		RET;
	}


	void node::Dispose() {
		_Deleter.dispose_all();
	}




// AVL Implementations
inline void simple_avl::rotate_Right() {
	node* _nRoot = nullptr;

	if (huffTree::DATA_TREE == node::xTree) {
		_nRoot = *node::_ppRoot;
		_root1 = (CONST_PTR)ALLOC_N(LEFT(_nRoot));
		_nRoot->links[0] = (CONST_PTR)ALLOC_N(RIGHT(_root1));
		_nRoot->links[0]->setCode('0');
		_root1->links[1] = _nRoot;
		_root1->links[1]->setCode('1');
		node::setRoot((const node** const)(&_root1));
	}

	NULL2P(_nRoot, _root1);
}

inline void simple_avl::rotate_Left() {
	node* _nRoot = nullptr;

	if (huffTree::DATA_TREE == node::xTree) {
		_nRoot = *node::_ppRoot;
		_root2 = (CONST_PTR)ALLOC_N(RIGHT(_nRoot));
		_nRoot->links[1] = (CONST_PTR)ALLOC_N(LEFT(_root2));
		_nRoot->links[1]->setCode('1');
		_root2->links[0] = _nRoot;
		_root2->links[0]->setCode('0');
		node::setRoot((const node** const)(&_root2));
	}

	NULL2P(_nRoot, _root2);
}


inline void data_tree_add(const node* const _p, const Byte _v) {
	node* _curr = (CONST_PTR)_p;
	double nParts = 0.00;

	if (nullptr == _curr) return;

	const Byte _Vr = _curr->Value();
		
	if ( (_Vr) > (_v) )
		if (ASSERT_P(_curr->links[0])) {
			_curr = _curr->links[0];
			_curr->Add((const Byte)_v);
			node::_recent = (CONST_PTR)_curr->Find<Byte>((const Byte)_v);
		}
		else {
			_curr->links[0] = (CONST_PTR)ALLOC_N<const Byte>((const Byte)_v);
			(_curr->links[0])->setCount((_curr->links[0])->Count() + 1.0);
			nParts = ((_curr->links[0])->Count() / node::_totSizes) * 100.00;
			(_curr->links[0])->setFreq(nParts);
			node::_recent = _curr->links[0];
		}

	else if ( (_Vr) < (_v) )
		if (ASSERT_P(_curr->links[1])) {
			_curr = _curr->links[1];
			_curr->Add((const Byte)_v);
			node::_recent = (CONST_PTR)_curr->Find<Byte>((const Byte)_v);
		}
		else {
			_curr->links[1] = (CONST_PTR)ALLOC_N<const Byte>((const Byte)_v);
			(_curr->links[1])->setCount((_curr->links[1])->Count() + 1.0);
			nParts = ((_curr->links[1])->Count() / node::_totSizes) * 100.00;
			(_curr->links[1])->setFreq(nParts);
			node::_recent = _curr->links[1];
		}

	else if ( (_Vr) == (_v) ) {
			_curr->setCount(_curr->Count() + 1.0);
			nParts = (_curr->Count() / node::_totSizes) * 100.00;
			_curr->setFreq(nParts);
			node::_recent = _curr;
		}
	
	// automatically add to garbage collector
	_Deleter.push(node::_recent);
	_curr = nullptr;
}




inline void freq_add_from_node(const node* _fRoot, const node& _dNod) {
	node* _fCurr = (CONST_PTR)_fRoot;

	/* in the frequency tree, we don't need any further the frequency value of 
	    any data frequency obtained earlier from the data tree */

	if (_fCurr->FrequencyData() > _dNod.FrequencyData()) 
		if (ASSERT_P(_fCurr->links[0])) {
			_fCurr = _fCurr->links[0];
			_fCurr->Add(_dNod);
			node::_recent = (CONST_PTR)PNODE<double>(_dNod.FrequencyData());
		}
		else {
			_fCurr->links[0] = (CONST_PTR)ALLOC_N<double>(_dNod.FrequencyData());
			(_fCurr->links[0])->setCode('0');
			(_fCurr->links[0])->setCount((_fCurr->links[0])->Count() + 1.0);
			node::_recent = _fCurr->links[0];
		}
	
	else if (_fCurr->FrequencyData() < _dNod.FrequencyData())
		if (ASSERT_P(_fCurr->links[1])) {
			_fCurr = _fCurr->links[1];
			_fCurr->Add(_dNod);
			node::_recent = (CONST_PTR)PNODE<double>(_dNod.FrequencyData());
		}
		else {
			_fCurr->links[1] = (CONST_PTR)ALLOC_N<double>(_dNod.FrequencyData());
			(_fCurr->links[1])->setCode('1');
			(_fCurr->links[1])->setCount((_fCurr->links[1])->Count() + 1.0);
			node::_recent = _fCurr->links[1];
		}

	else if (_fCurr->FrequencyData() == _dNod.FrequencyData()) {
		/* we need to enforce unique the frequency of each data unit
			for easing the organizer method of huffman concept 
		*/
		const double fcNew = (_dNod.FrequencyData() + (_dNod.FrequencyData() / 100.00)); // to enforce unique
		node::_totSizes += 1.0;
		_fCurr->Add(fcNew);
		node::_recent = (CONST_PTR)PNODE<double>(fcNew);
		_fCurr = node::_recent;
	}

	// automatically add to garbage collector
	_Deleter.push(node::_recent);
	_fCurr = nullptr;
}




template <class N>
constexpr inline void nodesSort(std::vector<node>& vn, const std::size_t _Len) {
std::size_t i = 0,j = 0,m = 0, t = 0, r = 0;
std::size_t mid = 0, _len = _Len;
N _v2 , _v4;
node _n2, _n4;

	mid = (_Len / 2);

	i = 0;  // accumulator: (i); index: (t), (r)
	m = mid; // threshold: (j), (_len)

	for (; i < _len; ) {
		for (; i < m; i++) {
			t = i; r = t + 1;

			while(t > 0) {
				_v4 = VALT<N>(vn[r]); // supposed as larger
				_v2 = VALT<N>(vn[t]); // supposed as smaller

				if (_v2 > _v4) {
					_n2 = vn[r]; // conserved the smaller
					vn[r] = vn[t]; // copy the greater to the smaller slot
					vn[t] = _n2; // replace the correct slot with the correct value
				}
				--t; r = t + 1;
			}
		}
	// 'i' is likely approaching 'm' ; lim( 'i->m' )
	m = _len; // 'mid < _len' for the next iterations of the inner 'for..loop'
  }	
}



inline void huff_tree_create(const std::vector<node>& vn, const std::size_t _Len) {
std::size_t i = 0;
	double fc = 0.00;
	// ft, f2t : branches of the huffman's tree
	node* ft = nullptr, *f2t = nullptr;

	
	// initialize fc before use
	fc = fc + VALT<double>(vn[0]) + VALT<double>(vn[1]);

	ft = (CONST_PTR)ALLOC_N<double>(fc);
	ft->links[0] = (CONST_PTR)(&vn[0]);
	ft->links[0]->setCode('0');

	ft->links[1] = (CONST_PTR)(&vn[1]);
	ft->links[1]->setCode('1');

	for (i = 2; i < _Len; i++) {
		// calculate root's frequency value
		fc = fc + VALT<double>(vn[i]);

		f2t = (CONST_PTR)ALLOC_N<double>(fc);
		f2t->links[0] = ft;
		f2t->links[0]->setCode('0');

		f2t->links[1] = (CONST_PTR)(&vn[i]);
		f2t->links[1]->setCode('1');

		ft = f2t;
	}

	NULLP(f2t);

	return (ft);	
}

