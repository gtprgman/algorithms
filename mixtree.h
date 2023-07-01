#pragma once

struct BNode;
struct AVL;

using UINT = unsigned int;
using PNODE = struct BNode*;

constexpr BNode* BNaN = nullptr;

static BNode* BRoot = nullptr;

constexpr const UINT EVNT_BALANCE = 0x21ff;

constexpr const std::size_t CHRSZ() { return sizeof(char); }

static BNode* const ALLOC_N(const int, const bool);

static BNode* const ALLOC_N(BNode* const, const bool);

BNode* const seek_nd(const BNode*, int const);

BNode* const treeAdd(BNode*, int const);




#if !defined(TREE_DIRS)
#define TREE_DIRS

#define ISNULL(x) (nullptr == x) 

#define C_ASSERT(x) (nullptr != x)

#define P_ASSERT(x) ( (nullptr != x) && (-1 != x->Value()) ) 

#define MAX(n1, n2) ( (n1 > n2)? n1 : n2 )

#define MIN(n1, n2) ( (n1 < n2)? n1 : n2 )

#define NULLP(p) p = nullptr

#define NULL2P(p1,p2) p1 = nullptr; p2 = nullptr;

#define NULL3P(p1,p2,p3) p1 = nullptr; p2= nullptr; p3 = nullptr;


#define IS_EMPTY(x) ( -1 == x->Value() )

#define IS_VALID(x) ( -1 != x->Value() )

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

#endif



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



// debugging macros
#define RET std::cout << "\n\n"
#define L_HEIGHT(x) std::cout << "L_HEIGHT: " << x->LCount() << "\n";
#define R_HEIGHT(x) std::cout << "R_HEIGHT: " << x->RCount() << "\n";

#define LEFT_TAIL(x) std::cout << "L_TAIL: " << "\n"; \
			x->leftEnd()->Print();

#define RIGHT_TAIL(x) std::cout << "R_TAIL: " << "\n"; \
			x->rightEnd()->Print();




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
#endif



#if defined (MIX_NOD)


struct BNode {


	BNode() {
		this->_value = -1;
		this->_dir = NOD_DIR::UNKNOWN;

		_topRoot = BNaN;
		_lstNodLeft = BNaN;
		_lstNodRight = BNaN;
		_recentNod = BNaN;
		_backRoot = nullptr;

		this->links[0] = BNaN;
		this->links[1] = BNaN;
		this->links[2] = BNaN;
	};


	BNode(int _nodVal) : BNode() {
		this->Set(_nodVal);
	};


	// Copy Ctor for BNode
	BNode(const BNode& refNod) {
		if (this == &refNod) return;
		//std::cout << "BNode's Copy Ctor Called.." << "\n\n";
		*this = refNod;
	}


	// copy assignment for BNode
	BNode const& operator= (const BNode& refNod) {
		if (this == &refNod) return *this;

		this->_value = refNod.Value();
		this->_dir = refNod.Dir();

		this->links[NOD_DIR::LEFT] = refNod.Left();
		this->links[NOD_DIR::RIGHT] = refNod.Right();
		this->links[NOD_DIR::PARENT] = refNod.Parent();

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

		rBNode._value = -1;
		rBNode._dir = NOD_DIR::UNKNOWN;

		return std::move(*this);
	}


	~BNode() {
		_backRoot = nullptr;
		if C_ASSERT(_lstNodLeft) free(_lstNodLeft);
		if C_ASSERT(_lstNodRight) free(_lstNodRight);
		if C_ASSERT(_recentNod) free(_recentNod);
		if C_ASSERT(_topRoot) free(_topRoot);
		

		for (std::size_t i = 0; i < 3; i++)
			if C_ASSERT(links[i]) free(links[i]);

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

	
	const BNode* const T_ROOT() const { return ISNULL(_topRoot)? BNaN : _topRoot; }
	BNode* recent() const { return ISNULL(_recentNod)? BNaN : _recentNod; }
	const unsigned int T_LEFT() const { return LEFT_T; }
	const unsigned int T_RIGHT() const { return RIGHT_T; }
	

	// Accessor Set() Methods...
	void Set(int _uVal) {
		this->_value = _uVal;
		this->_dir = NOD_DIR::PARENT;

	}

	void setLeft(BNode* uNod) {
		this->links[NOD_DIR::LEFT] = uNod;

		if P_ASSERT(this->links[NOD_DIR::LEFT])
			this->links[NOD_DIR::LEFT]->_dir = NOD_DIR::LEFT;
	}

	void setRight(BNode* uNod) {
		this->links[NOD_DIR::RIGHT] = uNod;

		if P_ASSERT(this->links[NOD_DIR::RIGHT])
			this->links[NOD_DIR::RIGHT]->_dir = NOD_DIR::RIGHT;
	}


	void setParent(BNode* uNod) {
		this->links[NOD_DIR::PARENT] = uNod;

		if P_ASSERT(this->links[NOD_DIR::PARENT])
			this->links[NOD_DIR::PARENT]->_dir = NOD_DIR::PARENT;
	}


	void Print() const {
		printf("\n root's id: %d ", P_ASSERT(this) ? VAL(this) : -1);
		printf("\n left's id: %d ", P_ASSERT(this) ? NULL_LEFT(this) ? -1 : VAL(this->Left()) : -1);
		printf("\n right's id: %d ", P_ASSERT(this) ? NULL_RIGHT(this) ? -1 : VAL(this->Right()) : -1);
		RET;
	}

	

	// Access Manipulation Methods...
	void setTopRoot() {
		BRoot = (BNode* const)this;
		_topRoot = (BNode* const)BRoot;
		_backRoot = (BNode** const)&_topRoot;
	}


	void Add(BNode* _uNod) {
		BNode* _tmpNod = (BNode* const)this;
		
		if ISNULL(_uNod) return;

		if (VAL(_tmpNod) < VAL(_uNod)) {
			if P_ASSERT(_tmpNod->Right()) { _tmpNod = _tmpNod->Right(); _tmpNod->Add(_uNod); }
			else {// if right node is null
				SET_RIGHT(_tmpNod, _uNod); _recentNod = _tmpNod->Right();
			}
		}
		else
			if P_ASSERT(_tmpNod->Left()) { _tmpNod = _tmpNod->Left(); _tmpNod->Add(_uNod); }
			else { // if left node is null
				SET_LEFT(_tmpNod, _uNod); _recentNod = _tmpNod->Left();
			} // end if ..

		NFO.reset_data();

		NFO.set_LT_Count(BRoot->LCount());
		NFO.set_RT_Count(BRoot->RCount());
		NFO.compute_balance();

		if (NFO.balance_value() > 1) {
			NFO.make_balance();
			setNewRoot(NFO.get_new_root());
		}

		NULLP(_tmpNod);
	}




	BNode* const Remove() {
		NOD_DIR nDir;
		BNode* pParent = BNaN, *nodTemp = BNaN;

		if ISNULL(this) return BNaN;

		// move the deleted node to temporary object
		nodTemp = new BNode( std::move(*this) );

		nDir = nodTemp->Dir();

		pParent = nodTemp->Parent();

		if ISNULL(pParent) return this;
		// avoid deleting the root node
		/* it best recommends to use any automatic garbage collector like
		   smart pointers to automatically invalidate any held resources by the head
		   pointer.
		*/

		bool isLeft = 0, isRight = 0;

		// Check whether the would be deleted node has childs
		if P_ASSERT(nodTemp->Left()) isLeft = 1;

		if P_ASSERT(nodTemp->Right()) isRight = 1;

		if (isLeft) pParent->Add(nodTemp->Left());
		if (isRight) pParent->Add(nodTemp->Right());

		NULL2P(pParent,nodTemp);

		// return the deleted node, to proof the move mechanism..
		return this;
	}


	// Find a specified node relative to the current node
	BNode* const Find(int const uVal) {
		BNode* _pCurr = (BNode* const)this;

		if ISNULL(_pCurr) {
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

	BNode* _topRoot;
	BNode* _lstNodLeft;
	BNode* _lstNodRight;
	BNode* _recentNod;
	BNode** _backRoot;
	
	unsigned int LEFT_T;
	unsigned int RIGHT_T;
	unsigned int BAL_T;
	

	// leaves count on the left of the current node
	const unsigned int Lefts() const {
		BNode* curr = ISNULL(this)? BNaN : (BNode* const)this;
	
		if ISNULL(curr) return 0;

		unsigned int cnt = 0;
		for (; nullptr != curr; cnt++)
			if P_ASSERT(curr->Left()) curr = curr->Left(); else break;

		NULLP(curr);
		return cnt;
	}


	// leaves count on the right of the current node
	const unsigned int Rights() const {
		BNode* curr = ISNULL(this)? BNaN : (BNode* const)this;

		if ISNULL(curr) return 0;

		unsigned int cnt = 0;
		for (; nullptr != curr; cnt++)
			if P_ASSERT(curr->Right()) curr = curr->Right(); else break;

		NULLP(curr);
		return cnt;
	}


	/* return the last node of the left
		of the current node */ 
	BNode* const lastLeft() const {
		BNode* curr = nullptr, *bckup = nullptr;
		curr = ISNULL(this)? BNaN : (BNode* const)this;

		if ISNULL(curr) return BNaN;

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
		BNode* curr = nullptr, *bckup = nullptr;
		curr = ISNULL(this)? BNaN : (BNode* const)this;

		if ISNULL(curr) return BNaN;

		do {
			bckup = curr;
			if P_ASSERT(curr->Right()) curr = curr->Right(); else break;
		} while (nullptr != curr);
			
			
		NULLP(curr);
		return (bckup);
	}



private:

	static struct AVL {

		constexpr AVL()
		{
		};

		static UINT const LT_Count() { return LT; }
		static UINT const RT_Count() { return RT; }

		static void set_LT_Count(const UINT nc) { LT = nc; }
		static void set_RT_Count(const UINT nc) { RT = nc; }

		static BNode* const get_new_root() {
			return (pNewRoot);
		}
	
		static void reset_data() {
			LT = 0; RT = 0; BAL = 0;
		}

		static void compute_balance() {
			BAL = (LT > RT)? LT - RT : RT - LT;
		}

		static void make_balance() {
			if (LT > RT) R_TURNS();
			 else L_TURNS();

		}

		static UINT const balance_value() {
			return BAL;
		}

	private:
		static UINT LT; // maximum in the Left Branches
		static UINT RT; // maximum in the Right Branches
		static UINT BAL; // difference between LT & RT
		static BNode* pNewRoot;

		/* rotate left branches to the right of the root. */
		static void R_TURNS() {
			PNODE leafRoot = BRoot;
			int rootVal = VAL(leafRoot);
			int leftVal = VAL(leafRoot->Left());

			PNODE leftLeft = ALLOC_N(leafRoot->Left()->Left(), IS_VALID(leafRoot->Left()->Left()));
			leftLeft->setParent(BNaN);

			PNODE leftRight = ALLOC_N(leafRoot->Left()->Right(),
								IS_VALID(leafRoot->Left()->Right()));

			leftRight->setParent(BNaN);

			PNODE right = ALLOC_N(leafRoot->Right(), IS_VALID(leafRoot->Right()));
			right->setParent(BNaN);

			PNODE newRoot = ALLOC_N(leftVal, (-1 != leftVal));
			newRoot->setTopRoot();

			newRoot->Add(ALLOC_N(rootVal, (-1 != rootVal)));
			newRoot->Add(leftLeft);
			newRoot->Add(leftRight);
			newRoot->Add(right);

			pNewRoot = (BNode* const)newRoot;
		}

		/* rotate right branches to the left of the root. */
		static void L_TURNS() {

		}

	} NFO; // END Of struct AVLX DEfs...




protected:
	NOD_DIR _dir;
	struct BNode* links[3];

	void setDir(const NOD_DIR xDir) {
		((BNode* const)this)->_dir = xDir;
	}

	void setNewRoot(BNode* const u_Root) {
		this->_topRoot = u_Root;
		this->_topRoot->setTopRoot();
	}

}; // end of BNode definition..

// Static AVL's members initialization
UINT BNode::AVL::LT = 0;
UINT BNode::AVL::RT = 0;
UINT BNode::AVL::BAL = 0;
BNode* BNode::AVL::pNewRoot = nullptr;


#endif




#if defined PNOD


static inline BNode* const ALLOC_N(const int _Value, const bool nd_valid = true) {
	return (nd_valid)? new BNode(_Value) : BNaN;
}


static inline BNode* const ALLOC_N(BNode* const uNod, const bool nd_valid = true) {
	return (nd_valid)? new BNode(std::move(*uNod)) : BNaN;
}


BNode* const seek_nd(const BNode* _Root, int const _Val) {

	if (!P_ASSERT(_Root)) return nullptr;

	for (; P_ASSERT(_Root);  ) {

		if (VAL(_Root) == _Val) return (BNode* const)(_Root);
		else if (VAL(_Root) < _Val) _Root = _Root->Right();
		else _Root = _Root->Left();

	}

	return (BNode*)(_Root);
}



BNode* const treeAdd(BNode* _Root, int const _Val) {
	BNode* _tmpRoot = _Root, * _tmpNew = nullptr;

	if ISNULL(_tmpRoot) return nullptr;

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


#endif

