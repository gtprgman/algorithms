/* Using GPL v. 3.0 License */
#pragma once


#ifndef REQUIRE_H
	#include "mixutil.h"
#endif



static LONGFLOAT CSIZE = 0.00;


// Forward Declaration of Any Immediate Required Elements..
struct node;

	
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

	void setData(const int);
	void setFrequencyData(const double);
	
	const int Value() const;
	const char dataValue() const;
	const double FrequencyData() const;
	const double Count() const;
	const int Code() const;

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
	
	
private:
	double _fdata;
	int _data;
	
};



#ifndef _DEALLOCS_H
#define _DEALLOCS_H

#ifndef HUFF_TREE 
	#include <vector>
	#include <queue>
	#include <map>
	
#endif

#endif



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
	
	// get the encoded bits of data from a map
	static inline std::map<int, char>&& CodeMap() {
		return std::move(_mPair);
	}

	// extract a schematic diagram of the build-up huffman tree. (Debugging)
	static inline void plot_huffman_tree(const std::vector<node>&);

	
private:
	// extract a schematic view of a built huffman tree
	static inline void extract_schema(const std::vector<node>&);

	
	static std::map<int, char> _mPair;
};


std::map<int, char> _TREE::_mPair = {};

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


// Extract nodes information from the vector
inline static void NPRINT(const std::vector<node>& _vn)
{
	for (const node& _e : _vn)
	{
		RPRINT(_e.dataValue()); RPRINT("->"); RPRINT(_e.FrequencyData());
		RET;
	}
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



#ifndef MX_HUFF_IMPLS

/* Filter priority queue nodes and compute the frequency of each node */
inline static void filter_pq_nodes(std::vector<node>&,NODE_T&&,const std::size_t);

#include "mixhuff_impls.h"

#endif


	// Node Class Impl..
	node::node() :_data(0), _fdata(0.00)
	{
		
	}


	node::node(const char _uChar) :_fdata(0.00),_data((int)_uChar)
	{
		
	}

	node::node(const double frq_data):_fdata(frq_data)
	{
		
	}

	node::node(const int c): _data(c), _fdata(0.00) 
	{
		
	}


	node::node(const int _c, const double _fv) : _data(_c), _fdata(_fv)
	{
		
	}


	/*
	* construct a node from a temporary node object free of any entangled pointers
	   relations, where only the data & frequency members are taking into consideration
	*/ 

	node::node(NODE_T& _NodT): _data(0), _fdata(0.00)
	{
		_data = _NodT._v;
		_fdata = _NodT._freq;
	}


	node::node(const NODE_T& _tmpNod) : _data(0) , _fdata(0.00)
	{
		_data = _tmpNod._v;
		_fdata = _tmpNod._freq;
	}


	node::node(NODE_T&& _nodT): _data(0), _fdata(0.00)
	{
		_data = _nodT._v;
		_fdata = _nodT._freq;
		
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

		_data = rNod._data;
		_fdata = rNod._fdata;
		
		return (*this);
	}


	node&& node::operator= (node&& rvNod) noexcept {
		if (this == &rvNod) return std::move(*this);

		_data = rvNod._data;
		_fdata = rvNod._fdata;
	
		rvNod._data = 0;
		rvNod._fdata = 0.00;
		
		rvNod.~node();

		return std::move(*this);
	}


	node::~node() {
		
	}


	void node::setData(const int uc) {
		_data = uc;
	}


	void node::setFrequencyData(const double fc) {
		_fdata = fc;
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
		RPRINT(this->_data); RPRINT("->"); RPRINT(this->_fdata);
		RET;
	}



inline void _TREE::plot_huffman_tree(const std::vector<node>& _fpNods)
{
	extract_schema(_fpNods);
}

