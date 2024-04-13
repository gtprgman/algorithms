/* Debugging Stages --> in Progress */
#pragma once


using Bit = unsigned char;
using UINT = unsigned int;
using ULONG = unsigned long;
using Byte = unsigned long;
using LongRange = long long;
using LONGFLOAT = long double;

static LONGFLOAT CSIZE = 0.00;

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

	const bool operator< (const BPAIR& _bPair) const {
		return (_cdata < _bPair._cdata);
	}

	const bool operator== (const BPAIR& _bPair) const {
		return (_cdata == _bPair._cdata);
	}

	const bool operator> (const BPAIR& _bPair) const {
		return (_cdata > _bPair._cdata);
	}

	const bool operator != (const BPAIR& _bPair) const {
		return (_cdata != _bPair._cdata);
	}
};


struct node {
	node();
	
	node(const Byte); // for data tree
	node(const double); // for frequency tree
	node(const Byte, const double);
	node(NODE_T&); // construct from a ref to NODE_T
	node(NODE_T&&); // construct from rvalue NODE_T
	node(const NODE_T&); // construct from a ref to const NODE_T

	node(const NODE_T*); // construct from a ptr to const NODE_T
	node(node&); // simple copy
	node(const node&); // overloaded copy
	node(node&&) noexcept;	// move

	const node& operator= (const node&);
	node&& operator= (node&&) noexcept;
	~node();


	void Add(node&&);
	
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
	
	// implicit conversion
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



// allocate a new node
template <class N>
constexpr inline const node* const ALLOC_N(N const v) {
	node* _p = (node* const)ALLOC_N(new node(v));
	return (_p);
}



// allocate a new node by transferring resources from the old node
template <>
inline const node* const ALLOC_N<node*>(node* _nod) {
	node* tmp = new node(_nod->Release());
	return (tmp);
}



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
			_nRepo.emplace_back(_p0->links[R]);
			
			while (nullptr != _p0) {
				_nRepo.emplace_back(_p0->links[L]);
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
		_repo.clear();
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

struct _TREE {
	static node* _Root;
};

node* _TREE::_Root;

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

	NODE_T(const double _fc) {
		_v = 0;
		_freq = _fc;
	}

	NODE_T(const Byte _vc, const double _fc) {
		_v = _vc;
		_freq = _fc;
	}


	NODE_T(const node* _pNod)
	{
		_v = _pNod->Value();
		_freq = _pNod->FrequencyData();
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


	// implicit conversion
	operator node() const {
		node _nd(_v, _freq);
		return _nd;
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

	if (nullptr == _p) return 0;
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

/* returns the data representation of any node in the huffman tree.
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
inline const NODE_T TO_NODE(const Byte _c) {
	node _Nod = _c;
	return _Nod;
}


// convert a specified node to the frequency node
inline const node TO_FREQ_NODE(const node& _nod) {
	node _fNod = _nod.FrequencyData(); // construct frequency node
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




// returns the last modified node's data
inline const node* RECENT() {
	return (node::_recent);
}



/* returns the frequency of a data node's value '_v',
  'setRoot()' must be initiated first before use
*/ 
inline const double FREQ(const Byte _v) {
	node* tmp = (CONST_PTR)node::_main;
	double fqr = (tmp->Find<Byte>(_v))->FrequencyData();
	NULLP(tmp);
	return fqr;
}



/* returns the frequency value of a node as specified by a pointer to any
  node in the tree.
*/
inline const double FREQX(const node* _Nod) {
	return _Nod->FrequencyData();
}



// evaluates the sum of total elements' value in the array with '_Count' elements
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
		printf("(%u.00, %.2f %%Fqr)\n", (_p)->Value(), (const double)(_p->FrequencyData()));
	else std::cout << 0.00 << "\n";
}



// searches a particular node's value relative to the root node
template < class N >
constexpr inline const node* seek_n(const node* const uRoot, const N uc) {
	node* tmp = (CONST_PTR)uRoot;
	node* bckup = nullptr;

	while (nullptr != tmp) {
		if (VALX<N>(tmp) > uc) tmp = LEFT(tmp);
		else if (VALX<N>(tmp) < uc) tmp = RIGHT(tmp);

		else if (VALX<N>(tmp) == uc) {
			bckup = tmp;
			break;
		}

		else if ISNULL(tmp) {
			bckup = nullptr;
			break;
		}
		
	}

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

#define ZEROES(var1, var2) var1 = var2 = 0.00


// Print a collection of nodes from the vector
template <class T>
inline void NPRINT(const std::vector<T>& _vn) {
	int col = 0;
	for (const T& _ne : _vn) {
		col++;
		RPRINT( (char)VALT<Byte>((node)_ne)); RPRINT(" ");
		RPRINT(VALT<double>((node)_ne)); RET;
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


// transform into vector node
inline void transForm(std::vector<node>& _target, const std::vector<NODE_T>& _source) {
	PRINT("Adding.. ");
	for (const auto& e : _source)
		if (e._v != 0) _target.push_back(e);
}




// construct a complete huffman tree data structure
void build_huffman_tree(node* , const node*);

/* sorts nodes in the vector in the decreasing order, the size_t
   argument should be the total size of the vector. */
template <class N>
void sort_Nodes(std::vector<node>&, const std::size_t);

/* sort the nodes in decreasing order using partial sort methods,
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
   this function can apply only to a vector node sorted on data value. */
template < class T>
const bool vector_search(const std::vector<T>&, const NODE_T& ,T&);


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

/* generate a huffman tree branches from the vector nodes data,
   the nodes in the vector must be sorted on frequency before use
*/
const void huff_tree_create(const std::vector<node>&, const std::size_t);

// Huffman Encoding initiated by this function
void huffman_encode(std::vector<HF_REC>&,const node* const);


	
// transform std::vector<unique_ptr<node>> into std::vector<node>
inline void transForm2(std::vector<node>& _vNods) {
	for (std::size_t i = 0; i < _repo.size(); i++) {
		add_Nodes(_vNods, (NODE_T)*_repo[i]);
	}
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


	node::node(const Byte _c, const double _fv) : xDir(tDir::UNKNOWN),
		_data(_c), _fdata(_fv), nCount(0.00), freq(0.00), cDir(0),
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


	node::node(const NODE_T* _pNodT)
	{
		this->_data = _pNodT->_v;
		this->_fdata = _pNodT->_freq;
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

		links[L]->~node();
		links[R]->~node();

		links[L] = rNod.links[L];
		links[R] = rNod.links[R];

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

		if (ASSERT_P(links[L])) delete links[L];
		if (ASSERT_P(links[R])) delete links[R];

		links[L] = rvNod.links[L];
		links[R] = rvNod.links[R];

		rvNod.cDir = 0;
		rvNod.nCount = 0.00;
		rvNod._data = 0;
		rvNod._fdata = 0.00;
		rvNod.freq = 0.00;
		xDir = tDir::UNKNOWN;

		rvNod._visited = 0;
		rvNod._deleted = 0;

		rvNod.links[L] = nullptr;
		rvNod.links[R] = nullptr;
		rvNod.~node();

		return std::move(*this);
	}


	node::~node() {
		
	}



	
	void node::Add(node&& _fv) 
	{
		node* _pNode = nullptr;
		double fc = 0.00;

		if (nullptr == this) return;

		node* _pThis = (node* const)this;

		if (_pThis->FrequencyData() > _fv.FrequencyData())
		{
			if (ASSERT_P(_pThis->links[L])) {
				_pNode = _pThis->links[L];
				_pNode->Add(std::forward<node>(_fv));
				node::_recent = (CONST_PTR)PNODE<double>(VALT<double>(_fv));
			}
			else {
				_pThis->links[L] = (CONST_PTR)ALLOC_N(&_fv);
				(_pThis->links[L])->setCode(L);
				(_pThis->links[L])->setCount(_pThis->Count() + 1.0);
				node::_recent = _pThis->links[L];
			}

		}
		else if (_pThis->FrequencyData() < _fv.FrequencyData())
		{
			if (ASSERT_P(_pThis->links[R])) {
				_pNode = _pThis->links[R];
				_pNode->Add(std::forward<node>(_fv));
				node::_recent = (CONST_PTR)PNODE<double>(VALT<double>(_fv));
			}
			else {
				_pThis->links[R] = (CONST_PTR)ALLOC_N(&_fv);
				(_pThis->links[R])->setCode(R);
				(_pThis->links[R])->setCount(_pThis->Count() + 1.0);
				node::_recent = _pThis->links[R];
			}

		}
		else if (_pThis->FrequencyData() == _fv.FrequencyData())
		{
			fc = (_pThis->FrequencyData() + (_fv.FrequencyData() / 100.00));
			node::_totSizes += 1.0;
			_fv.setFrequencyData(fc);
			_pThis->Add(std::forward<node>(_fv));
			node::_recent = (CONST_PTR)PNODE<double>(fc);
		}
	
		// automatically add to garbage collector
		_Deleter.push(node::_recent);
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
		NODE_T nc(_data, _fdata);
		
		return std::forward<NODE_T>(nc);
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





inline void build_huffman_tree(node* _fRoot, const node* _f0t) {
	node _tmp = *_f0t;
	_fRoot->Add( std::forward<node>(_tmp) );
}




template <class N >
inline void sort_Nodes(std::vector<node>& vn, const std::size_t _Len) {
	LongRange j = 0, k = 0, r = 0, t = 0;
	const LongRange	_Max = (LongRange)_Len;
	N _v1, _v3, _v2, _v4;
	NODE_T _n1, _n3, _n2, _n4;

	if (vn.empty()) return;

	// apply quick sort on each half of the set
	for (std::size_t n = 0, e = (_Len / 2), u = _Len, d = e, l = e;
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
 for(std::size_t g = 0; g < _Len; g++) 
	for (LongRange i = 0, m = (_Max / 2); (i < m) && (m < _Max); i++, m++) {
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
			if ( VALT<N>(_vn[q]) < VALT<N>(_vn[p]) ) {
				tiny = _vn[q];
				_vn[q] = _vn[p];
				_vn[p] = tiny;
			}

			if ( VALT<N>(_vn[q]) < VALT<N>(_vn[lim]) ) {
				tiny = _vn[q];
				_vn[q] = _vn[lim];
				_vn[lim] = tiny;
			}

			q--; p--;
		}
	} // ..phase 1 passed

	lim = R;

	for (LongRange t = 0; t < mid; t++, lim--) {
		p = t; q = p + 1;

		if (lim < 0) break;

		if ( VALT<N>(_vn[p]) < VALT<N>(_vn[q]) ) {
			tiny = _vn[p];
			_vn[p] = _vn[q];
			_vn[q] = tiny;
		}

		if (VALT<N>(_vn[p]) < VALT<N>(_vn[lim]) ) {
			tiny = _vn[p];
			_vn[p] = _vn[lim];
			_vn[lim] = tiny;
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




template < class T >
inline const bool vector_search(const std::vector<T>& _vecNod, const NODE_T& _fNod, T& _vElem) {
	LongRange vecSize = 0, M = 0, L = 0, R = 0;
	LongRange L1 = 0, R1 = 0, M1 = 0, nSeek = 0;
	NODE_T Uc; // Uc: user's value
	T Vc; // Vector's value

	if (_vecNod.empty()) return 0;

	vecSize = (LongRange)_vecNod.size();
	R = vecSize;
	R1 = R;
	M = (L + R) / 2;


	Uc = _fNod; // user's value

	do {
		Vc = _vecNod[M];  // vector's value

		if (Uc() < Vc()) {
			L = M;
			R = R1;
		}
		else if (Uc() > Vc()) {
			L = L1;
			R = M;
		}

		else if (Uc() == Vc()) {
			_vElem = _vecNod.at(M);
			break;
		}

		L1 = L;
		R1 = R;
		M = (L + R) / 2;

		++nSeek;
		if ((M < 0) || (M > vecSize)) break;
		if (nSeek > vecSize) break;

	} while (Uc() != Vc());

	return (Uc() == Vc());
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




void add_Nodes(std::vector<node>& _nodes, const NODE_T _nod) 
{
	bool _bFound = 0;
	const std::size_t _vSize = _nodes.size();
	node _tmp;

	if (_nodes.empty()) {
		if (_nod() != 0) _nodes.push_back(_nod);
		return;
	}

	if (_nodes.size() < 20)
	{
		//PRINT("Linear search..");
		_bFound = search_Node(_nodes, _nod);

	}
	else if (_nodes.size() > 20)
	{
		//PRINT("Vector search..");
		_bFound = vector_search(_nodes, _nod, _tmp);
	}
		
	if (_bFound) return;
	else {
		if (_nod() != 0) _nodes.push_back(_nod);
		if (_nodes.size() > 20) sort_Nodes<Byte>(_nodes,_nodes.size());
	}
}




inline void filter_Nodes(std::vector<node>& _dest, const std::vector<node>& _src) {
	PRINT("Filtering.. "); RET;

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
			ZEROES(_count, fc);
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
}



inline const void huff_tree_create(const std::vector<node>& vn, const std::size_t _Len) {

	node* ft = nullptr, *f2t = nullptr;
	double fc = 0.00;

	_TREE::_Root = (CONST_PTR)ALLOC_N<double>(50.00);
	node::setRoot(ROOT2P(&_TREE::_Root));


	for (std::size_t i = 0; i < _Len; i += 2) {
		if (!ASSERT_P(&vn[i])) return;
		fc = (vn[i].FrequencyData() ) + ((i + 1) >= _Len)? 0.00 : (vn[i + 1].FrequencyData());
		ft = new node(fc);
		PRINT(ft->FrequencyData());

		ft->links[L] =  (CONST_PTR)(&vn[i]);
		ft->links[L]->setCode(L);

		ft->links[R] = ((i+1) >= _Len)? nullptr : (CONST_PTR)(&vn[i + 1]);
		if (ASSERT_P(ft->links[R])) ft->links[R]->setCode(R);
		fc = 0.00;
	
		build_huffman_tree(_TREE::_Root, ft);
	}
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
	_hc._bits.push_back(R);
	_hc._data = _f0t->links[R]->links[R]->Value();

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
