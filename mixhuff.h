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
	node(unsigned char&&); // data only
	node(unsigned char&&, int64_t&&); // for data & frequency

	node(const node&); // overloaded copy
	node(node&&);	// move

	const node& operator= (const node&);
	node&& operator= (node&&) noexcept;
	~node();

	void setData(unsigned char&&);
	void setFrequencyData(int64_t&&);
	
	const int& Value() const;
	const unsigned char& dataValue() const;
	const int64_t& FrequencyData() const;
	
	const bool operator < (const node&);
	const bool operator > (const node&);

	const bool operator <= (const node&);
	const bool operator >= (const node&);

	const bool operator == (const node&);

	node&& Release() const;
	
	// implicit conversion
	operator int() const;

	const int& operator()() const;
	void Print() const;
	
private:
	int64_t _fdata;
	unsigned char _data;
	
};



#ifndef HUFF_TREE
#define HUFF_TREE
	
	#ifndef REQUIRE_H
		#include <vector>
	#endif

struct h_tree
{
	node* _Left;
	node* _Right;
	intmax_t _Freq;
};



struct _TREE {

	// get the encoded bits of data from a Vector 
	static inline const std::vector<BPAIR<unsigned char>>& CodeMap() {
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
	static inline void enforce_unique(std::vector<BPAIR<unsigned char>>&);

	// iterates through the dataset elements in the vector to project a certain section of tree-view
	static inline void schema_Iter(const std::vector<node>&, const double);

	// directly create a huffman encoding table from the std::vector<node> without actually prebuilt the tree
	static inline const bool create_encoding(const size_t&, const size_t&, int64_t&, const std::vector<node>&);

	static std::vector<BPAIR<unsigned char>> _vPair;
};

std::vector<BPAIR<unsigned char>> _TREE::_vPair = {};


// Extracts nodes information from the vector
inline static void NPRINT(const std::vector<node>& _vn)
{
	for (const node& _e : _vn)
	{
		RPRINT(_e.dataValue()); RPRINT("->"); RPRINT(_e.FrequencyData());
		RET;
	}
}

#endif


#ifndef MX_HUFF_IMPLS

/* Filters priority queue nodes and compute the frequency of each node */
inline static void filter_pq_nodes(std::vector<node>&, std::priority_queue<node>&);

#include "mixhuff_impls.h"

#endif


	



