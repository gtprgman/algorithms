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
	char _data;
	int _val;
};



struct node {
	node();
	node(const char); // for data tree
	node(const int); // for data tree
	node(const double); // for frequency tree
	node(const int, const double);
	
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

	const node Release() const;
	
	// implicit conversion
	operator int() const;

	const int operator()() const;
	void Print() const;
	
private:
	double _fdata;
	int _data;
	
};



#ifndef HUFF_TREE
#define HUFF_TREE
	#include <vector>
	#include <queue>

	
#endif



#ifndef MX_BIT
	#include "mixbit.h"
	BitN biXs;
#endif




#ifndef _TYPE_INFO_H
#define _TYPE_INFO_H
	#include <typeinfo>

struct _TREE {
	
	// get the encoded bits of data from a map
	static inline std::vector<BPAIR>&& CodeMap() {
		return std::move(_vPair);
	}

	// create a huffman tree-view like scheme from the vector nodes data
	static inline void plot_tree(const std::vector<node>&);


	// reclaim allocated resources from _TREE
	inline static  void Clean() {
		_vPair.clear();
	}
	
private:
	// iterate through the dataset elements in the vector to project a certain section of tree-view
	static inline void schema_Iter(const std::vector<node>&);

	// directly create huffman encoding table from std::vector<node> without prebuilt the tree
	static inline void create_encoding(const int, const int, std::string&, const std::vector<node>&);

	static std::vector<BPAIR> _vPair;
};

std::vector<BPAIR> _TREE::_vPair = {};



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
inline static void filter_pq_nodes(std::vector<node>&, node&&, const std::size_t);

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

		this->_data = rNod._data;
		this->_fdata = rNod._fdata;
		
		return (*this);
	}


	node&& node::operator= (node&& rvNod) noexcept {
		if (this == &rvNod) return std::move(*this);

		this->_data = rvNod._data;
		this->_fdata = rvNod._fdata;
	
		rvNod._data = 0;
		rvNod._fdata = 0.00;
		
		rvNod.~node();

		return std::move(*this);
	}


	node::~node() {
		
	}


	void node::setData(const int uc) {
		this->_data = uc;
	}


	void node::setFrequencyData(const double fc) {
		this->_fdata = fc;
	}


	// Get Accessor Methods..
	const int node::Value() const {
		return this->_data;
	}


	const char node::dataValue() const {
		return (char)this->_data;
	}


	const double node::FrequencyData() const {
		return this->_fdata;
	}

	
	// implicit conversion
	node::operator int() const {
		return this->_data;
	}

	const node node::Release() const {
		node&& tmp = node(*this);
		this->~node();
		return std::remove_all_extents_t<node&&>(tmp);
	}


	const int node::operator()() const {
		return this->_data;
	}

	
	void node::Print() const {
		RPRINT(this->_data); RPRINT("->"); RPRINT(this->_fdata);
		RET;
	}


inline void _TREE::plot_tree(const std::vector<node>& _fpNods)
{
	schema_Iter(_fpNods);
}

