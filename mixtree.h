#pragma once

struct BNode;

using UINT = unsigned int;

constexpr BNode* BNaN = nullptr;

constexpr const UINT EVNT_BALANCE = 0x21ff;

constexpr const std::size_t CHRSZ() { return sizeof(char); }

struct T_INFO {
	UINT LL = 0;
	UINT LR = 0;

	UINT RL = 0;
	UINT RR = 0;
	BNode* _Lesser = nullptr;
	BNode* _Greater = nullptr;
};


#if !defined(TREE_DIRS)
#define TREE_DIRS

#define ISNULL(x) (nullptr == x) 

#define P_ASSERT(x) (nullptr != x) 

#define HAS_PARENT(x) (P_ASSERT(x->Parent()))

#define HAS_CHILDS(x) ( P_ASSERT(x->Left()) || P_ASSERT(x->Right()) )

#define NOD_TYPE(x) (x->_dir)

#define NULL_LEFT(x) ( ISNULL(x->links[NOD_DIR::LEFT]) )

#define NULL_RIGHT(x) ( ISNULL(x->links[NOD_DIR::RIGHT]) )

#define NULL_PARENT(x) ( ISNULL(x->links[NOD_DIR::PARENT]) )

#define LEFT_NOD(x) (x->links[NOD_DIR::LEFT])

#define RIGHT_NOD(x) (x->links[NOD_DIR::RIGHT])

#define PARENT_NOD(x) ( x->links[NOD_DIR::PARENT] )

#define VAL(x) (x->Value())

#define MAX(n1, n2) ( (n1 > n2)? n1 : n2 )

#define MIN(n1, n2) ( (n1 < n2)? n1 : n2 )

#define CHAR(x) ((unsigned char)x->Value())

#define LEFT_VAL(x) ( ISNULL(x->links[NOD_DIR::LEFT])? -1 : VAL(LEFT_NOD(x)) )

#define RIGHT_VAL(x) ( ISNULL(x->links[NOD_DIR::RIGHT])? -1  : VAL(RIGHT_NOD(x)) )

#define LESS_THAN(x, n) ( VAL(x) < VAL(n) )

#define GREATER_THAN(x, n) ( VAL(x) > VAL(n) )

#define IS_EQUAL(x, n) ( VAL(x) == VAL(n) )

#define SET_PARENT(x, _nod_ ) ( _nod_->setParent(x) )


#define SET_LEFT(x, _nod_) {\
	x->setLeft(_nod_); \
	_nod_->setParent(x);\
	_nod_->_dir = NOD_DIR::LEFT;\
}



#define SET_RIGHT(x, _nod_) {\
	x->setRight(_nod_); \
	_nod_->setParent(x); \
	_nod_->_dir = NOD_DIR::RIGHT; \
}


#define NULLP(p) p = nullptr
#define NULL2P(p1,p2) p1 = nullptr; p2 = nullptr;
#define NULL3P(p1,p2,p3) p1 = nullptr; p2= nullptr; p3 = nullptr; 

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
#define L_HEIGHT(x) std::cout << x->LCount() << "\n\n";
#define R_HEIGHT(x) std::cout << x->RCount() << "\n\n";

#define LEFT_END_INFO(x) x->leftEnd()->Print();
#define RIGHT_END_INFO(x) x->rightEnd()->Print();




#if defined MIX_CTEXT
struct CTEXT {
	CTEXT() :_k("empty") {};

	CTEXT(const char* _st) {
		if (P_ASSERT(_st)) _k.assign(_st);
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
		_topRoot = BNaN;
		
		_lstNodLeft = BNaN;
		_lstNodRight = BNaN;
		_recentNod = BNaN;

		NODINFO = { 0,0,0,0,BNaN,BNaN };

		this->links[0] = nullptr;
		this->links[1] = nullptr;
		this->links[2] = nullptr;
		this->_dir = NOD_DIR::UNKNOWN;
		this->_value = -1;
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

		rBNode.links[NOD_DIR::LEFT] = BNaN;
		rBNode.links[NOD_DIR::RIGHT] = BNaN;
		rBNode.links[NOD_DIR::PARENT] = BNaN;
		rBNode._dir = NOD_DIR::UNKNOWN;
		rBNode._value = -1;

		return std::move(*this);
	}


	~BNode() {
		_topRoot = BNaN;
		
		_lstNodLeft = BNaN;
		_lstNodRight = BNaN;
		_recentNod = BNaN;

		NODINFO = { 0,0,0,0,BNaN, BNaN};

		this->links[0] = nullptr;
		this->links[1] = nullptr;
		this->links[2] = nullptr;

		this->_value = -1;
		this->_dir = NOD_DIR::UNKNOWN;
	}

	// Accessor Get() Methods...
	int Value() const {
		return this->_value;
	}

	BNode* Left() const {
		return this->links[NOD_DIR::LEFT];
	}

	BNode* Right() const {
		return
			this->links[NOD_DIR::RIGHT];
	}

	BNode* Parent() const {
		return
			this->links[NOD_DIR::PARENT];
	}

	const UINT LCount() const {
		return this->Lefts();
	}

	UINT const RCount() const {
		return this->Rights(); 
	}

	BNode* leftEnd() const {
		return this->lastLeft();
	}

	BNode* rightEnd() const {
		return this->lastRight();
	}
		
	NOD_DIR Dir() const {
		return this->_dir;
	}

	BNode* leftTop() const {
		return this->topLeft();
	}

	BNode* rightTop() const {
		return this->topRight();
	}

	T_INFO const& tree_info() const {
		return this->NODINFO;
	}

	BNode* T_ROOT() const { return ISNULL(_topRoot)? BNaN : _topRoot; }
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
		this->links[NOD_DIR::LEFT]->_dir = NOD_DIR::LEFT;
	}

	void setRight(BNode* uNod) {
		this->links[NOD_DIR::RIGHT] = uNod;
		this->links[NOD_DIR::RIGHT]->_dir = NOD_DIR::RIGHT;
	}

	void setParent(BNode* uNod) {
		this->links[NOD_DIR::PARENT] = uNod;
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
		_topRoot = this;
	}


	void Add(BNode* _uNod) {
		BNode* _tmpNod = this, 
		*nodLeft = nullptr, *nodRight = nullptr;

		UINT LL = 0, RR = 0, DIFF_T = 0;

		if (!P_ASSERT(_uNod)) return;

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

		
		nodLeft = topLeft();
		nodRight = topRight();

		NODINFO.LL = ISNULL(nodLeft)? 0 : nodLeft->LCount();
		NODINFO.LR = ISNULL(nodLeft)? 0 : nodLeft->RCount();

		LL = MAX(NODINFO.LL, NODINFO.LR);

		NODINFO.RL = ISNULL(nodRight)? 0 : nodRight->LCount();
		NODINFO.RR = ISNULL(nodRight)? 0 : nodRight->RCount();

		RR = MAX(NODINFO.RL, NODINFO.RR);
		
		LEFT_T = LL;
		RIGHT_T = RR;

		DIFF_T = (LL > RR)? LL - RR : RR - LL;

		if (DIFF_T > 1 )
			T_SIGNAL(EVNT_BALANCE); 

		
		NULL3P(_tmpNod,nodLeft,nodRight);
	}




	BNode* Remove() {
		BNode* nodParent = nullptr, * newLeft = nullptr, * newRight = nullptr;
		BNode nodCurr, nodLeft, nodRight;
		NOD_DIR nDir;

		// copy the deleted node to temporary object
		nodCurr = *this; 

		if (-1 == nodCurr.Value()) return this;

		nodParent = nodCurr.Parent();

		if (!(P_ASSERT(nodParent))) return this;
		// avoid deleting the root node
		/* it best recommends to use any automatic garbage collector like
		   smart pointers to automatically invalidate any held resources by the head
		   pointer.
		*/

		// move the deleted node to temporary object
		nodCurr = std::move(*this);

		if P_ASSERT(nodCurr.Left()) nodLeft = std::move(*nodCurr.Left());
		if P_ASSERT(nodCurr.Right()) nodRight = std::move(*nodCurr.Right());

		nDir = nodCurr.Dir();

		// discard relation to parent node
		nodCurr.links[NOD_DIR::PARENT] = BNaN;

		// discard branches' relations
		if (NOD_DIR::RIGHT == nDir) nodParent->links[NOD_DIR::RIGHT] = BNaN;
			else nodParent->links[NOD_DIR::LEFT] = BNaN;

		// allocates spaces for new nodes
		newLeft = new BNode(std::move(nodLeft));
		newRight = new BNode(std::move(nodRight));

		// add leftover branches to parent
		nodParent->Add(newLeft);
		nodParent->Add(newRight);

		// Clear deleted BNode's resources..
		nodCurr = std::move(BNode()); 

		NULL3P(nodParent,newLeft,newRight);

		// return the deleted node, to proof the move mechanism..
		return this;
	}


	BNode* Find(int const uVal) {
		BNode* nodTmp = nullptr, *_pRoot = _topRoot;

		if (nullptr == _pRoot) {
			std::cout << "Can't locate the root node.." << "\n\n";
			return nullptr;
		}

		nodTmp = _pRoot;

		for (; nullptr != nodTmp; ) {
			if (VAL(nodTmp) == uVal) break;
			else if (VAL(nodTmp) < uVal) nodTmp = nodTmp->Right();
			else nodTmp = nodTmp->Left();
		}

		NULLP(_pRoot);
		return (nodTmp);
	}



private:
	int _value;
	BNode* backUp = BNaN;
	BNode* _topRoot;
	
	BNode* _lstNodLeft;
	BNode* _lstNodRight;

	BNode* _recentNod;

	unsigned int LEFT_T;
	unsigned int RIGHT_T;
	unsigned int BAL_T;
	T_INFO NODINFO;
	

	const unsigned int Lefts() const {
		BNode* curr = nullptr; 
		curr = ISNULL(_topRoot)? BNaN : topLeft();

		if ISNULL(curr) return 0;

		unsigned int cnt = 0;
		for (; nullptr != curr; cnt++)
			curr = curr->Left();

		NULLP(curr);
		return cnt;
	}


	const unsigned int Rights() const {
		BNode* curr = nullptr;
		curr = ISNULL(_topRoot) ? BNaN : topRight();

		if ISNULL(curr) return 0;

		unsigned int cnt = 0;
		for (; nullptr != curr; cnt++)
			curr = curr->Right();

		NULLP(curr);
		return cnt;
	}


	BNode* lastLeft() const {
		BNode* curr = nullptr, *bckup = nullptr;
		curr = ISNULL(_topRoot)? BNaN : topLeft();

		if ISNULL(curr) return BNaN;

		bckup = curr;

		for (; nullptr != curr; )
			if P_ASSERT(curr) curr = curr->Left();

		// to avoid returning an invalid pointer
		if ISNULL(curr) curr = bckup; 
		NULLP(bckup);

		return (curr);
	}



	BNode* lastRight() const {
		BNode* curr = nullptr, *bckup = nullptr;
		curr = ISNULL(_topRoot)? BNaN : topRight();

		if ISNULL(curr) return BNaN;

		bckup = curr;

		for (; nullptr != curr; )
			if P_ASSERT(curr) curr = curr->Right();
			
		if ISNULL(curr) curr = bckup;
		NULLP(bckup);

		return (curr);
	}


	BNode* topLeft() const {
		return ISNULL(_topRoot)? BNaN : _topRoot->Left();
	}

	BNode* topRight() const {
		return ISNULL(_topRoot)? BNaN : _topRoot->Right();
	}


protected:
	NOD_DIR _dir;
	struct BNode* links[3];

	// rotate left branches to the right of the root.
	void R_TURNS(BNode* refLeaf) {
		BNode nodLeast;
		BNode* nodTmp = nullptr , *nodRight = nullptr;

		// A BNode's object contains relation to its left & right leaflet.
		nodLeast = *refLeaf; 

		refLeaf->Remove();

		/* Recall that the Remove() method adjusts the associated child
		   leaves of the deleted node. The following is to avoid the duplicated
		   leaves on the branches.. */
		nodLeast.links[NOD_DIR::PARENT] = BNaN;
		nodLeast.links[NOD_DIR::LEFT] = BNaN;
		nodLeast.links[NOD_DIR::RIGHT] = BNaN;

		// move the copied leaflet into heap spaces.
		nodTmp = new BNode(std::move(nodLeast));

		// Add lesser to the right branches.
		nodRight = topRight();
		if P_ASSERT(nodRight) nodRight->Add(nodTmp);
	

		NULL2P(nodTmp,nodRight);
	}


	// rotate right branches to the left of the root.
	void L_TURNS(BNode* refLeaf) {
		BNode nodGreat;
		BNode* nodTmp = nullptr, *nodLeft = nullptr;

		nodGreat = *refLeaf;

		refLeaf->Remove();

		nodGreat.links[NOD_DIR::PARENT] = BNaN;
		nodGreat.links[NOD_DIR::LEFT] = BNaN;
		nodGreat.links[NOD_DIR::RIGHT] = BNaN;

		nodTmp = new BNode(std::move(nodGreat));

		// Add greater to the left branches.
		nodLeft = topLeft();
		if P_ASSERT(nodLeft) nodLeft->Add(nodTmp);

		NULL2P(nodTmp, nodLeft);
	}


	void Normalize() {
		UINT L0, R0, L1, R1; 
		BNode* pLest = nullptr, *pGreat = nullptr,
			*nodLeft = nullptr, *nodRight = nullptr;
		
		nodLeft = _topRoot->Left();
		nodRight = _topRoot->Right();

		L0 = NODINFO.LL; // leaves count on the leftmost node
		R0 = NODINFO.LR; // leaves count on the right of the leftmost node

		L1 = NODINFO.RL; // leaves count on the left of the rightmost node
		R1 = NODINFO.RR; // leaves count on the rightmost node

		pLest = (L0 > R0)? (ISNULL(nodLeft) ? BNaN : nodLeft->leftEnd()) :
				   ISNULL(nodLeft) ? BNaN : nodLeft->rightEnd();

		pGreat = (L1 > R1)? (ISNULL(nodRight) ? BNaN : nodRight->leftEnd()) :
				    ISNULL(nodRight) ? BNaN : nodRight->rightEnd();

		NODINFO._Lesser = pLest;
		NODINFO._Greater = pGreat;

		if P_ASSERT(pLest) R_TURNS(pLest);
		else if P_ASSERT(pGreat) L_TURNS(pGreat); else;

		NULL2P(pLest, pGreat);
	}


	void T_SIGNAL(UINT _signal) {

		switch (_signal)
		{
		case EVNT_BALANCE: Normalize(); break;

		default: break;
		}

	}


}; // end of BNode definition..


#endif




#if defined PNOD


static inline BNode* ALLOC_N(const int _Value) {
	return new BNode(_Value);
}




BNode* seek_nd(BNode* _Root, int const _Val) {

	if (!P_ASSERT(_Root)) return nullptr;

	for (; nullptr != _Root; ) {

		if (VAL(_Root) == _Val) return (_Root);
		else if (VAL(_Root) < _Val) _Root = _Root->Right();
		else _Root = _Root->Left();

		if (!P_ASSERT(_Root)) continue;

	}
	return (_Root);
}



BNode* treeAdd(BNode* _Root, int const _Val) {

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


#endif
