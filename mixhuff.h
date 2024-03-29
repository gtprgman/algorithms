/* Using License: GPL v 3.0 */
#pragma once

using Bit = unsigned char;
using UINT = unsigned int;
using ULONG = unsigned long;
using Byte = unsigned long;
using LongRange = long long;
using LONGFLOAT = long double;


enum class huffDir : std::int8_t { UNKNOWN = -1, ZERO = 0, ONE = 1 };

enum class tDir : std::int8_t {
	UNKNOWN = -1,
	LEFT = static_cast<std::int8_t>(huffDir::ONE),
	RIGHT = static_cast<std::int8_t>(huffDir::ZERO)
};

constexpr std::int8_t R = static_cast<std::int8_t>(tDir::RIGHT);
constexpr std::int8_t L = static_cast<std::int8_t>(tDir::LEFT);
constexpr std::int8_t ROOT = static_cast<std::int8_t>(tDir::UNKNOWN);


/* this structure is used to note the index to the first and the last address of a
   number of pointers to node stored in a vector */
struct LINK_ID {
	std::size_t _First;
	std::size_t _Last;
	std::size_t _Size;
} vNodeLnk;

struct NODE_T;


// a data structure of a Pair of Bit and Byte
struct BPAIR {
	Bit _bit;
	Byte _cdata;
	std::string _bitStr;
	

	const Byte operator()(void) const {
		return _cdata;
	}

	const bool operator< (const BPAIR& _bPair) {
		return (_cdata < _bPair._cdata);
	}

	const bool operator== (const BPAIR& _bPair) {
		return (_cdata == _bPair._cdata);
	}

	const bool operator> (const BPAIR& _bPair) {
		return (_cdata > _bPair._cdata);
	}

	const bool operator != (const BPAIR& _bPair) {
		return (_cdata != _bPair._cdata);
	}
};


struct node {
	node();
	
	node(const Byte); // for data tree
	node(const double); // for frequency tree
	node(NODE_T&); // construct from NODE_T
	node(NODE_T&&); // construct from rvalue NODE_T
	node(const NODE_T&); // construct from NODE_T
	node(node&); // copy
	node(const node&); // overloaded copy
	node(node&&) noexcept;	// move

	const node& operator= (const node&);
	node&& operator= (node&&) noexcept;
	~node();


	void Add(const Byte);
	void Add(const double);
	
	static void setSize(std::size_t const);
	static void setRoot(const node** const);
	static void setRoot_stack(const node*);

	template < class N >
	constexpr const node* Find(const N);

	void setCode(const bool);
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
	const bool Code() const;

	const bool Visited() const;
	const bool Deleted() const;
	const node Release() const;
	
	//Implicit conversion
	operator NODE_T() const;

	const Byte operator()(void) const;
	const bool operator< (const node&) const;
	const bool operator> (const node&) const;
	const bool operator!= (const node&) const;
	const bool operator== (const node&) const;

	void Print() const;
	static void Dispose();

	tDir xDir;
	node* links[2];

	static node* _main, *_recent;
	static node** _ppRoot;
	static double _totSizes;
	static std::size_t _nRec;
	
private:
	bool cDir;
	double nCount,freq, _fdata;
	Byte _data;
	bool _visited, _deleted;
};

double node::_totSizes = 0.00;
std::size_t node::_nRec = 0;
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
	
	#define NUM_BITS(_X_) (const int)_X_

	std::vector<std::unique_ptr<node>> _repo;
	std::vector<node*> _nRepo;

	struct HF_REC {
		HF_REC() {
			_data = 0;
		}

		HF_REC(std::vector<bool>& _bts, Byte _dta) {
			_bits = _bts;
			_data = _dta;
		}

		~HF_REC() {
			reset();
		}

		HF_REC(const HF_REC& _hfr) {
			if (this == &_hfr) return;
			*this = _hfr;
		}


		HF_REC(HF_REC&& _hfr) {
			if (this == &_hfr) return;
			*this = std::move(_hfr);
		}


		const HF_REC& operator= (const HF_REC& _hfc) {
			if (this == &_hfc) return (*this);

			_bits = _hfc._bits;
			_data = _hfc._data;

			return (*this);
		}


		HF_REC&& operator= (HF_REC&& _hf) {
			if (this == &_hf) return std::move(*this);

			_bits = _hf._bits;
			_data = _hf._data;

			_hf.~HF_REC();
		
			return std::move(*this);
		}

		
		const char* Bits() const {
			std::size_t sz = _bits.size();
			
			for (std::size_t j = 0; j < sz; j++)
				if (_bits[j]) strcat((char*)_cb.c_str(), "1"); else
					strcat((char*)_cb.c_str(), "0"); 

			return _cb.data();
		}


		void reset() {
			if (!_bits.empty()) _bits.clear();
			if (!_cb.empty()) _cb.clear();
			this->_data = 0;
		}

		std::string _cb;
		std::vector<bool> _bits;
		Byte _data;
	};

	

	// custom deleter for std::unique_ptr<node>
	struct N_DELETER {
		void operator()(node* _p0) {
			_nRepo.emplace_back(_p0->links[1]);
			
			while (nullptr != _p0) {
				_nRepo.emplace_back(_p0->links[0]);
				_p0 = _p0->links[0];
			}

			for (node* p_ : _nRepo) p_->~node();
		}
	};

	HF_REC _hc;
#endif


static struct _Deallocator {
	
	void push(node* _del) {
		_repo.emplace_back(_del);
	}

	void dispose_all() {
		if (_repo.empty()) return; 
		for (std::vector<std::unique_ptr<node>>::iterator vi = _repo.begin();
			vi != _repo.end(); vi++) {
			vi->reset(nullptr);
			vi->release();
		}
			
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


// to construct a temporary node object, uses this structure for safety reason
struct NODE_T {
	Byte _v;
	double _freq;

	NODE_T() : _v(0), _freq(0.00) {}

	~NODE_T() {
		_v = 0;
		_freq = 0.00;
	}

	NODE_T(const int _ch) {
		_v = _ch;
		_freq = 0.00;
	}

	NODE_T(const Byte _vc) {
		_v = _vc;
		_freq = 0.00;
	}

	NODE_T(const Byte _vc, const double _fc) {
		_v = _vc;
		_freq = _fc;
	}

	NODE_T(node&& _rvn) {
		_v = _rvn.Value();
		_freq = _rvn.FrequencyData();
		_rvn.~node();
	}

	NODE_T(const node& _uNod) {
		_v = _uNod.Value();
		_freq = _uNod.FrequencyData();
	}

	NODE_T(NODE_T& rNod) {
		if (this == &rNod) return;
		*this = rNod;
	}

	// NODE_T copy ctor
	NODE_T(const NODE_T& _rNod) {
		if (this == &_rNod) return;
		*this = _rNod;
	}

	const NODE_T& operator= (const NODE_T& _nod) {
		if (this == &_nod) return *this;
		_v = _nod._v;
		_freq = _nod._freq;

		return *this;
	}

	// NODE_T rvalue ctor
	NODE_T(NODE_T&& _rvNodT) {
		if (this == &_rvNodT) return;
		*this = std::move(_rvNodT);
	}

	
	NODE_T&& operator= (NODE_T&& _rvNodT) {
		if (this == &_rvNodT) return std::move(*this);
		_v = _rvNodT._v;
		_freq = _rvNodT._freq;
		_rvNodT.~NODE_T();

		return std::move(*this);
	}

	const Byte operator()(void) const {
		return _v;
	}

	const bool operator < (const NODE_T& rNodT) const {
		return (_v < rNodT._v);
	}

	const bool operator != (const NODE_T& rNodT) const {
		return (_v != rNodT._v);
	}

	const bool operator == (const NODE_T& rNodT) const {
		return (_v == rNodT._v);
	}

	const bool operator > (const NODE_T& rNodT) const {
		return (_v > rNodT._v);
	}
	
};

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
	node* _p = new node(v);
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
	return ISNULL(_any)? nullptr : _any->links[L];
}


inline node* const RIGHT(const node* const _any) { 
	return ISNULL(_any)? nullptr : _any->links[R];
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
template < class N >
constexpr inline const N VALT(const node& _Nod) {
	N _vt;
	
	_vt = (std::strcmp("unsigned long", typeid(_vt).name()))?
		(N)_Nod.FrequencyData() : (N)_Nod.Value();

	return std::remove_all_extents_t<N>(_vt);
}


// returns the value of a node specified by a pointer to any node in the tree.
template < class N > 
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
 'setRoot()' must be initiated first before use. This function is valid
 only on a data or frequency tree but before plotted to a huffman tree.
*/
template <class N>
constexpr inline const char DATA(const N _v) {
	node* tmp = (CONST_PTR)node::_main;
	const Byte data_value = tmp->Find<N>(_v)->Value();
	NULLP(tmp);
	return (char)data_value;
}

/* returns the data representation of any node in the huffman tree,
*  this should be used only on a generated huffman tree.
*/
inline const char DATAX(const node* _ft) {
	return (char)_ft->Value();
}


/* returns the data representation of a node, which
  node is already converted to a frequency node.
*/
inline const char DATAC(const node& _fNod) {
	return (char)_fNod.Value();
}



// create an instantaneous node object from a specified Byte value '_c'
inline const node TO_NODE(const Byte _c) {
	node _Nod = _c;
	return _Nod;
}


// convert a specified node to the frequency node
inline const node TO_FREQ_NODE(const node& _nod) {
	node _fNod = _nod.Freq(); // construct frequency node
	_fNod.setData(_nod.Value());
	return _fNod;
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



/* Get an instantaneous node object as specified by a data value 'ch',
   this could apply only when the data tree has been built. */ 
const NODE_T ANODE(const char ch) {
	NODE_T _ntx = node();

	node* _nx = (CONST_PTR)PNODE<Byte>((Byte)ch);

	if (nullptr != _nx) {
		_ntx = TO_FREQ_NODE(*_nx);	// _nx is a pointer to any node on the tree
		return _ntx; // return a copied NODE_T
	}

	return _ntx; // return a copied NODE_T
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
template <class T >
constexpr std::size_t inline total_values(const T& _any, const LongRange _Count) {
	std::size_t nums = 0;
	for (LongRange j = 0; j < _Count; j++)
		nums = nums + (std::size_t)_any[j];

	return nums;
}



// puts a mark-up on a node as 'visited'
inline void passed_by(const node* const _p = nullptr ) {
	if (!_p->Visited())
		((node* const)_p)->setVisit(true);
}


inline void print_vf(const node* const _p) {
	if (ASSERT_P(_p))
		printf("(%u.00, %.2f %%Fqr)\n", (_p)->Value(), (const double)(_p->Freq()));
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

inline void RET2() {
	RET;
	RET;
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


/* construct an instantaneous node object with a data specified as '_ch',
 this can be used anytime whenever we want an instantaneous node object,
 despite of whether a data tree has been built or not, especially in the
 case where we want to supply an instantaneous node object as argument to
 the search_Node function. */ 
#define nodeX(_ch) node((Byte)_ch)


/* construct an instantaneous NODE_T object with a data specified as '_ch' 
   this is useful whenever we want an instantaneous NODE_T object supplied
   as argument to the vector_search function */
#define nodeY(_ch) (Byte)_ch

// Print a collection of nodes from the vector
inline void NPRINT(const std::vector<node>& _vn) {
	int col = 0;
	for (const node& _ne : _vn) {
		col++;
		RPRINT(_ne.dataValue());
		if (col > 79) {
			col = 0;
			RET;
		}
	}
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


// Add a node as specified by the Byte parameter to the tree 
void data_tree_add(const node*, const Byte);
void freq_add_from_node(const node*, const node&);
void build_huffman_tree(const std::vector<node*>&);

/* sorts nodes in the vector in an increasing order, the size_t
   argument should be (n - 1). */
template <class N>
void sort_Nodes(std::vector<node>&, const std::size_t);

/* sort the nodes in ascending order using partial sort methods,
  the first element's pos should be 0 and the last element should be (n - 1), 
  you should precede the call to this one before calling any subsequent sorting
  mechanism */ 
template <class N>
void range_sort(std::vector<node>&, const LongRange, const LongRange);


#ifndef MIXHUFF_USE_THREAD
#define MIXHUFF_USE_THREAD
	#include <thread>
/*
 Use threading processes to sort each subdivided data set,
 the total datum to be sorted must be greater than 10 ( n > 10 ),
 the '_Size' parameter argument should be the total size of the 
 vector.
*/
	template< class N >
	void merge_sort(std::vector<node>&, const std::size_t);
#endif


/* search a node in the vector container using binary search method,
   this function can apply only on a vector node sorted on data value. */
template < class T , class E = NODE_T>
const bool vector_search(const std::vector<T>&, const E ,T&);


/* search a node in the vector using linear search method,
  argument to the second parameter must be supplied with ' nodeX(_ch)' macro. */ 
const bool search_Node(const std::vector<node>&, const NODE_T);

/*  Filter nodes to a separate vector container,
	the nodes in the source vector must be sorted before apply the filter. */ 
void filter_Nodes(std::vector<node>&, const std::vector<node>&);

/* add a node to the vector container, the method incorporate
   methods for restricting any data with the same value for being
   entered twice.
*/ 
void add_Nodes(std::vector<node>&, const NODE_T);

/* generate a huffman tree from the vector nodes data,
   the nodes in the vector must be sorted on frequency before use
*/
const void huff_tree_create(std::vector<node*>&, const std::vector<node>&, const std::size_t);

// Huffman Encoding initiated by this function
void huffman_encode(std::vector<HF_REC>&,const node* const);


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
		nCount(0.00), freq(0.00),cDir(0), _visited(0), _deleted(0)
	{
		this->links[L] = nullptr;
		this->links[R] = nullptr;
	}


	node::node(const double frq_data):xDir(tDir::UNKNOWN), _fdata(frq_data),
		_data(0),nCount(0.00), freq(0.00), cDir(0), _visited(0), _deleted(0)
	{
		this->links[L] = nullptr;
		this->links[R] = nullptr;
	}


	node::node(const Byte c): xDir(tDir::UNKNOWN), 
		_data(c),nCount(0.00), freq(0.00), _fdata(0.00), cDir(0),  
		_visited(0), _deleted(0)
	{
		this->links[L] = nullptr;
		this->links[R] = nullptr;
	}

	/*
	* construct a node from a temporary node object free of any entangled pointers
	   relations, where only the data & frequency members are taking into consideration
	*/ 

	node::node(NODE_T& _NodT): xDir(tDir::UNKNOWN),
		_data(0), nCount(0.00), freq(0.00), _fdata(0.00), cDir(0),
		_visited(0), _deleted(0)
	{
		_data = _NodT._v;
		_fdata = _NodT._freq;
		this->links[L] = nullptr;
		this->links[R] = nullptr;
	}


	node::node(const NODE_T& _tmpNod) : xDir(tDir::UNKNOWN),
		_data(0), nCount(0.00), freq(0.00), _fdata(0.00), cDir(0),
		_visited(0), _deleted(0) 
	{
		_data = _tmpNod._v;
		_fdata = _tmpNod._freq;
		this->links[L] = nullptr;
		this->links[R] = nullptr;
	}


	node::node(NODE_T&& _nodT):xDir(tDir::UNKNOWN),
		_data(0), nCount(0.00), freq(0.00), _fdata(0.00), cDir(0),
		_visited(0), _deleted(0) 
	{
		_data = _nodT._v;
		_fdata = _nodT._freq;
		this->links[L] = nullptr;
		this->links[R] = nullptr;
		_nodT.~NODE_T();
	}


	node::node(node& rNod) {
		if (this == &rNod) return;
		*this = rNod;
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

		rvNod.cDir = 0;
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
		 cDir = 0;
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

		{
			node* _curr = (CONST_PTR)_p;
			double nParts = 0.00;

			if (nullptr == _curr) return;

			const Byte _Vr = _curr->Value();

			if ((_Vr) > (uc))
				if (ASSERT_P(_curr->links[L])) {
					_curr = _curr->links[L];
					_curr->Add(uc);
					node::_recent = (CONST_PTR)_curr->Find<Byte>(uc);
				}
				else {
					_curr->links[L] = (CONST_PTR)ALLOC_N<const Byte>(uc);
					(_curr->links[L])->setCode(L);
					(_curr->links[L])->setCount((_curr->links[L])->Count() + 1.0);
					nParts = ((_curr->links[L])->Count() / node::_totSizes) * 100.00;
					(_curr->links[L])->setFreq(nParts);
					node::_recent = _curr->links[L];
				}

			else if ((_Vr) < (uc))
				if (ASSERT_P(_curr->links[R])) {
					_curr = _curr->links[R];
					_curr->Add(uc);
					node::_recent = (CONST_PTR)_curr->Find<Byte>(uc);
				}
				else {
					_curr->links[R] = (CONST_PTR)ALLOC_N<const Byte>(uc);
					(_curr->links[R])->setCode(R);
					(_curr->links[R])->setCount((_curr->links[R])->Count() + 1.0);
					nParts = ((_curr->links[R])->Count() / node::_totSizes) * 100.00;
					(_curr->links[R])->setFreq(nParts);
					node::_recent = _curr->links[R];
				}

			else if ((_Vr) == (uc)) {
				_curr->setCount(_curr->Count() + 1.0);
				nParts = (_curr->Count() / node::_totSizes) * 100.00;
				_curr->setFreq(nParts);
				node::_recent = _curr;
			}

			// automatically add to garbage collector
			_Deleter.push(node::_recent);
			_curr = nullptr;
		}

		simple_avl::reset_data();
		simple_avl::set_LT(L_HEIGHT(_main));
		simple_avl::set_RT(R_HEIGHT(_main));
		simple_avl::compute_balance();

		if (simple_avl::balance_value() > 1)
			simple_avl::make_balance();

		NULLP(_p);
	}


	void node::Add(const double _fv) {
		node* _p = nullptr;
		_p = (CONST_PTR)(this);
		node nodTmp = node(_fv);
		std::thread tfAdd = std::thread{ [_p, &nodTmp]() {freq_add_from_node(_p,nodTmp); } };
		tfAdd.join();
		++node::_nRec;
		NULLP(_p);
	}
	
	// the total size of a data source
	void node::setSize(std::size_t const _sizes) {
		_totSizes = (const double)_sizes;
	}


	void node::setRoot(const node** const _uRoot = nullptr) {
		if (node::_ppRoot == nullptr) node::_ppRoot = (node**)_uRoot;
		if (*_ppRoot != *_uRoot) *_ppRoot = (node*)*_uRoot;
		(*node::_ppRoot)->setCode(L);
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


	void node::setCode(const bool bc) {
		cDir = bc;
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


	const bool node::Code() const {
		return cDir;
	}

	
	const bool node::Visited() const { return _visited;  }

	const bool node::Deleted() const { return _deleted;  }

	// implicit conversion
	node::operator NODE_T() const {
		node _nc = _data;
		_nc.setFrequencyData(_fdata);
		return _nc;
	}

	const node node::Release() const {
		node&& tmp = node(*this);
		this->~node();
		return std::remove_all_extents_t<node&&>(tmp);
	}


	const Byte node::operator()(void) const {
		return _data;
	}

	const bool node::operator< (const node& _rNod) const {
		return (_data < _rNod._data);
	}


	const bool node::operator> (const node& _rNod) const {
		return (_data > _rNod._data);
	}


	const bool node::operator== (const node& _rNod) const{
		return (_data == _rNod._data);
	}

	const bool node::operator != (const node& _rNod) const {
		return (_data != _rNod._data);
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
	_nRoot = *node::_ppRoot;
	_root1 = (CONST_PTR)ALLOC_N(LEFT(_nRoot));
	_nRoot->links[L] = (CONST_PTR)ALLOC_N(RIGHT(_root1));
	_nRoot->links[L]->setCode(L);
	_root1->links[R] = _nRoot;
	_root1->links[R]->setCode(R);
	node::setRoot((const node** const)(&_root1));

	NULL2P(_nRoot, _root1);
}


inline void simple_avl::rotate_Left() {
	node* _nRoot = nullptr;
	_nRoot = *node::_ppRoot;
	_root2 = (CONST_PTR)ALLOC_N(RIGHT(_nRoot));
	_nRoot->links[R] = (CONST_PTR)ALLOC_N(LEFT(_root2));
	_nRoot->links[R]->setCode(R);
	_root2->links[L] = _nRoot;
	_root2->links[L]->setCode(L);
	node::setRoot((const node** const)(&_root2));

	NULL2P(_nRoot, _root2);
}


inline void data_tree_add(const node* const _p, const Byte _v) {
	
}


inline void freq_add_from_node(const node* _fRoot, const node& _dNod) {
	node* _fCurr = (CONST_PTR)_fRoot;

	/* in the frequency tree, we don't need any further the frequency value of
		any data frequency obtained earlier from the data tree */

	if (_fCurr->FrequencyData() > _dNod.FrequencyData())
		if (ASSERT_P(_fCurr->links[L])) {
			_fCurr = _fCurr->links[L];
			_fCurr->Add(_dNod.FrequencyData());
			node::_recent = (CONST_PTR)PNODE<double>(_dNod.FrequencyData());
		}
		else {
			_fCurr->links[L] = (CONST_PTR)ALLOC_N<double>(_dNod.FrequencyData());
			(_fCurr->links[L])->setCode(L);
			(_fCurr->links[L])->setCount((_fCurr->links[L])->Count() + 1.0);
			node::_recent = _fCurr->links[L];
		}

	else if (_fCurr->FrequencyData() < _dNod.FrequencyData())
		if (ASSERT_P(_fCurr->links[R])) {
			_fCurr = _fCurr->links[R];
			_fCurr->Add(_dNod.FrequencyData());
			node::_recent = (CONST_PTR)PNODE<double>(_dNod.FrequencyData());
		}
		else {
			_fCurr->links[R] = (CONST_PTR)ALLOC_N<double>(_dNod.FrequencyData());
			(_fCurr->links[R])->setCode(R);
			(_fCurr->links[R])->setCount((_fCurr->links[R])->Count() + 1.0);
			node::_recent = _fCurr->links[R];
		}

	else if (_fCurr->FrequencyData() == _dNod.FrequencyData()) {
		/* we need to enforce unique the frequency value of each data unit
			to distinguish any different data with the same frequency value.
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


inline void build_huffman_tree(const std::vector<node*>& _Vnt) {
	node* hfNode = (CONST_PTR)ALLOC_N<double>(100.00);
	node::setRoot(ROOT2P(&hfNode));
	const std::size_t _totF = (vNodeLnk._First * 2) + (vNodeLnk._Last * 2);
	node::setSize(_totF);
}


template <class N >
inline void sort_Nodes(std::vector<node>& vn, const std::size_t _Len) {
	LongRange i = 0, m = 0, t = 0, r = 0;
	LongRange mid = 0, _len = (LongRange)_Len;
	N _v2, _v4;
	NODE_T _n2, _n4;

	if (vn.empty()) return;

	mid = (_len / 2);
	m = mid;

	/*
	index: (t) & (r)
	accumulator: (i)
	threshold: (m) & (len)
	*/


	for (; i < _len; ) {
		for (; i < m; i++) {
			t = i; r = t + 1;
			
			while (t >= 0) {
				_v4 = VALT<N>(vn[r]); // supposed as larger
				_v2 = VALT<N>(vn[t]); // supposed as smaller

				if (_v2 > _v4) { // if [t] > [r]
					_n2 = vn[r]; // conserved the smaller
					vn[r] = vn[t]; // replace the smaller with the greater in the proper slot
					vn[t] = _n2; // replace the correct slot with the proper cardinal value
				}
				--t; r = t + 1;
			}
		}
		/* i' is likely approaching 'm' ; lim( 'i->m' )
			'mid < _len' for the next iterations of the inner 'for..loop' */
		m = _len;
	}	
}



template <class N>
void range_sort(std::vector<node>& _vn, const LongRange L, const LongRange R) {
	NODE_T tiny;
	LongRange q = 0, p = 0, lim = 0;
	LongRange mid = (L + R) / 2;

	while ((R - mid) < (mid - L)) --mid;

	for (LongRange i = 0; i < mid; i++)
	{
		lim = mid + i;
		if (lim > R) break;

		q = i; p = q + 1;

		while (q >= 0) {
			if ( VALT<N>(_vn[q]) > VALT<N>(_vn[p]) ) {
				tiny = _vn[p];
				_vn[p] = _vn[q];
				_vn[q] = tiny;
			}

			if ( VALT<N>(_vn[q]) > VALT<N>(_vn[lim]) ) {
				tiny = _vn[lim];
				_vn[lim] = _vn[q];
				_vn[q] = tiny;
			}

			q--; p--;
		}
	} // ..phase 1 passed

	lim = R;

	for (LongRange t = 0; t < mid; t++, lim--) {
		p = t; q = p + 1;

		if (lim < 0) break;

		if ( VALT<N>(_vn[p]) > VALT<N>(_vn[q]) ) {
			tiny = _vn[q];
			_vn[q] = _vn[p];
			_vn[p] = tiny;
		}

		if (VALT<N>(_vn[p]) > VALT<N>(_vn[lim]) ) {
			tiny = _vn[lim];
			_vn[lim] = _vn[p];
			_vn[p] = tiny;
		}
	}
}



#ifdef MIXHUFF_USE_THREAD
	template < class N >
	void merge_sort(std::vector<node>& _vn, const std::size_t _Size) {
		if (_Size < 10) return;

		std::size_t l = 0, r = 0;
		std::size_t nDivs = _Size / 4;
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


template < class T, class E>
inline const bool vector_search(const std::vector<T>& _vecNod, const E _fNod, T& _vElem) {
	LongRange vecSize = 0, M = 0, L = 0, R = 0; 
	LongRange L1 = 0, R1 = 0, M1 = 0, nSeek = 0;
	E Uc; // Uc: user's value
	T Vc; // Vector's value

	if (_vecNod.empty()) return 0;

	vecSize = (LongRange)_vecNod.size();
	R = vecSize; 
	R1 = R;
	M = (L + R) / 2;
	

	Uc = _fNod; // user's value

	do {
		Vc = _vecNod[M];  // vector's value

		if (Uc > Vc() ) {
			L = M;
			R = R1;
		}
		else if (Uc < Vc() ) {
			L = L1;
			R = M;
		}

		else if (Uc == Vc() ) {
			_vElem = _vecNod.at(M);
			break;
		}

		L1 = L;
		R1 = R;
		M = (L + R) / 2;

		++nSeek;
		if ((M < 0) || (M > vecSize )) break;
		if (nSeek > vecSize) break; 
		
	} while (Uc != Vc());

	return ( Uc == Vc() );
}



const bool search_Node(const std::vector<node>& _vec, const NODE_T _Nod) {
	bool _xFound = 0;
	Byte _v0, _v1;

	if (_vec.empty()) return 0;

	_v0 = _Nod._v;

	for (const node& e : _vec) {
		_v1 = e.Value();

		if (_v0 == _v1) {
			_xFound = 1;
			break;
		}
	}

	return _xFound;
}




void add_Nodes(std::vector<node>& _vec, const NODE_T _Nod) {
	bool _bFound = 0;
	const std::size_t _vSize = _vec.size();
	node _tmp;

	if (_vec.empty()) {
		if (_Nod._v != 0 )_vec.emplace_back(ANODE((char)_Nod._v));
		return;
	}
	
	if (_vSize < 20)
		_bFound = search_Node(_vec, _Nod._v );

	if (_vSize > 20) {
		sort_Nodes<Byte>(_vec, _vSize-1);
		_bFound = vector_search(_vec, _Nod._v,_tmp);
	}

	if (_bFound) return;

	if (_Nod._v != 0) _vec.emplace_back(ANODE((char)_Nod._v));
}



inline void filter_Nodes(std::vector<node>& _dest, const std::vector<node>& _src) {
	LongRange jPos = 0,_Len = 0;
	NODE_T nc;

	if (_src.empty()) return;

	_Len = (LongRange)_src.size();

	nc = ANODE(_src[0].dataValue());

	for (LongRange i = 0; i < _Len; i++) {
			// comparing value of both side
		if (VALT<double>(nc) == (VALT<double>(_src[i]))) {
			nc = ANODE(_src[i].dataValue());
			add_Nodes(_dest, nc);
		}
		else
			nc = ANODE(_src[i].dataValue());
	}
}



inline const void huff_tree_create(std::vector<node*>& vnt,std::vector<node>& vn, 
				const std::size_t _Len) {

	node* ft = nullptr, *f2t = nullptr;
	double fc = 0.00;

	for (std::size_t i = 0; i < _Len; i += 2) {
		fc = VALT<double>(vn[i]) + ((i + 1) == _Len)? 0.00 : VALT<double>(vn[i + 1]);
		ft = new node(fc);

		ft->links[L] =  (CONST_PTR)(&vn[i]);
		ft->links[L]->setCode(L);

		ft->links[R] = ((i+1)== _Len)? nullptr : (CONST_PTR)(&vn[i + 1]);
		if (ASSERT_P(ft->links[R])) ft->links[R]->setCode(R);
		fc = 0.00;
	
		vnt.push_back(ft);
	}

	ft = nullptr; fc = 0.00;

	if (vnt.empty()) return;
	const std::size_t _Len2 = vnt.size();

	fc = VALT<double>(*vnt[0]) + VALT<double>(*vnt[1]);
	ft = new node(fc);

	ft->links[L] = vnt[0];
	ft->links[L]->setCode(L);

	ft->links[R] = vnt[1];
	ft->links[R]->setCode(R);

	f2t = ft;
	vnt.push_back(ft);


	for (std::size_t j = 2; j < _Len2; j++) {
		fc = fc + VALT<double>(*vnt[j]);
		ft = new node(fc);

		ft->links[L] = f2t;
		ft->links[L]->setCode(L);
		
		ft->links[R] = vnt[j];
		ft->links[R]->setCode(R);

		f2t = ft;
		vnt.push_back(ft);
	}

	ft = nullptr;
	f2t = nullptr;

	vNodeLnk._First = _Len2 - 1;
	vNodeLnk._Last = vnt.size() - 1;

}



inline void huffman_encode(std::vector<HF_REC>& _tab, const node* const _f0t) {
	node* _ft = nullptr;

	if (_f0t == nullptr) return;
	
	const std::size_t LT = L_HEIGHT(_f0t);

	//_hc._bits.push_back(R);
	//_hc._bits.push_back(R);
	//_hc._data = _f0t->links[R]->links[R]->Value();

	//_tab.push_back(_hc);
	//_hc.reset();

	_hc._bits.push_back(R);
	_hc._bits.push_back(L);
	_hc._data = _f0t->links[R]->links[L]->Value();

	_tab.push_back(_hc);
	_hc.reset();

	if (nullptr != _f0t->links[L])
		_ft = _f0t->links[L];
	else
		return;

	_hc._bits.push_back(L); // outer left first branch

	for (std::size_t i = 1; i < LT; i++) {

		if ((i + 1) == LT) {
			_hc._bits.push_back(R);
			_hc._data = _ft->links[R]->Value();

			_tab.push_back(_hc);

			_hc._bits.pop_back();
			_hc._bits.push_back(L);
			_hc._data = _ft->links[L]->Value();

			_tab.push_back(_hc);
			_hc.reset();
			continue;
		}

		_hc._bits.push_back(R);
		_hc._bits.push_back(R);
		_hc._data = _ft->links[R]->links[R]->Value();

		_tab.push_back(_hc);

		_hc._bits.pop_back();
		_hc._bits.push_back(L);
		_hc._data = _ft->links[R]->links[L]->Value();

		_tab.push_back(_hc);

		_hc._bits.pop_back();
		_hc._bits.pop_back();
		_hc._bits.push_back(L);

		_ft = _ft->links[L];
		if (nullptr == _ft) break;	
	}
}
