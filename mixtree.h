#pragma once

struct BNode;
struct AVL;

using UINT = unsigned int;
using PNODE = struct BNode*;


constexpr BNode* BNaN = nullptr;

constexpr const std::size_t CHRSZ() { return sizeof(char); }

static void setTopRoot(BNode** const);

static BNode* const ALLOC_N(const int, const bool);

static BNode* const ALLOC_N(BNode* const, const bool);

static BNode* const seek_nd(const BNode*, int const);

static BNode* const treeAdd(BNode* const, int const);




#if !defined(TREE_DIRS)
#define TREE_DIRS

#define ISNULL(x) (nullptr == x)

#define C_ASSERT(x) (nullptr != x)

#define P_ASSERT(x) ( (nullptr != x) && (x->Value() > 0) )

#define MAX(n1, n2) ( (n1 > n2)? n1 : n2 )

#define MIN(n1, n2) ( (n1 < n2)? n1 : n2 )

#define NULLP(p) p = nullptr

#define NULL2P(p1,p2) p1 = nullptr; p2 = nullptr;

#define NULL3P(p1,p2,p3) p1 = nullptr; p2= nullptr; p3 = nullptr;

#define FREE1M(p) if C_ASSERT(p) free(p);

#define FREE2M(p1,p2) if C_ASSERT(p1) free(p1); \
					  if C_ASSERT(p2) free(p2);

#define FREE3M(p1,p2,p3) if C_ASSERT(p1) free(p1); \
						 if C_ASSERT(p2) free(p2); \
						 if C_ASSERT(p3) free(p3);

#define IS_EMPTY(x) ( -1 == x->Value() )

#define IS_VALID(x) ( x->Value() > 0 )

#define HAS_PARENT(x) (P_ASSERT(x->Parent()))

#define HAS_CHILDS(x) ( P_ASSERT(x->Left()) || P_ASSERT(x->Right()) )

#define NOD_TYPE(x) (x->_dir)

#define NULLIFY(x) ( x = BNaN )

#define NULL_LEFT(x) ( ISNULL(x->links[NOD_DIR::LEFT]) )

#define NULL_RIGHT(x) ( ISNULL(x->links[NOD_DIR::RIGHT]) )

#define NULL_PARENT(x) ( ISNULL(x->links[NOD_DIR::PARENT]) )

#define LEFT_ND(x) (x->Left())

#define RIGHT_ND(x) (x->Right())

#define PARENT_ND(x) ( x->Parent() )

#define VAL(x) (x->Value())

#define CHAR(x) ((unsigned char)x->Value())

#define LEFT_VAL(x) ( ISNULL(x->links[NOD_DIR::LEFT])? -1 : VAL(LEFT_NOD(x)) )

#define RIGHT_VAL(x) ( ISNULL(x->links[NOD_DIR::RIGHT])? -1  : VAL(RIGHT_NOD(x)) )

#define LESS_THAN(x, n) ( VAL(x) < VAL(n) )

#define GREATER_THAN(x, n) ( VAL(x) > VAL(n) )

#define IS_EQUAL(x, n) ( VAL(x) == VAL(n) )

#define SET_PARENT(x, _nod_ ) ( _nod_->setParent(x) )

#define LCOUNT(x) (x->LCount())
#define RCOUNT(x) (x->RCount())


#define SET_LEFT(x, _nod_) {\
	x->setLeft(_nod_); \
	_nod_->setParent(x);\
	_nod_->setDir(NOD_DIR::LEFT);\
}



#define SET_RIGHT(x, _nod_) {\
	x->setRight(_nod_); \
	_nod_->setParent(x); \
	_nod_->setDir(NOD_DIR::RIGHT); \
}



#define ADD(x, _nod) { \
 if LESS_THAN(x, _nod) {\
	if P_ASSERT(x->Right()) SET_RIGHT(x->Right(),_nod) \
		else SET_RIGHT(x, _nod);\
 }\
 else if P_ASSERT(x->Left()) SET_LEFT(x->Left(),_nod) \
	else SET_LEFT(x,_nod); \
}


#define TEXT_ALLOC(_txt) new char[std::strlen(_txt)*CHRSZ]


// debugging macros
#define RET std::cout << "\n\n"
#define L_HEIGHT(x) std::cout << "L_HEIGHT: " << x->LCount() << "\n";
#define R_HEIGHT(x) std::cout << "R_HEIGHT: " << x->RCount() << "\n";

#define LEFT_TAIL(x) std::cout << "L_TAIL: " << "\n"; \
			x->leftEnd()->Print();

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


#endif  // end of #TREE_DIRS



#if !defined(MIX_CTEXT)
#define MIX_CTEXT

#endif



#if !defined(MIX_NOD)
#define MIX_NOD
// BNode definition
#endif


#if !defined(PNOD)
#define PNOD
// External functions for manipulating BNode
#endif



#if !defined(NDIR)
#define NDIR
enum NOD_DIR { UNKNOWN = -1, PARENT = 0, LEFT = 1, RIGHT = 2 };

#endif



#if !defined(AVL_X_H)
#define AVL_X_H
	#include "avlx.h"
#endif



#if defined MIX_CTEXT
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

private:
	std::string _k;
};
#endif  // end of #MIX_CTEXT



#if defined (MIX_NOD)


struct BNode {
	friend void setTopRoot(BNode** const);
	friend BNode* const ALLOC_N(BNode* const, const bool);

	BNode() {
		this->_value = -1;
		this->_dir = NOD_DIR::UNKNOWN;
		this->links[0] = BNaN;
		this->links[1] = BNaN;
		this->links[2] = BNaN;
	};


	BNode(int _nodVal) {
		this->_value = _nodVal;
		this->_dir = NOD_DIR::UNKNOWN;
		this->links[0] = BNaN;
		this->links[1] = BNaN;
		this->links[2] = BNaN;

	};


	// Copy Ctor for BNode
	BNode(const BNode& refNod) {
		if (this == &refNod) return;
		//std::cout << "BNode's Copy Ctor Called.." << "\n\n";
		*this = refNod;
	}


	// copy assignment for BNode
	const BNode& operator= (const BNode& refNod) {
		if (this == &refNod) return *this;

		this->_value = refNod._value;
		this->_dir = refNod._dir;

		this->links[NOD_DIR::LEFT] = refNod.links[NOD_DIR::LEFT];
		this->links[NOD_DIR::RIGHT] = refNod.links[NOD_DIR::RIGHT];
		this->links[NOD_DIR::PARENT] = refNod.links[NOD_DIR::PARENT];

		return *this;
	}



	//  Move Ctor for BNode
	BNode(BNode&& rBNode) {
		if (this == &rBNode) return;
		//std::cout << "BNode's Move Ctor Called.." << "\n\n";
		*this = std::move(rBNode);
	}



	// assignment for move ctor..
	BNode&& operator= (BNode&& rBNode) {
		if (this == &rBNode) return std::move(*this);

		this->_value = rBNode._value;
		this->_dir = rBNode._dir;

		this->links[NOD_DIR::LEFT] = rBNode.Left();
		this->links[NOD_DIR::RIGHT] = rBNode.Right();
		this->links[NOD_DIR::PARENT] = rBNode.Parent();

		rBNode.links[NOD_DIR::LEFT] = nullptr;
		rBNode.links[NOD_DIR::RIGHT] = nullptr;
		rBNode.links[NOD_DIR::PARENT] = nullptr;

		rBNode.~BNode();

		return std::move(*this);
	}


	~BNode() {
		this->_value = -1;
		this->_dir = NOD_DIR::UNKNOWN;
		this->links[0] = nullptr;
		this->links[1] = nullptr;
		this->links[2] = nullptr;
	}

	// Accessor Get() Methods...
	int const Value() const {
		return this->_value;
	}

	BNode* const Left() const {
		return this->links[NOD_DIR::LEFT];
	}

	BNode* const Right() const {
		return this->links[NOD_DIR::RIGHT];
	}

	BNode* const Parent() const {
		return this->links[NOD_DIR::PARENT];
	}

	const UINT LCount() const {
		return this->Lefts();
	}

	UINT const RCount() const {
		return this->Rights();
	}

	BNode* const leftEnd() const {
		return this->lastLeft();
	}

	BNode* const rightEnd() const {
		return this->lastRight();
	}

	const NOD_DIR Dir() const {
		return this->_dir;
	}


	static const BNode* const T_ROOT() { return ISNULL(BNode::_topRoot) ? BNaN : BNode::_topRoot; }
	static const BNode* const recent() { return ISNULL(BNode::_recentNod) ? BNaN : BNode::_recentNod; }


	// Accessor Set() Methods...
	void Set(int _uVal) {
		this->_value = _uVal;
	}

	void setLeft(BNode* const uNod) {
		// uNod is a new allocated space in memory
		// So free old memory space
		if P_ASSERT(this->links[NOD_DIR::LEFT])
			delete(this->links[NOD_DIR::LEFT]);

		this->links[NOD_DIR::LEFT] = uNod;

		if P_ASSERT(this->links[NOD_DIR::LEFT])
			this->links[NOD_DIR::LEFT]->_dir = NOD_DIR::LEFT;
	}

	void setRight(BNode* const uNod) {
		// uNod is a new allocated space in memory
		if P_ASSERT(this->links[NOD_DIR::RIGHT])
			delete(this->links[NOD_DIR::RIGHT]);

		this->links[NOD_DIR::RIGHT] = uNod;

		if P_ASSERT(this->links[NOD_DIR::RIGHT])
			this->links[NOD_DIR::RIGHT]->_dir = NOD_DIR::RIGHT;
	}


	void setParent(BNode* const uNod) {
		// uNod is a new allocated space in memory
		if P_ASSERT(this->links[NOD_DIR::PARENT])
			delete(this->links[NOD_DIR::PARENT]);

		this->links[NOD_DIR::PARENT] = uNod;

		if P_ASSERT(this->links[NOD_DIR::PARENT])
			this->links[NOD_DIR::PARENT]->_dir = NOD_DIR::PARENT;
	}



	void Print() const {
		printf("\n root's id: %d ", P_ASSERT(this) ? VAL(this) : -1);
		printf("\n left's id: %d ", P_ASSERT(this) ? (P_ASSERT(this->Left()) ? VAL(this->Left()) : -1) : -1);
		printf("\n right's id: %d ", P_ASSERT(this) ? (P_ASSERT(this->Right()) ? VAL(this->Right()) : -1) : -1);
		RET;
	}



	// Access Manipulation Methods...

	void Add(BNode* _uNod) {
		BNode* _tmpNod = (BNode* const)this;

		if (!P_ASSERT(_uNod)) return;
		if (!P_ASSERT(_tmpNod)) return;

		if (VAL(_tmpNod) < VAL(_uNod)) {
			if (P_ASSERT(_tmpNod->Right())) { _tmpNod = _tmpNod->Right(); _tmpNod->Add(_uNod); }
			else {// if right node is null
				SET_RIGHT(_tmpNod, _uNod); BNode::_recentNod = _tmpNod->Right();
			}
		}
		else
			if (P_ASSERT(_tmpNod->Left())) { _tmpNod = _tmpNod->Left(); _tmpNod->Add(_uNod); }
			else { // if left node is null
				SET_LEFT(_tmpNod, _uNod); BNode::_recentNod = _tmpNod->Left();
			} // end if ..

		AVL::reset_data();

		AVL::set_LT_Count(BNode::_topRoot->LCount());
		AVL::set_RT_Count(BNode::_topRoot->RCount());
		AVL::compute_balance();

		if (AVL::balance_value() > 1)
			AVL::make_balance();

		NULL2P(_tmpNod, _uNod);
	}




	BNode* const Remove() {
		NOD_DIR nDir;
		BNode* nodTemp = new BNode();
		BNode* pParent = BNaN;

		if (!P_ASSERT(this)) return (this);

		// IF current node has no parent, then perhaps it is the root node
		if (!P_ASSERT(this->Parent())) return (this);
		// avoid deleting the root node
		/* it best recommends to use any automatic garbage collector like
		   smart pointers to automatically invalidate any held resources by the head
		   pointer.
		*/

		// delete the selected node by transferring its resources to temporary node
		nodTemp = ALLOC_N(this, IS_VALID(this));

		nDir = nodTemp->Dir();

		pParent = (BNode* const)nodTemp->Parent();

		bool isLeft = 0, isRight = 0;

		// Check whether the would be deleted node has childs
		if P_ASSERT(nodTemp->Left()) isLeft = 1;

		if P_ASSERT(nodTemp->Right()) isRight = 1;

		if (isLeft) pParent->Add(nodTemp->Left());
		if (isRight) pParent->Add(nodTemp->Right());

		// free the temporary space in heap
		delete nodTemp;
		NULLP(pParent);

		// return the deleted node, to proof the move mechanism..
		return (this);
	}


	// Find a specified node relative to the current node
	BNode* const Find(int const uVal) {
		BNode* _pCurr = (BNode* const)this;

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

	static BNode* _topRoot;
	static BNode* _recentNod;
	static BNode** _outRoot;


	// leaves count on the left of the current node
	const unsigned int Lefts() const {
		BNode* curr = !P_ASSERT(this) ? BNaN : (BNode* const)this;

		if (!P_ASSERT(curr)) return 0;

		unsigned int cnt = 0;
		for (; nullptr != curr; cnt++)
			if P_ASSERT(curr->Left()) curr = curr->Left(); else break;

		NULLP(curr);
		return cnt;
	}


	// leaves count on the right of the current node
	const unsigned int Rights() const {
		BNode* curr = !P_ASSERT(this) ? BNaN : (BNode* const)this;

		if (!P_ASSERT(curr)) return 0;

		unsigned int cnt = 0;
		for (; nullptr != curr; cnt++)
			if P_ASSERT(curr->Right()) curr = curr->Right(); else break;

		NULLP(curr);
		return cnt;
	}


	/* return the last node of the left
		of the current node */
	BNode* const lastLeft() const {
		BNode* curr = nullptr, * bckup = nullptr;
		curr = !P_ASSERT(this) ? BNaN : (BNode* const)this;

		if (!P_ASSERT(curr)) return BNaN;

		do {
			bckup = curr;
			if P_ASSERT(curr->Left()) curr = curr->Left(); else break;
		} while (nullptr != curr);


		NULLP(curr);
		return (bckup);
	}



	/* return the last node of the right of
		the current node */
	BNode* const lastRight() const {
		BNode* curr = nullptr, * bckup = nullptr;
		curr = !P_ASSERT(this) ? BNaN : (BNode* const)this;

		if (!P_ASSERT(curr)) return BNaN;

		do {
			bckup = curr;
			if P_ASSERT(curr->Right()) curr = curr->Right(); else break;
		} while (nullptr != curr);


		NULLP(curr);
		return (bckup);
	}


protected:
	NOD_DIR _dir;
	struct BNode* links[3];

	void setDir(const NOD_DIR xDir) {
		((BNode* const)this)->_dir = xDir;
	}

	// release current selected node & transfer resources to the respected node.
	const BNode Release() {
		BNode&& bNod = std::move(*this);
		return std::remove_all_extents_t<BNode&&>(bNod);
	}

}; // end of BNode definition..


// Static BNode's members initialization
BNode** BNode::_outRoot = nullptr;
BNode* BNode::_topRoot = nullptr;
BNode* BNode::_recentNod = nullptr;


#endif  // end of #MIX_NOD




#if defined(PNOD)

static inline void setTopRoot(BNode** const outPtr) {
	// ppThis = &nRoot;
	if (BNode::_outRoot == nullptr) BNode::_outRoot = outPtr;

	// nRoot != newRoot?; nRoot = newRoot;
	if (*BNode::_outRoot != *outPtr)  *BNode::_outRoot = *outPtr;
	BNode::_topRoot = *BNode::_outRoot;

}



static inline BNode* const ALLOC_N(const int _Value, const bool nd_valid = true) {
	return ((nd_valid) ? new BNode(_Value) : nullptr);
}


static inline BNode* const ALLOC_N(BNode* const uNod, const bool nd_valid = true) {
	int val_ = VAL(uNod);
	BNode* tmpNew = nullptr;
	if ((nd_valid) && (val_ > 0))
		tmpNew = new BNode(uNod->Release());

	return (tmpNew);
}


static BNode* const seek_nd(const BNode* _Root, int const _Val) {

	if (!P_ASSERT(_Root)) return nullptr;

	for (; P_ASSERT(_Root); ) {

		if (VAL(_Root) == _Val) return (BNode* const)(_Root);
		else if (VAL(_Root) < _Val) _Root = _Root->Right();
		else _Root = _Root->Left();

	}

	return (BNode*)(_Root);
}



static BNode* const treeAdd(BNode* const _Root, int const _Val) {
	BNode* _tmpRoot = _Root, * _tmpNew = nullptr;

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
			_tmpRoot->Add(ALLOC_N(_Val));
			NULLP(_tmpRoot);
		}
		else continue;
	}

	return (_tmpNew);
}



// AVL Implementations
void AVL::R_TURNS() {
	BNode* const leafRoot = (BNode* const)BNode::T_ROOT();
	int rootVal = VAL(leafRoot);
	int leftVal = VAL(leafRoot->Left());

	PNODE leftLeft = ALLOC_N(leafRoot->Left()->Left(), true);

	PNODE leftRight = ALLOC_N(leafRoot->Left()->Right(), true);

	PNODE right = ALLOC_N(leafRoot->Right(), true);

	newRight = ALLOC_N(leftVal, (leftVal > 0) );
	setTopRoot(&newRight);

	newRight->Add(ALLOC_N(rootVal,(rootVal > 0)) );
	newRight->Add(leftLeft);
	newRight->Add(right);
	newRight->Add(leftRight);

	NULL3P(leftLeft, leftRight, right);
}

void AVL::L_TURNS() {
	BNode* const mRoot = (BNode* const)BNode::T_ROOT();
	int rootV = VAL(mRoot);
	int rightV = VAL(mRoot->Right());

	PNODE left = ALLOC_N(mRoot->Left(), true);
	PNODE rightRight = ALLOC_N(mRoot->Right()->Right(), true);
	PNODE rightLeft = ALLOC_N(mRoot->Right()->Left(), true);

	newLeft = ALLOC_N(rightV, (rightV > 0));
	setTopRoot(&newLeft);

	newLeft->Add(ALLOC_N(rootV, (rootV > 0)));
	newLeft->Add(left);
	newLeft->Add(rightRight);
	newLeft->Add(rightLeft);

	NULL3P(left, rightLeft, rightRight);
}



#endif // end of #PNOD
