/* Using GPL v. 3.0 License */
#pragma once


#ifndef REQUIRE_H
	#include "mixutil.h"
#endif

using namespace mix::auto_looper;


static LONGFLOAT CSIZE = 0.00;


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


constexpr int R = 0;
constexpr int L = 1;
constexpr int ROOT = -1;


struct NODE_T;


// a data structure of a Pair of Bit and Byte
struct BPAIR
{
	char _data : 8;
	int _val : 32;
};



struct node {
	node();
	
	node(const char); // for data tree
	node(const int); // for data tree
	node(const double); // for frequency tree
	node(const int, const double);
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

	constexpr const node* Find(const double);

	void setCode(const int);
	void setData(const int);
	void setFrequencyData(const double);
	void setCount(const double);
	
	void setVisit(const bool);
	void setDelete(const bool);

	const int Value() const;
	const char dataValue() const;
	const double FrequencyData() const;
	const double Count() const;
	const double Capacity() const;
	const int Code() const;

	const bool Visited() const;
	const bool Deleted() const;
	const node Release() const;
	
	// implicit conversion
	operator NODE_T() const;
	operator int() const;

	const int operator()() const;
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
	int cDir;
	double nCount,_fdata;
	int _data;
	bool _visited, _deleted;
};

double node::_totSizes = 0.00;
std::size_t node::_nRec = 0;
node* node::_main = nullptr;
node* node::_recent = nullptr;
node** node::_ppRoot = nullptr;



// allocate a new node
template <class N = int>
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
	#include <queue>
	#include <map>

	std::vector<std::unique_ptr<node>> _repo;
	std::vector<node*> _nRepo;
	//std::map<int, node*> _Map; // used primarily for debugging purposes.
	
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

	// construct a complete huffman tree data structure
	static inline void build_huffman_tree(std::vector<node>&);

	// extract a schematic diagram of the build-up huffman tree. (Debugging)
	static inline void plot_huffman_tree(const node* const);

	// build a complete table data from the huffman encoded bits pattern
	static inline void encode_tree(std::map<int,char>&, const node* const);

private:
	/* traversing the entire branches of the huffman tree and encodes every visited leaf node.
	   NB: The huffman tree must be built and exists before this function could apply. */
	static inline void traverse_encode(std::map<int,char>&,const node* const, const int);

	// proceed on the subnode's right branches
	inline static void iterRight(const node* const, const int, char*&, BPAIR&, std::map<int, char>&);

	// enforce unique bits
	inline static void enforce_unique(std::string&, char*&);
};

node* _TREE::_Root = nullptr;

#endif



// to construct a temporary node object, uses this structure for safety reason
struct NODE_T {
	int _v;
	double _freq;

	NODE_T() : _v(0), _freq(0.00) {}

	~NODE_T() {
		_v = 0;
		_freq = 0.00;
	}

	NODE_T(const char _ch) {
		_v = _ch;
		_freq = 0.00;
	}

	NODE_T(const int _vc) {
		_v = _vc;
		_freq = 0.00;
	}

	NODE_T(const double _fc) {
		_v = 0;
		_freq = _fc;
	}

	NODE_T(const int _vc, const double _fc) {
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


	const int operator()() const {
		return _v;
	}


	// implicit conversion
	operator node() const {
		node _nd(_v, _freq);
		return _nd;
	}

	// implicit conversion
	operator int() const
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


inline static bool ASSERT_P(const node* _ptr = nullptr) {
	return (_ptr != nullptr);
}


inline static node* const LEFT(const node* const _any) {
	return ISNULL(_any)? nullptr : _any->links[L];
}


inline static node* const RIGHT(const node* const _any) { 
	return ISNULL(_any)? nullptr : _any->links[R];
}


// get the number of nodes in the outer left branches relative to the root node.
inline static const ULONG L_HEIGHT(const node* const _pRoot ) {
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
inline static const ULONG R_HEIGHT(const node* const _pRoot) {
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
	
	_vt = (std::strcmp("int", typeid(_vt).name()))?
		(N)_Nod.FrequencyData() : (N)_Nod.Value();

	return _vt;
}


// returns the value of a node specified by a pointer to any node in the tree.
template < class N > 
constexpr inline const N VALX(const node* const _p) {
	N _v;

	if (nullptr == _p) return 0;
	_v = (std::strcmp("int", typeid(_v).name()))? (N)_p->FrequencyData() : (N)_p->Value();

	return _v;
}



/* extracts information from a node with a frequency value '_fv'
   ,this function needs to be initiated with a call to 'setRoot()' before used.
*/ 
constexpr const node* NODE(const double _fv) {
	return (node::_main)->Find(_fv);
}


/* Get a pointer to any node of the tree with a specified value '_fv'
  'setRoot()' must be called first before use
*/ 
constexpr const node* PNODE(const double _fv) {
	return (node::_main)->Find(_fv);
}


// returns the last modified node's data
inline static const node* RECENT() {
	return (node::_recent);
}


/* returns the frequency value of a node as specified by a pointer to any
  node in the tree.
*/
inline static const double FREQX(const node* _Nod) {
	return _Nod->FrequencyData();
}


// Extract nodes information from the vector
inline static void NPRINT(const std::vector<node>& _vn)
{
	for (const node& _e : _vn)
	{
		RPRINT(_e.dataValue()); RPRINT("->"); RPRINT(_e.FrequencyData());
		RET;
	}
}



inline static void print_vf(const node* const _p) {
	if (ASSERT_P(_p))
		printf("(%d.00, %.2f %%Fqr)\n", (_p)->Value(), (const double)(_p->FrequencyData()));
	else std::cout << 0.00 << "\n";
}



// searches a particular node's value relative to the root node
constexpr inline const node* seek_n(const node* const uRoot, const double fv) {
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


#define ZEROES(var1, var2) var1 = var2 = 0.00

#endif


template < class T = node>
struct fqLess
{
	const bool operator()(const T& _First, const T& _Second)
	{
		return _First.FrequencyData() < _Second.FrequencyData();
	}
};


// marks a node for deletion
inline static const bool DELETED(const node* const _p) {
	bool flDel = 0;
	if (!_p->Deleted()) {
		((node* const)_p)->setDelete(true);
		flDel = 1;
	}
	return flDel;
}


// puts a node to the garbage list
inline static void COLLECT(const node* const _p) {
	_Deleter.push((node* const)_p);
}



#ifndef MX_HUFF_IMPLS
constexpr int ENCODE_SCHEMA = -1;
constexpr int ENCODE_TREE = 0;

/* Filter priority queue nodes and compute the frequency of each node */
inline static void filter_pq_nodes(std::vector<node>&,NODE_T&&,const std::size_t);

#include "mixhuff_impls.h"

#endif


	// Node Class Impl..
	node::node() : xDir(tDir::UNKNOWN), _data(0), _fdata(0.00),
		nCount(0.00),cDir(0), _visited(0), _deleted(0)
	{
		this->links[L] = nullptr;
		this->links[R] = nullptr;
	}


	node::node(const char _uChar) : xDir(tDir::UNKNOWN), _fdata(0.00),
		_data((int)_uChar), nCount(0.00), _visited(0), _deleted(0)
	{
		this->links[L] = nullptr;
		this->links[R] = nullptr;
	}

	node::node(const double frq_data):xDir(tDir::UNKNOWN), _fdata(frq_data),
		_data(0),nCount(0.00), cDir(0), _visited(0), _deleted(0)
	{
		this->links[L] = nullptr;
		this->links[R] = nullptr;
	}


	node::node(const int c): xDir(tDir::UNKNOWN), 
		_data(c),nCount(0.00), _fdata(0.00), cDir(0),  
		_visited(0), _deleted(0)
	{
		this->links[L] = nullptr;
		this->links[R] = nullptr;
	}


	node::node(const int _c, const double _fv) : xDir(tDir::UNKNOWN),
		_data(_c), _fdata(_fv), nCount(0.00), cDir(0),
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
		_data(0), nCount(0.00),_fdata(0.00), cDir(0),
		_visited(0), _deleted(0)
	{
		_data = _NodT._v;
		_fdata = _NodT._freq;
		this->links[L] = nullptr;
		this->links[R] = nullptr;
	}


	node::node(const NODE_T& _tmpNod) : xDir(tDir::UNKNOWN),
		_data(0), nCount(0.00), _fdata(0.00), cDir(0),
		_visited(0), _deleted(0) 
	{
		_data = _tmpNod._v;
		_fdata = _tmpNod._freq;
		this->links[L] = nullptr;
		this->links[R] = nullptr;
	}


	node::node(NODE_T&& _nodT):xDir(tDir::UNKNOWN),
		_data(0), nCount(0.00), _fdata(0.00), cDir(0),
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
				//_Map.emplace(std::pair<int, node*>(_fv, node::_recent));
			}
			else {
				_pThis->links[L] = (CONST_PTR)ALLOC_N(&_fv);
				(_pThis->links[L])->setCode(L);
				(_pThis->links[L])->setCount(_pThis->Count() + 1.0);
				node::_recent = _pThis->links[L];
				//_Map.emplace(std::pair<int, node*>(_fv, node::_recent));
			}

		}
		else if (_pThis->FrequencyData() < _fv.FrequencyData())
		{
			if (ASSERT_P(_pThis->links[R])) {
				_pNode = _pThis->links[R];
				_pNode->Add(_fv);
				node::_recent = (CONST_PTR)PNODE(VALT<double>(_fv));
				//_Map.emplace(std::pair<int, node*>(_fv, node::_recent));

			}
			else {
				_pThis->links[R] = (CONST_PTR)ALLOC_N(&_fv);
				(_pThis->links[R])->setCode(R);
				(_pThis->links[R])->setCount(_pThis->Count() + 1.0);
				node::_recent = _pThis->links[R];
				//_Map.emplace(std::pair<int, node*>(_fv, node::_recent));
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
			//_Map.emplace(std::pair<int, node*>(_fv, node::_recent));
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
		(*node::_ppRoot)->setCode(ROOT);
		_main = (CONST_PTR)*_ppRoot;
	}


	void node::setRoot_stack(const node* const _root) {
		node::_main = (CONST_PTR)_root;
	}


	constexpr const node* node::Find(const double fc) {
		
		return seek_n(this, fc);
	}
		

	void node::setData(const int uc) {
		_data = uc;
	}


	void node::setFrequencyData(const double fc) {
		_fdata = fc;
	}


	void node::setCount(const double fc) {
		nCount = fc;
	}


	void node::setCode(const int bc) {
		cDir = bc;
	}

	void node::setVisit(const bool _v) {
		_visited = _v;
	}

	void node::setDelete(const bool _d) {
		_deleted = _d;
	}


	// Get Accessor Methods..
	const int node::Value() const {
		return _data;
	}


	const char node::dataValue() const {
		return (char)_data;
	}


	const double node::FrequencyData() const {
		return _fdata;
	}

	
	const double node::Count() const {
		return nCount;
	}
	
	const double node::Capacity() const {
		return _totSizes;
	}


	const int node::Code() const {
		return this->cDir;
	}

	
	const bool node::Visited() const { return this->_visited;  }

	const bool node::Deleted() const { return _deleted;  }


	// implicit conversion
	node::operator NODE_T() const {
		NODE_T nc(_data, _fdata);
		
		return std::forward<NODE_T>(nc);
	}

	
	// implicit conversion
	node::operator int() const {
		return _data;
	}

	const node node::Release() const {
		node&& tmp = node(*this);
		this->~node();
		return std::remove_all_extents_t<node&&>(tmp);
	}


	const int node::operator()() const {
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



inline void _TREE::build_huffman_tree(std::vector<node>& _fNods)
{
	const std::size_t TSZ = _fNods.size();
	const std::size_t halfTSz = (TSZ / 2);

	_TREE::_Root = (CONST_PTR)ALLOC_N<node>(NODE_T(0, 100.00));
	_TREE::_Root->setCode(0);
	node::setRoot(ROOT2P(&_TREE::_Root));
	node::setSize(TSZ);


	// fill up the first half of the branches..
	for (std::size_t i = 0; i < halfTSz; i++)
	{
		node _ntd = _fNods.at(i);
		_TREE::_Root->Add(_ntd);
	}

	// spill up to the right branches of the tree
	// create a new branches before appended to the right of the root
	_TREE::_Root->links[R] = (CONST_PTR)ALLOC_N<node>(NODE_T(0, 0.00));
	_TREE::_Root->links[R]->setCode(R);
	node* _rightLeft = _TREE::_Root->links[R]; // _rightLeft points to the right branches

	int _nDir = 0;
	*_rightLeft = _fNods[halfTSz];
	_rightLeft->setCode(_nDir);

	for (std::size_t j = halfTSz + 1; j < TSZ; j++)
	{
		_nDir = !_nDir;
		_rightLeft->links[_nDir] = (node*)ALLOC_N(_fNods[j]);
		_rightLeft->setCode(_nDir);
		_rightLeft = _rightLeft->links[_nDir];
	}


	_rightLeft = nullptr;
}



inline void _TREE::plot_huffman_tree(const node* const _fRoot)
{
	std::map<int, char> _map = {};
	traverse_encode(_map, _fRoot,ENCODE_SCHEMA);
}


inline void _TREE::encode_tree(std::map<int,char>& _mPair, const node* const _fRoot)
{
	traverse_encode(_mPair, _fRoot,ENCODE_TREE);
}


