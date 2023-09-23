/* Using Licence GPL v 3.0 */
#pragma once

/* T represents the core data stored in the BNode Tree.
   P represents a pointer to BNode<T>.

   NB:
   The parametric instantiation of BNode only to identify the stored data object
   associated with a specific key value, so the argument to the BNode instantiation 
   is always a key value of int that is valid for every constructed BNode Object of 
   whatever type the T may represent.

*/

template <class T>
struct BNode;


template <class T>
struct NodeTraits;

template < class T >
struct NodeTraits< BNode<T> > {
	using memberType = T;
	
};

template < typename P = void*  >
struct NodeTraits {
	using p_type = typename P;
	using v_type = std::remove_pointer_t<P>;
	using uType = std::unique_ptr<v_type>;
	using iType = typename NodeTraits<v_type>::memberType;

};





template <typename P>
struct AVL;

using UINT = unsigned int;

template <class T>
using PNODE = struct BNode<T>*;


constexpr const std::size_t CHRSZ() { return sizeof(char); }


// evaluate the deleted status of a node.
template <class T, typename P >
constexpr bool DELETED(P const );

// collect a specified node to the garbage list.
template <class T, typename P >
constexpr void COLLECT(P );

template < class T >
constexpr BNode<T>* const ALLOC_N(int const, const bool);

template <class T, typename P >
constexpr P const ALLOC_N(P const, const bool);

// seek a specified node relative with the root node.
template <class T, typename P >
constexpr P seek_nd(P , int const);

// add a new node object to the tree branches which root is P.
template <class T , typename P>
constexpr P treeAdd(P const, int const);




#ifndef NDIR
#define NDIR
	enum class TDIR: std::int8_t { UNKNOWN = -1, PARENT = 0, LEFT = 1, RIGHT = 2 };
	enum class PDIR: std::int8_t { TOP, RECENT };

	constexpr std::int8_t UNKNOWN = static_cast<std::int8_t>(TDIR::UNKNOWN);
	constexpr std::int8_t LEFT = static_cast<std::int8_t>(TDIR::LEFT);
	constexpr std::int8_t RIGHT = static_cast<std::int8_t>(TDIR::RIGHT);
	constexpr std::int8_t PARENT = static_cast<std::int8_t>(TDIR::PARENT);

	constexpr std::int8_t TOP = static_cast<std::int8_t>(PDIR::TOP);
	constexpr std::int8_t RECENT = static_cast<std::int8_t>(PDIR::RECENT);
	
#endif



#ifndef TREE_DIRS
#define TREE_DIRS

#define ISNULL(x) (nullptr == x)

#define C_ASSERT(x) (nullptr != x)

#define P_ASSERT(x) ( (nullptr != x)? (x->Value() > 0) : 0 )

#define MAX(n1, n2) ( (n1 > n2)? n1 : n2 )

#define MIN(n1, n2) ( (n1 < n2)? n1 : n2 )

#define NULLP(p) p = nullptr

#define NULL2P(p1,p2) p1 = nullptr; p2 = nullptr;

#define NULL3P(p1,p2,p3) p1 = nullptr; p2= nullptr; p3 = nullptr;

#define FREE1M(p) if C_ASSERT(p) delete p;

#define FREE2M(p1,p2) if C_ASSERT(p1) delete p1; \
		if C_ASSERT(p2) delete p2;

#define FREE3M(p1,p2,p3) if C_ASSERT(p1) delete p1; \
			if C_ASSERT(p2) delete p2; \
			if C_ASSERT(p3) delete p3;

#define IS_EMPTY(x) ( -1 == x->Value() )

#define IS_VALID(x) ( x->Value() > 0 )

#define HAS_PARENT(x) (P_ASSERT(x->Parent()))

#define HAS_CHILDS(x) ( P_ASSERT(x->Left()) || P_ASSERT(x->Right()) )

#define NOD_TYPE(x) (x->_dir)

#define NULLIFY(x) ( x = nullptr )

#define NULL_LEFT(x) ( ISNULL(x->links[LEFT]) )

#define NULL_RIGHT(x) ( ISNULL(x->links[RIGHT]) )

#define NULL_PARENT(x) ( ISNULL(x->links[PARENT]) )

#define LEFT_ND(x) (x->Left())

#define RIGHT_ND(x) (x->Right())

#define PARENT_ND(x) ( x->Parent() )

#define VAL(x) (x->Value())

#define CHAR(x) ((unsigned char)x->Value())

#define LEFT_VAL(x) ( ISNULL(x->links[LEFT])? -1 : VAL(LEFT_NOD(x)) )

#define RIGHT_VAL(x) ( ISNULL(x->links[RIGHT])? -1  : VAL(RIGHT_NOD(x)) )

#define LESS_THAN(x, n) ( VAL(x) < VAL(n) )

#define GREATER_THAN(x, n) ( VAL(x) > VAL(n) )

#define IS_EQUAL(x, n) ( VAL(x) == VAL(n) )

#define SET_PARENT(x, _nod_ ) ( _nod_->setParent(x) )

#define LCOUNT(x) (x->LCount())
#define RCOUNT(x) (x->RCount())


#define SET_LEFT(x, _nod_) {\
	x->setLeft(_nod_); \
	_nod_->setParent(x);\
	_nod_->setDir(TDIR::LEFT);\
}



#define SET_RIGHT(x, _nod_) {\
	x->setRight(_nod_); \
	_nod_->setParent(x); \
	_nod_->setDir(TDIR::RIGHT); \
}



#define ADD(x, _nod) { \
 if LESS_THAN(x, _nod) {\
	if P_ASSERT(x->Right()) { \
		BNode* pRight = x->Right(); \
		pRight->Add(_nod);\
		NULLP(pRight);\
	} \
	else x->Add(_nod);\
 }\
 else if P_ASSERT(x->Left()) { \
		BNode* pLeft = x->Left(); \
		pLeft->Add(_nod); \
		NULLP(pLeft);\
 } \
	else x->Add(_nod); \
}


//* Short Cut Functions for Manipulating BNode Objects.. *//
// extract parent information of the specified node's value related to the _root
template < class T >
constexpr void PARENT_OF(BNode<T>* _root, BNode<T>* const _nod) {
	BNode<T>* Nod_ = _root->Find(VAL(_nod));
	if P_ASSERT(Nod_) Nod_->Parent()->Print();
	else
		std::cerr << "The specified node has no parent." << "\n\n";
		
}


// Find a Node with a specified value 'V_' and get a pointer to it.
template <class T>
constexpr BNode<T>* const NODE_T(int V_) {
	return BNode<T>::T_ROOT()->Find(V_);
}


// Delete a specified node's value relative to the root
template < class T >
constexpr void DEL_T(int V_) {
	BNode<T>::T_ROOT()->Find(V_)->Remove();
}


// Store a data object to the specified Node's value
template <class T>
constexpr void SET_T(int const _V, T const _Data) {
	NODE_T<T>(_V)->Set_Data(_Data);
}


// Get a data object from a specified Node's value
template < class T >
constexpr T const DATA_T(int const _V) {
	NODE_T<T>(_V)->Data();
}

#define TEXT_ALLOC(_txt) new char[std::strlen(_txt)*CHRSZ];
#endif // End of #TREE_DIRS defs




#ifndef DEBUGGER_TREE_H
#define DEBUGGER_TREE_H
// debugging macros
#define RET std::cout << "\n\n"
#define PRINT(_Text) std::cout << (_Text) << "\n\n";

// get the height of the left branches
#define L_HEIGHT(x) std::cout << "L_HEIGHT: " << x->LCount() << "\n";

// get the height of the right branches
#define R_HEIGHT(x) std::cout << "R_HEIGHT: " << x->RCount() << "\n";

// extract information of a node at the leftmost end of the branches
#define LEFT_TAIL(x) std::cout << "L_TAIL: " << "\n"; \
			x->leftEnd()->Print();


// extract information of a node at the rightmost end of the branches
#define RIGHT_TAIL(x) std::cout << "R_TAIL: " << "\n"; \
			x->rightEnd()->Print();


#define ROOT_C_ASSERT(x) std::cout << "root ptr: " << C_ASSERT(x) << "\n\n";
#define ROOT_P_ASSERT(x) std::cout << "root ptr: " << P_ASSERT(x) << "\n\n";

#define C_ASSERT_PRINT(x) std::cout << "root ptr: " << C_ASSERT(x) << "\n"; \
			std::cout << "left ptr: " << C_ASSERT(x->Left()) << "\n"; \
			std::cout << "right ptr: " << C_ASSERT(x->Right()) << "\n"; \
			RET;

#define P_ASSERT_PRINT(x) std::cout << "root ptr: " << P_ASSERT(x) << "\n"; \
			std::cout << "left ptr: " << P_ASSERT(x->Left()) << "\n"; \
			std::cout << "right ptr: " << P_ASSERT(x->Right()) << "\n"; \
			RET;

#define V_ASSERT_PRINT(x) std::cout << "root ptr: " << VAL(x) << "\n"; \
			std::cout << "left ptr: " << VAL(x->Left()) << "\n"; \
			std::cout << "right ptr: " << VAL(x->Right()) << "\n"; \
			RET;

#define ROOT_V_ASSERT(x) std::cout << "root value: " << VAL(x) << "\n";
#define LEFT_V_ASSERT(x) std::cout << "left value: " << VAL(x->Left()) << "\n";
#define RIGHT_V_ASSERT(x) std::cout << "right value: " << VAL(x->Right())) << "\n";

#endif // End of #DEBUGGER_TREE_H Defs..




#ifndef MIX_CTEXT
#define MIX_CTEXT

#endif  // end of #MIX_CTEXT defs



#ifndef MIX_NOD
#define MIX_NOD
// BNode definition

#endif // End of #MIX_NOD defs



#ifndef PNOD
#define PNOD
// External functions for manipulating BNode
#endif   // end of #PNOD defs



#ifndef AVL_X_H
#define AVL_X_H
	#include "avl2x.h"

#endif  // end of #AVL_X_H defs



// Garbage Collector Design
#ifndef _DEALLOC_H
#define _DEALLOC_H
	#include <memory>
	#include <vector>
	
	template < typename P >
	struct _Dealloc {
		constexpr void push(typename P const);
		constexpr void dispose_all();

	private:
		std::vector<typename NodeTraits<P>::uType> _unused;
	};


#endif



#ifdef MIX_CTEXT
struct CTEXT {
	CTEXT() :_k("empty") {};

	CTEXT(const char* _st) {
		if (C_ASSERT(_st)) _k.assign(_st);
	}

	int const Value() const {
		return (int)_k[0];
	}

	const char* Text() const {
		return _k.data();
	}

	const char* operator()() const {
		return _k.data();
	}


	// implicit conversion
	operator const char* () const {
		return _k.data();
	}
	
	operator std::string() const {
		return _k.data();
	}
	
	
private:
	std::string _k;
};
#endif  // end of #MIX_CTEXT Impl




#ifdef MIX_NOD


template <class T >
struct BNode {
	friend struct AVL<BNode<T>*>;
	friend constexpr BNode<T>* const ALLOC_N<T>(BNode<T>* const, const bool);
	friend constexpr bool DELETED<T>(BNode<T>* const);
	friend constexpr void COLLECT<T>(BNode<T>* );


	constexpr BNode<T>() {
		this->_value = -1;
		this->_dir = TDIR::UNKNOWN;

		this->links[0] = nullptr;
		this->links[1] = nullptr;
		this->links[2] = nullptr;

		this->_deletion = 0;
	};


	constexpr BNode<T>(int _Key) {
		this->_value = _Key;
		this->_dir = TDIR::UNKNOWN;

		this->links[0] = nullptr;
		this->links[1] = nullptr;
		this->links[2] = nullptr;
		this->_deletion = 0;
	};


	// Copy Ctor for BNode
	constexpr BNode<T>(const BNode<T>& refNod) {
		if (this == &refNod) return;
		//std::cout << "BNode's Copy Ctor Called.." << "\n\n";
		*this = refNod;
	}


	// copy assignment for BNode
	constexpr BNode<T>& operator= (const BNode<T>& refNod) {
		if (this == &refNod) return *this;

		this->_value = refNod._value;
		this->_dir = refNod._dir;
		this->_deletion = refNod._deletion;
		this->nod_data = refNod.nod_data;

		this->links[LEFT] = refNod.links[LEFT];
		this->links[RIGHT] = refNod.links[RIGHT];
		this->links[PARENT] = refNod.links[PARENT];

		return *this;
	}



	//  Move Ctor for BNode
	constexpr BNode<T>(BNode<T>&& rBNode) {
		if (this == &rBNode) return;
		//std::cout << "BNode's Move Ctor Called.." << "\n\n";
		*this = std::move(rBNode);
	}



	// assignment for move ctor..
	constexpr BNode<T>&& operator= (BNode<T>&& rBNode) {
		if (this == &rBNode) return std::move(*this);

		this->_value = rBNode._value;
		this->_dir = rBNode._dir;
		this->_deletion = rBNode._deletion;
		this->nod_data = rBNode.nod_data;

		this->links[LEFT] = rBNode.Left();
		this->links[RIGHT] = rBNode.Right();
		this->links[PARENT] = rBNode.Parent();

		rBNode.links[LEFT] = nullptr;
		rBNode.links[RIGHT] = nullptr;
		rBNode.links[PARENT] = nullptr;
		rBNode._deletion = -1;
		rBNode.nod_data = T();

		rBNode.~BNode();

		return std::move(*this);
	}


	~BNode<T>() {
		this->_value = -1;
		this->_dir = TDIR::UNKNOWN;

		this->links[0] = nullptr;
		this->links[1] = nullptr;
		this->links[2] = nullptr;
		this->_deletion = -1; // this is crucial to prevent delete on the already deleted node.
	}

	// Accessor Get() Methods...
	int constexpr Value() const {
		return this->_value;
	}

	constexpr T Data() const {
		return this->nod_data;
	}


	BNode<T>* const Left() const {
		return this->links[LEFT];
	}

	BNode<T>* const Right() const {
		return this->links[RIGHT];
	}

	BNode<T>* const Parent() const {
		return this->links[PARENT];
	}

	constexpr UINT LCount() const {
		return this->Lefts();
	}

	UINT constexpr RCount() const {
		return this->Rights();
	}

	BNode<T>* const leftEnd() const {
		return this->lastLeft();
	}

	BNode<T>* const rightEnd() const {
		return this->lastRight();
	}

	constexpr TDIR Dir() const {
		return this->_dir;
	}

	static constexpr BNode<T>* const originalRoot() {
		return *AVL<BNode<T>*>::outerRoot;
	}
	
	static constexpr BNode<T>* const T_ROOT() { return ISNULL(linkPtr[TOP])? nullptr : linkPtr[TOP]; }
	static constexpr BNode<T>* const recent() { return ISNULL(linkPtr[RECENT])? nullptr : linkPtr[RECENT]; }
	
	// garbage collector static member functions..
	static void Collect();

	static void Dispose();


	static constexpr void setTopRoot(BNode<T>** const outPtr = nullptr) {
		if (nullptr == AVL<BNode<T>*>::outerRoot)
			AVL<BNode<T>*>::outerRoot = outPtr;

		if (*AVL<BNode<T>*>::outerRoot != *outPtr)
			*AVL<BNode<T>*>::outerRoot = *outPtr;

		BNode<T>::linkPtr[TOP] = (BNode<T>* const)(*AVL<BNode<T>*>::outerRoot);
	}


	// Accessor Set() Methods...
	constexpr void Set(int _uVal = -1) {
		this->_value = _uVal;
	}


	constexpr void Set_Data(T const uData) {
		this->nod_data = uData;
	}


	constexpr void setLeft(BNode<T>* const uNod) {
		// uNod is a new allocated space in memory
		// So free old memory space
		if P_ASSERT(this->links[LEFT])
			delete(this->links[LEFT]);

		this->links[LEFT] = uNod;

		if P_ASSERT(this->links[LEFT])
			this->links[LEFT]->_dir = TDIR::LEFT;
	}

	constexpr void setRight(BNode<T>* const uNod) {
		// uNod is a new allocated space in memory
		if P_ASSERT(this->links[RIGHT])
			delete(this->links[RIGHT]);

		this->links[RIGHT] = uNod;

		if P_ASSERT(this->links[RIGHT])
			this->links[RIGHT]->_dir = TDIR::RIGHT;
	}


	constexpr void setParent(BNode<T>* const uNod) {

		this->links[PARENT] = uNod;

		if P_ASSERT(this->links[PARENT])
			this->links[PARENT]->_dir = TDIR::PARENT;
	}


	constexpr void Print() const {
		printf("\n root's id: %d ", P_ASSERT(this)? VAL(this) : -1);
		printf("\n left's id: %d ", P_ASSERT(this)? (P_ASSERT(this->Left())? VAL(this->Left()) : -1) : -1);
		printf("\n right's id: %d ", P_ASSERT(this)? (P_ASSERT(this->Right())? VAL(this->Right()) : -1) : -1);
		RET;
	}



	// Access Manipulation Methods...

	constexpr void Add(BNode<T>* _uNod ) {
		BNode<T>* _tmpNod = (BNode<T>* const)this;

		if (!P_ASSERT(_uNod)) return;
		if (!P_ASSERT(_tmpNod)) return;
		if (VAL(_uNod) < 0) return; // -1

		if (VAL(_tmpNod) < VAL(_uNod)) {
			if (P_ASSERT(_tmpNod->Right())) { _tmpNod = _tmpNod->Right(); _tmpNod->Add(_uNod); }
			else {// if right node is null
				SET_RIGHT(_tmpNod, _uNod); BNode<T>::linkPtr[RECENT] = _tmpNod->Right();
			}
		}
		else
			if (P_ASSERT(_tmpNod->Left())) { _tmpNod = _tmpNod->Left(); _tmpNod->Add(_uNod); }
			else { // if left node is null
				SET_LEFT(_tmpNod, _uNod); BNode<T>::linkPtr[RECENT] = _tmpNod->Left();
			} // end if ..

		AVL<BNode<T>*>::reset_data();

		AVL<BNode<T>*>::set_LT_Count(linkPtr[TOP]->LCount());
		AVL<BNode<T>*>::set_RT_Count(linkPtr[TOP]->RCount());
		AVL<BNode<T>*>::compute_balance();

		if (AVL<BNode<T>*>::balance_value() > 1)
			AVL<BNode<T>*>::make_balance();

		NULL2P(_tmpNod, _uNod);
	}




	constexpr BNode<T>* const Remove() {
		TDIR nDir;
		BNode<T>* nodTemp = nullptr;

		if (!P_ASSERT(this)) return (this);

		// IF current node has no parent, then perhaps it is the root node
		if (!P_ASSERT(this->Parent())) return (this);
		// avoid deleting the root node
		/* it best recommends to use any automatic garbage collector like
		   smart pointers to automatically invalidate any held resources by the head
		   pointer.
		*/

		// delete the selected node by transferring its resources to temporary node
		nodTemp = ALLOC_N<T>(this, IS_VALID(this));

		nDir = nodTemp->Dir();

		BNode<T>* pParent = (BNode<T>* const)nodTemp->Parent();

		/*
		bool isLeft = 0, isRight = 0;
		if (isLeft) pParent->Add(nodTemp->Left());
		if (isRight) pParent->Add(nodTemp->Right());

		To anticipate recurring flip-flop rotations caused by the Add() method,
		we use the Macro version of ADD.
		*/

		// Check whether the would be deleted node has childs
		if P_ASSERT(nodTemp->Left())
			ADD(pParent, nodTemp->Left());

		if P_ASSERT(nodTemp->Right())
			ADD(pParent, nodTemp->Right());

		// free the unused space in heap
		delete nodTemp;
		NULLP(pParent);
		// return the deleted node, to proof the move mechanism..
		return (this);
	}


	// Find a specified node relative to the current node
	constexpr BNode<T>* const Find(int const uVal) {
		BNode<T>* _pCurr = (BNode<T>* const)this;

		if (!P_ASSERT(_pCurr)) {
			std::cout << "Can't locate the root node.." << "\n\n";
			return nullptr;
		}

		do {
			if (VAL(_pCurr) == uVal) break;
			else if (VAL(_pCurr) < uVal) _pCurr = _pCurr->Right();
			else _pCurr = _pCurr->Left();

		} while (nullptr != _pCurr);

		return (_pCurr);
	}



private:
	int _value;
	typename T nod_data;

	static BNode<T>* linkPtr[2];
	
	// leaves count on the left of the current node
	constexpr unsigned int Lefts() const {
		BNode<T>* curr = !P_ASSERT(this)? nullptr : (BNode<T>* const)this;

		if (!P_ASSERT(curr)) return 0;

		unsigned int cnt = 0;
		for (; nullptr != curr; cnt++)
			if P_ASSERT(curr->Left()) curr = curr->Left(); else break;

		NULLP(curr);
		return cnt;
	}


	// leaves count on the right of the current node
	constexpr unsigned int Rights() const {
		BNode<T>* curr = !P_ASSERT(this)? nullptr : (BNode<T>* const)this;

		if (!P_ASSERT(curr)) return 0;

		unsigned int cnt = 0;
		for (; nullptr != curr; cnt++)
			if P_ASSERT(curr->Right()) curr = curr->Right(); else break;

		NULLP(curr);
		return cnt;
	}


	/* return the last node of the left
		of the current node */
	constexpr BNode<T>* const lastLeft() const {
		BNode<T>* curr = nullptr, *bckup = nullptr;
		curr = !P_ASSERT(this)? nullptr : (BNode<T>* const)this;

		if (!P_ASSERT(curr)) return nullptr;

		do {
			bckup = curr;
			if P_ASSERT(curr->Left()) curr = curr->Left(); else break;
		} while (nullptr != curr);


		NULLP(curr);
		return (bckup);
	}



	/* return the last node of the right of
		the current node */
	constexpr BNode<T>* const lastRight() const {
		BNode<T>* curr = nullptr, *bckup = nullptr;
		curr = !P_ASSERT(this)? nullptr : (BNode<T>* const)this;

		if (!P_ASSERT(curr)) return nullptr;

		do {
			bckup = curr;
			if P_ASSERT(curr->Right()) curr = curr->Right(); else break;
		} while (nullptr != curr);


		NULLP(curr);
		return (bckup);
	}



protected:
	int _deletion;
	TDIR _dir;
	struct BNode<T>* links[3];


	constexpr void setDir(const TDIR xDir) {
		((BNode<T>* const)this)->_dir = xDir;
	}

	// release current selected node & transfer resources to the respected node.
	constexpr BNode<T> Release() {
		BNode<T>&& bNod = std::move(*this);
		this->_deletion = -1;
		return std::remove_all_extents_t<BNode<T>&&>(bNod);
	}


	// garbage collector member functions impl..
	constexpr void collectLeft() {
		BNode<T>* _curr = (BNode<T>* const)this;

		while (P_ASSERT(_curr)) {
			if P_ASSERT(_curr->Right()) COLLECT<T>(_curr->Right());
			if P_ASSERT(_curr->Left()) COLLECT<T>(_curr->Left());
			_curr = _curr->Left();
		};

		NULLP(_curr);
	}


	constexpr void collectRight() {
		BNode<T>* _curr = (BNode<T>* const)this;

		while (P_ASSERT(_curr)) {
			if P_ASSERT(_curr->Right()) COLLECT<T>(_curr->Right());
			if P_ASSERT(_curr->Left()) COLLECT<T>(_curr->Left());
			_curr = _curr->Right();
		};

		NULLP(_curr);
	}

}; // end of BNode definition..

//BNode<T>'s members initialization..

template <class T>
BNode<T>* BNode<T>::linkPtr[] = { nullptr,nullptr };


#endif  // end of #MIX_NOD Impl



// Garbage Collector's Implementations
#ifdef _DEALLOC_H

template <typename P>
constexpr void _Dealloc<P>::push(typename P const _up) {
	_unused.emplace_back(_up);
}


template <typename P >
constexpr void _Dealloc<P>::dispose_all() {
		if (_unused.empty()) return;
		for (auto vt = _unused.begin(); vt != _unused.end(); vt++)
			vt->reset(nullptr);
}



template <typename P  >
_Dealloc<P> _Deleter;

#endif  // End of #_DEALLOC_H Impl




#ifdef PNOD


template < class T >
constexpr BNode<T>* const ALLOC_N(int const Val_, const bool nd_stat = true) {
	return ((nd_stat) ? new BNode<T>(Val_) : nullptr);
}


template <class T, typename P = BNode<T>* >
constexpr P const ALLOC_N(P const _uNod, const bool nd_stat = true) {
	int val_ = VAL(_uNod);
	P tmpNew = nullptr;

	if ((nd_stat) && (val_ > 0))
		tmpNew = new BNode<T>(_uNod->Release());

	// Re-Managing the Node's relations of Parent, Left & New connections
	P nLeft =
		P_ASSERT(tmpNew->Left())? tmpNew->Left() : nullptr;

	P nRight =
		P_ASSERT(tmpNew->Right())? tmpNew->Right() : nullptr;

	if P_ASSERT(nLeft) nLeft->setParent(tmpNew);
	if P_ASSERT(nRight) nRight->setParent(tmpNew);

	NULL2P(nLeft, nRight);
	return (tmpNew);
}



// seek a specified node's value relative to the specified root node.
template < class T, typename P = BNode<T>* >
constexpr P seek_nd(P _Root, int const _Val) {

	if (!P_ASSERT(_Root)) return nullptr;

	for (; P_ASSERT(_Root); ) {

		if (VAL(_Root) == _Val) return (P const)(_Root);
		else if (VAL(_Root) < _Val) _Root = _Root->Right();
		else _Root = _Root->Left();

	}

	return (P const)(_Root);
}



// add a new node object to the tree branches which root is P.
template <class T, typename P = BNode<T>* >
constexpr P treeAdd(P const _Root, int const _Val ) {

	P _tmpRoot = _Root, _tmpNew = nullptr;

	if (!P_ASSERT(_tmpRoot)) return nullptr;

	for (; nullptr != _tmpRoot; ) {
		if (VAL(_tmpRoot) == _Val) break;
		else if (VAL(_tmpRoot) < _Val) {
			_tmpNew = _tmpRoot;
			_tmpRoot = _tmpRoot->Right();

		}
		else {
			_tmpNew = _tmpRoot;
			_tmpRoot = _tmpRoot->Left();
		}

		if (!P_ASSERT(_tmpRoot)) {
			_tmpRoot = _tmpNew;
			_tmpRoot->Add(ALLOC_N<T>(_Val));
			NULLP(_tmpRoot);
		}
		else continue;
	}

	return (_tmpNew);
}



/* AVL Implementations:
	Rotations Algorithms of both R_TURNS() & L_TURNS()
	are inspired from https://www.programiz.com/dsa/avl-tree
*/


template <typename P  >
constexpr inline void AVL<typename P>::R_TURNS() {
	// because ::_outRoot directly point to the outer root
	AVL<P>::pRoot1 = *AVL<P>::outerRoot;
	AVL<P>::newRoot1 = ALLOC_N<AVL<P>::DType>(AVL<P>::pRoot1->Left(), true);
	AVL<P>::newRoot1->links[PARENT] = nullptr;

	AVL<P>::pRoot1->setLeft(ALLOC_N<AVL<P>::DType>(AVL<P>::newRoot1->Right(), true));
	AVL<P>::pRoot1->Left()->setParent(AVL<P>::pRoot1);
	AVL<P>::pRoot1->Left()->setDir(TDIR::LEFT);

	SET_RIGHT(AVL<P>::newRoot1, AVL<P>::pRoot1);
	BNode<AVL<P>::DType>::setTopRoot(&AVL<P>::newRoot1); 

	NULL2P(AVL<P>::pRoot1, AVL<P>::newRoot1);
}



template <typename P>
constexpr inline void AVL<typename P>::L_TURNS() {
	AVL<P>::pRoot2 = *AVL<P>::outerRoot;
	AVL<P>::newRoot2 = ALLOC_N<AVL<P>::DType>(AVL<P>::pRoot2->Right(), true);
	AVL<P>::newRoot2->links[PARENT] = nullptr;

	AVL<P>::pRoot2->setRight(ALLOC_N<AVL<P>::DType>(AVL<P>::newRoot2->Left(), true));
	AVL<P>::pRoot2->Right()->setParent(AVL<P>::pRoot2);
	AVL<P>::pRoot2->Right()->setDir(TDIR::RIGHT);

	SET_LEFT(AVL<P>::newRoot2, AVL<P>::pRoot2);

	BNode<AVL<P>::DType>::setTopRoot(&AVL<P>::newRoot2);
	NULL2P(AVL<P>::pRoot2, AVL<P>::newRoot2);
}



// evaluate the deleted status of a node.
template <class T, typename P = BNode<T>* >
constexpr bool DELETED(P const _p ) {
	return P_ASSERT(_p)? (_p->_deletion < 0) : -1;
}



// collect a specified node to the garbage list.
template <class T, typename P = BNode<T>* >
constexpr void COLLECT(P _p) {

	if (!DELETED<T>(_p)) {
		_p->_deletion = -1;
		_Deleter<P>.push(_p);
	}
}


/* BNode's garbage collector impl.., 
  Collect() & Dispose() are static member methods of BNode<T>. */
template <class T >
void inline BNode<T>::Collect() {
	if ((AVL<BNode<T>*>::LT_Count() < 1) || (AVL<BNode<T>*>::RT_Count() < 1)) return;

	BNode<T>* _gRoot = (BNode<T>* const)BNode<T>::T_ROOT();
	BNode<T>* _gLeft = _gRoot->Left();
	BNode<T>* _gRight = _gRoot->Right();

	// garbage collects on the left sub-branches..
	_gLeft->Left()->collectLeft();
	_gLeft->Left()->collectRight();

	_gLeft->Right()->collectLeft();
	_gLeft->Right()->collectRight();


	// garbage collects on the right sub-branches..
	_gRight->Left()->collectLeft();
	_gRight->Left()->collectRight();

	_gRight->Right()->collectLeft();
	_gRight->Right()->collectRight();

	NULL2P(_gLeft, _gRight);

	// garbage collects on the residual root's branches..
	_gRoot->collectLeft();
	_gRoot->collectRight();

	NULLP(_gRoot);
}



template <class T >
inline void BNode<T>::Dispose() {
	_Deleter<BNode<T>*>.dispose_all();
}

#endif // end of #PNOD Impl
