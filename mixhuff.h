/* Using GPL v. 3.0 License */
#pragma once


#ifndef REQUIRE_H
	#include "mixutil.h"
#endif


#ifndef MX_BIT
	#include "mixbit.h"
#endif


static LONGFLOAT CSIZE = 0.00;


// Forward Declaration of Any Immediate Required Elements..
struct node;

	
constexpr int R = 0;
constexpr int L = 1;
constexpr int ROOT = -1;


struct node {
	node();
	node(int64_t&&);
	node(char&&, int64_t&&); // for data & frequency

	node(const node&); // overloaded copy
	node(node&&);	// move

	const node& operator= (const node&);
	node&& operator= (node&&) noexcept;
	~node();

	void setData(int64_t&&);
	void setFrequencyData(int64_t&&);
	
	const int64_t& Value() const;
	const char& dataValue() const;
	const int64_t& FrequencyData() const;
	const int64_t& Count() const;
	const int64_t& Code() const;

	node&& Release() const;
	
	// implicit conversion
	operator int() const;

	const int& operator()() const;
	void Print() const;
	
private:
	int64_t _fdata;
	char _data;
	
};



#ifndef HUFF_TREE
#define HUFF_TREE

#ifndef REQUIRE_H
	#include <vector>
	#include <queue>
#endif
	
#endif



#ifndef _TYPE_INFO_H
#define _TYPE_INFO_H
	#include <typeinfo>

struct _TREE {

	// get the encoded bits of data from A Vector 
	static inline const std::vector<BPAIR>& CodeMap() {
		return _TREE::_vPair;
	}

	// creates a huffman tree-view like scheme from the vector nodes data
	static inline void plot_tree(const std::vector<node>&, const double&);

	// reclaims allocated resources from _TREE
	inline static  void Clean() {
		_vPair.clear();
	}
	
private:

	// Enforces the uniqueness of each bit in the vector
	static inline void enforce_unique(std::vector<BPAIR>&);

	// iterates through the dataset elements in the vector to project a certain section of tree-view
	static inline void schema_Iter(const std::vector<node>&, const double);

	// directly create a huffman encoding table from the std::vector<node> without actually prebuilt the tree
	static inline const bool create_encoding(const size_t&, const size_t&, int64_t&, const std::vector<node>&);

	static std::vector<BPAIR> _vPair;
};

std::vector<BPAIR> _TREE::_vPair = {};




// Extracts nodes information from the vector
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



struct fqLess
{
	const bool operator()(const node& _First, const node& _Second)
	{
		return (_First.FrequencyData() < _Second.FrequencyData() );
	}
};


struct bitLess
{
	const bool operator()(const int64_t& _bp1st, const int64_t& _bp2nd)
	{
		return (_bp1st < _bp2nd );
	}
};


struct chrLess
{
	const bool operator()(const char& _c1, const char& _c2)
	{
		return ( _c1 < _c2 );
	}
};


#ifndef MX_HUFF_IMPLS

/* Filters priority queue nodes and compute the frequency of each node */
inline static void filter_pq_nodes(std::vector<node>&, std::priority_queue<node>&);

#include "mixhuff_impls.h"

#endif


	// Node Class Impl..
	node::node() :_data(0), _fdata(0)
	{
		
	}


	node::node(int64_t&& _Val): _data((int)_Val), _fdata(0)
	{
		
	}

	
	node::node(char&& _c, int64_t&& _fv) : _data(_c), _fdata(_fv)
	{
		
	}


	node::node(const node& rNod) {
		if (this == &rNod) return;
		this->_data = rNod._data;
		this->_fdata = rNod._fdata;
	}


	node::node(node&& rvNod) {
		if (this == &rvNod) return;
		this->_data = rvNod._data;
		this->_fdata = rvNod._fdata;
		rvNod.~node();
	};


	
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
		rvNod._fdata = 0;
		
		rvNod.~node();

		return std::move(*this);
	}


	node::~node() {
		
	}


	void node::setData(int64_t&& uc) {
		this->_data = (int)uc;
	}


	void node::setFrequencyData(int64_t&& fc) {
		this->_fdata = fc;
	}


	// Get Accessor Methods..
	const int64_t& node::Value() const {
		return this->_data;
	}


	const char& node::dataValue() const {
		return (int)this->_data;
	}


	const int64_t& node::FrequencyData() const {
		return this->_fdata;
	}

	
	// implicit conversion
	node::operator int() const {
		return (int)this->_data;
	}

	node&& node::Release() const {
		return node(std::move(*this));
	}


	const int& node::operator()() const {
		return (int)this->_data;
	}

	
	void node::Print() const {
		RPRINT(this->_data); RPRINT("->"); RPRINT(this->_fdata);
		RET;
	}



inline void _TREE::plot_tree(const std::vector<node>& _fpNods, const double& _compRate)
{
	schema_Iter(_fpNods, _compRate);
}



