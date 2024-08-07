/* Debugging Stages --> in Progress */
#pragma once

#ifndef REQUIRE_H
#include "mixutil.h"
#endif


static LONGFLOAT CSIZE = 0.00;


#ifndef MX_HASH
	#include "mixhash.h"
	template <class Ty>
	cHash<cElem<Ty>> HS(4096); // a preset of 4K size
#endif




// Forward Declaration of Any Immediate Required Elements..
	struct node;

	const ULONG L_HEIGHT(const node* const);
	const ULONG R_HEIGHT(const node* const);




enum class huffDir : std::int8_t { UNKNOWN = -1, ZERO = 0, ONE = 1 };

enum class tDir : std::int8_t {
	UNKNOWN = -1,
	LEFT = static_cast<std::int8_t>(huffDir::ONE),
	RIGHT = static_cast<std::int8_t>(huffDir::ZERO)
};

constexpr std::int8_t R = static_cast<std::int8_t>(tDir::RIGHT);
constexpr std::int8_t L = static_cast<std::int8_t>(tDir::LEFT);
constexpr std::int8_t ROOT = static_cast<std::int8_t>(tDir::UNKNOWN);



struct NODE_T;


// a data structure of a Pair of Bit and Byte
struct BPAIR {
	
	LINT _data;
	UINT _val;
	
};


struct node {
	node();
	
	node(const char); // for data tree
	node(const LongRange); // for data tree
	node(const double); // for frequency tree
	node(const LongRange, const double);
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


	void Add(node&);
	
	static void setSize(const std::size_t);
	static void setRoot(const node** const);
	static void setRoot_stack(const node*);

	constexpr const node* Find(const double,const LongRange);

	void setCode(const bool);
	void setData(const LongRange);
	void setFrequencyData(const double);
	void setCount(const double);
	void setFreq(const double);
	
	void setVisit(const bool);
	void setDelete(const bool);

	const LongRange Value() const;
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
	operator LongRange() const;

	const LongRange operator()() const;
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
	LongRange _data;
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
	#include <deque>
	#include <map>

	std::vector<std::unique_ptr<node>> _repo;
	std::vector<node*> _nRepo;
	std::map<LINT, node*> _Map;
	
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


#define LCOUNT(_p) std::cout << "L_H: " << L_HEIGHT(_p) << "\n";
#define RCOUNT(_p) std::cout << "R_H: " << R_HEIGHT(_p) << "\n";



#ifndef HUFF_TREE
#define HUFF_TREE

#ifndef MX_BIT
	#include "mixbit.h"
	BitN biXs;
#endif


#ifndef _DEALLOCS_H
#include <vector>
#endif

#ifndef _TYPE_INFO_H
#define _TYPE_INFO_H
	#include <typeinfo>

struct _TREE {
	static node* _Root;

	static inline void Info()
	{
		RET2();
		LCOUNT(_Root);
		RCOUNT(_Root);
		RET2();
	}
};

node* _TREE::_Root = nullptr;

#endif


// to construct a temporary node object, uses this structure for safety reason
struct NODE_T {
	LongRange _v;
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

	NODE_T(const LongRange _vc) {
		_v = _vc;
		_freq = 0.00;
	}

	NODE_T(const double _fc) {
		_v = 0;
		_freq = _fc;
	}

	NODE_T(const LongRange _vc, const double _fc) {
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


	
	const LongRange operator()() const {
		return _v;
	}


	// implicit conversion
	operator node() const {
		node _nd(_v, _freq);
		return _nd;
	}

	// implicit conversion
	operator LongRange() const
	{
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


// get the number of nodes in the outer left branches relative to the root node.
inline const ULONG L_HEIGHT(const node* const _pRoot ) {
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


// get the number of nodes in the outer right branches relative to the root node.
inline const ULONG R_HEIGHT(const node* const _pRoot) {
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
	
	_vt = (std::strcmp("__int64", typeid(_vt).name()))?
		(N)_Nod.FrequencyData() : (N)_Nod.Value();

	return std::remove_all_extents_t<N>(_vt);
}


// returns the value of a node specified by a pointer to any node in the tree.
template < class N > 
constexpr inline const N VALX(const node* const _p) {
	N _v;

	if (nullptr == _p) return 0;
	_v = (std::strcmp("__int64", typeid(_v).name()))? (N)_p->FrequencyData() : (N)_p->Value();

	return std::remove_all_extents_t<N>(_v);
}



// convert a specified node to the frequency node
inline const node TO_FREQ_NODE(const node& _nod) {
	node _fNod = _nod.FrequencyData(); // construct frequency node
	_fNod.setData(_nod.Value());
	return _fNod;
}



/* extracts information from a node with a frequency value '_fv'
   ,this function needs to be initiated with a call to 'setRoot()' before used.
*/ 
constexpr const node* NODE(const double _fv) {
	return (node::_main)->Find(_fv,0);
}



/* Get a pointer to any node of the tree with a specified value '_v'
  'setRoot()' must be called first before use
*/ 
constexpr const node* PNODE(const double _fv) {
	return (node::_main)->Find(_fv,0);
}



// returns the last modified node's data
inline const node* RECENT() {
	return (node::_recent);
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
		printf("(%lld.00, %.2f %%Fqr)\n", (_p)->Value(), (const double)(_p->FrequencyData()));
	else std::cout << 0.00 << "\n";
}



// searches a particular node's value relative to the root node
constexpr inline const node* seek_n(const node* const uRoot, const double fv, const LongRange c) {
	node* tmp = (CONST_PTR)uRoot;
	node* bckup = nullptr;

	while (nullptr != tmp) {
		if (VALX<double>(tmp) > fv ) tmp = LEFT(tmp);

		else if (VALX<double>(tmp) < fv ) tmp = RIGHT(tmp);

		else if (VALX<double>(tmp) == fv ) {
			bckup = tmp;
			break;
		}

		else if ISNULL(tmp) {
			bckup = nullptr;
			break;
		}
	}

	tmp = nullptr;
	return (bckup);
}




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
inline void NPRINT(const std::vector<node>& _vn) {
	for (const node& _ne : _vn) {
		RPRINT( _ne.dataValue() ); RPRINT(" ");
		RPRINT(_ne.FrequencyData() ); RET;
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
void build_huffman_tree(std::vector<node>&);


// build a complete table data from the huffman encoded bits pattern
void build_huffman_code(std::vector<BPAIR>&, const node*);

/* sorts the nodes in the vector in the decreasing order, the size_t
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

/*  Filter nodes to a separate vector container,
	the nodes in the source vector must be sorted before apply the filter. */ 
void filter_Nodes(std::vector<node>&, const std::vector<node>&);

/* add a node to the vector container, the method incorporate
   methods for restricting any data with the same value for being
   entered twice.
*/ 
void add_Nodes(std::vector<node>&, const NODE_T);


	// Node Class Impl..
	node::node() : xDir(tDir::UNKNOWN), _data(0), _fdata(0.00),
		nCount(0.00), freq(0.00),cDir(0), _visited(0), _deleted(0)
	{
		this->links[L] = nullptr;
		this->links[R] = nullptr;
	}



	node::node(const char _uChar) : xDir(tDir::UNKNOWN), _fdata(0.00),
		_data(_uChar), nCount(0.00), freq(0.00), _visited(0), _deleted(0)
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


	node::node(const LongRange c): xDir(tDir::UNKNOWN), 
		_data(c),nCount(0.00), freq(0.00), _fdata(0.00), cDir(0),  
		_visited(0), _deleted(0)
	{
		this->links[L] = nullptr;
		this->links[R] = nullptr;
	}


	node::node(const LongRange _c, const double _fv) : xDir(tDir::UNKNOWN),
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



	void node::Add(node& _fv) 
	{
		node* _pNode = nullptr;
		double fc = 0.00;

		if (nullptr == this) return;

		node* _pThis = (node* const)this;

		if (_pThis->FrequencyData() > _fv.FrequencyData())
		{
			if (ASSERT_P(_pThis->links[L])) {
				_pNode = _pThis->links[L];
				_pNode->Add(_fv);
				node::_recent = (CONST_PTR)PNODE(VALT<double>(_fv));
				_Map.emplace(std::pair<LINT, node*>(_fv, node::_recent));
			}
			else {
				_pThis->links[L] = (CONST_PTR)ALLOC_N(&_fv);
				(_pThis->links[L])->setCode(L);
				(_pThis->links[L])->setCount(_pThis->Count() + 1.0);
				node::_recent = _pThis->links[L];
				_Map.emplace(std::pair<LINT, node*>(_fv, node::_recent));
			}

		}
		else if (_pThis->FrequencyData() < _fv.FrequencyData())
		{
			if (ASSERT_P(_pThis->links[R])) {
				_pNode = _pThis->links[R];
				_pNode->Add(_fv);
				node::_recent = (CONST_PTR)PNODE(VALT<double>(_fv));
				_Map.emplace(std::pair<LINT, node*>(_fv, node::_recent));

			}
			else {
				_pThis->links[R] = (CONST_PTR)ALLOC_N(&_fv);
				(_pThis->links[R])->setCode(R);
				(_pThis->links[R])->setCount(_pThis->Count() + 1.0);
				node::_recent = _pThis->links[R];
				_Map.emplace(std::pair<LINT, node*>(_fv, node::_recent));
			}

		}
		else if (_pThis->FrequencyData() == _fv.FrequencyData())
		{
			node::_totSizes += 1.0;
			fc = (_pThis->FrequencyData() + (_fv.FrequencyData() / 100.00)) +
				(0.5 * (2.5 + node::_totSizes));
			_fv.setFrequencyData(fc);
			_pThis->Add(_fv);
			node::_recent = (CONST_PTR)PNODE(fc);
			_Map.emplace(std::pair<LINT, node*>(_fv, node::_recent));
		}
	
		// automatically add to garbage collector
		_Deleter.push(node::_recent);
	}
	


	// the total size of a data source
	void node::setSize(const std::size_t _sizes) {
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



	constexpr const node* node::Find(const double fc, const LongRange c) {
		
		return seek_n(this, fc, c);
	}
		

	
	void node::setData(const LongRange uc) {
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
	const LongRange node::Value() const {
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

	
	// implicit conversion
	node::operator LongRange() const {
		return _data;
	}

	const node node::Release() const {
		node&& tmp = node(*this);
		this->~node();
		return std::remove_all_extents_t<node&&>(tmp);
	}


	const LongRange node::operator()() const {
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


	 

	inline void build_huffman_tree(std::vector<node>& _fNods)
	{
		const std::size_t TSZ = _fNods.size();
		const std::size_t halfTSz = (TSZ / 2);

		_TREE::_Root = (CONST_PTR)ALLOC_N<node>(NODE_T(0,100.00) );
		node::setRoot(ROOT2P(&_TREE::_Root));
		node::setSize(TSZ);

		_TREE::_Root->links[R] = (CONST_PTR)ALLOC_N<LINT>(0);
		_TREE::_Root->links[R]->setCode(R);


		// fill up the first half of the branches..
		for (std::size_t i = 0; i < halfTSz; i++)
			_TREE::_Root->Add(_fNods.at(i));


		// spill up to the right branches of the tree
		node* _rightLeft = _TREE::_Root->links[R];
		*_rightLeft = _fNods.at(halfTSz);

		for (std::size_t j = halfTSz + 1; j < TSZ; j++)
		{
			_rightLeft->links[L] = (CONST_PTR)ALLOC_N<LINT>(0);
			_rightLeft->links[L]->setCode(L);
			*_rightLeft->links[L] = _fNods.at(j);
			_rightLeft = _rightLeft->links[L];

		}
		_rightLeft = nullptr;
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



void add_Nodes(std::vector<node>& _nodes, const NODE_T _nod) 
{
	LongRange val = _nod._v;
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
	if (HS<NODE_T>.get((LINT)_nod)._v > 0) return;
	else
	{
		_nodes.push_back(_nod);
	}
		
	return;
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




inline void build_huffman_code(std::vector<BPAIR>& vtPair, const node* _fRoot) 
{
	node* _pt = nullptr;
	char* _bt = (char*)"0";
	BPAIR _bpt = {}; 
	
	if (_fRoot != nullptr) _pt = (node*)_fRoot;
	else return;

	// encoding bits pattern from the left branches..
	ULONG _leftEnd = L_HEIGHT(_pt);

	for (ULONG i = 1; i <= _leftEnd; i++)
	{
		if (ASSERT_P(_pt->links[L]))
		{
			_pt = _pt->links[L];

			if (_pt->Visited()) continue;

			_bt = (char*)concat_str(_bt, inttostr(L));

			_bpt._data = _pt->Value();
			_bpt._val = biXs.value_from_bitstr(_bt);
			vtPair.push_back(_bpt);
			_bpt = {};

			passed_by(_pt); // signed as visited
		}
		else
			break;
	}

	// encoding bits pattern from the right branches..
	_pt = (node*)_fRoot->links[R];
	_bpt = {};
	_bt = (char*)"10";
	
	ULONG _rightLeftEnd = L_HEIGHT(_pt);

	for (ULONG j = 1; j <= _rightLeftEnd; j++)
	{
		if (ASSERT_P(_pt->links[L]))
		{
			_pt = _pt->links[L];
			if (_pt->Visited()) continue;

			_bt = (char*)concat_str(_bt, inttostr(L));

			_bpt._data = _pt->Value();
			_bpt._val = biXs.value_from_bitstr(_bt);
			vtPair.push_back(_bpt);
			_bpt = {};

			passed_by(_pt); // signed as visited
		}
		else
			break;
	}

	_pt = nullptr;
}

