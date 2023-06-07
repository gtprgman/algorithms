#pragma once
struct BNode;


#if !defined(TREE_DIRS)
#define TREE_DIRS

#define P_ASSERT(x) (nullptr != x) 

#define HAS_PARENT(x) (P_ASSERT(x->Parent()))

#define HAS_CHILDS(x) ( P_ASSERT(x->Left()) || P_ASSERT(x->Right()) )

#define NOD_TYPE(x) (x->_dir)

#define ISNULL(x) (nullptr == x) 

#define NULL_LEFT(x) ( ISNULL(x->links[NOD_DIR::LEFT]) )

#define NULL_RIGHT(x) ( ISNULL(x->links[NOD_DIR::RIGHT]) )

#define LEFT_NOD(x) (x->links[NOD_DIR::LEFT])

#define RIGHT_NOD(x) (x->links[NOD_DIR::RIGHT])

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

constexpr BNode* BNaN = nullptr;

constexpr const std::size_t CHRSZ() { return sizeof(char); }

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

#define TALL  std::cout << "L_HEIGHT: " << BNode::T_LEFT() << "\n" ; \
			  std::cout << "R_HEIGHT: " << BNode::T_RIGHT() << "\n" ;\


using UINT = unsigned int;
constexpr const UINT EVT_BAL = 0x21ff;


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



#if defined MIX_NOD

struct BNode {
	BNode() {
		this->links[0] = nullptr;
		this->links[1] = nullptr;
		this->links[2] = nullptr;
		this->_dir = NOD_DIR::UNKNOWN;
		this->_value = -1;
	};

	BNode(int _nodVal) : BNode() {
		this->Set(_nodVal);
	};

	~BNode() {
		this->links[0] = nullptr;
		this->links[1] = nullptr;
		this->links[2] = nullptr;

		BNode::_topRoot = nullptr;
		BNode::_lstNodLeft = nullptr;
		BNode::_lstNodRight = nullptr;
		BNode::_recentNod = nullptr;
	}

	// Accessor Get() Methods...
	const int& Value() const {
		return _value;
	}

	BNode* Left() const {
		return this->links[NOD_DIR::LEFT];
	}

	BNode* Right() const {
		return this->links[NOD_DIR::RIGHT];
	}

	BNode* lastNodLeft() const {
		return BNode::_lstNodLeft;
	}

	BNode* lastNodRight() const {
		return BNode::_lstNodRight;
	}

	BNode* Parent() const {
		return this->links[NOD_DIR::PARENT];
	}

	NOD_DIR const& Dir() const {
		return this->_dir;
	}


	const unsigned int Lefts() {
		BNode* curr = this;
		unsigned int cnt = 0;
		for (; nullptr != curr; cnt++) 
			curr = curr->Left();

		return --cnt;
	}


	const unsigned int Rights() {
		BNode* curr = this;
		unsigned int cnt = 0;
		for (; nullptr != curr; cnt++)
			curr = curr->Right();

		return --cnt;
	}

	
	static BNode* T_ROOT() { return BNode::_topRoot; }
	static BNode* recent() { return BNode::_recentNod; }
	static const unsigned int T_LEFT() { return BNode::LEFT_T; }
	static const unsigned int T_RIGHT() { return BNode::RIGHT_T; }


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


	void Print() {
		printf("\n root's id: %d ", P_ASSERT(this) ? VAL(this) : -1);
		printf("\n left's id: %d ", P_ASSERT(this) ? NULL_LEFT(this) ? -1 : VAL(this->Left()) : -1);
		printf("\n right's id: %d ", P_ASSERT(this) ? NULL_RIGHT(this) ? -1 : VAL(this->Right()) : -1);
		RET;
	}

	static void SET_ROOT(BNode* _uRoot) { 
		BNode::_topRoot = _uRoot; 
		BNode::_topRoot->_dir = NOD_DIR::PARENT; 
	}


	// Access Manipulation Methods...
	void Add(BNode* _uNod) {
		BNode* _tmpNod = this;
		UINT LT = 0, RT = 0, _BAL = 0;

		if (!P_ASSERT(_uNod)) return;

		if (VAL(_tmpNod) < VAL(_uNod)) {
			if P_ASSERT(_tmpNod->Right()) { _tmpNod = _tmpNod->Right(); _tmpNod->Add(_uNod); }
			else {// if right node is null
				SET_RIGHT(_tmpNod, _uNod); ++BNode::RIGHT_T; _lstNodRight = _uNod;
				_recentNod = _lstNodRight;
			}
		}
		else
			if P_ASSERT(_tmpNod->Left()) { _tmpNod = _tmpNod->Left(); _tmpNod->Add(_uNod); }
			else { // if left node is null
				SET_LEFT(_tmpNod, _uNod); ++BNode::LEFT_T; _lstNodLeft = _uNod;
				_recentNod = _lstNodLeft;
			}

		LT = BNode::LEFT_T;
		RT = BNode::RIGHT_T;
		_BAL = (LT > RT) ? LT - RT : RT - LT;

		if (_BAL > 1) T_SIGNAL(EVT_BAL);
		
		NULLP(_tmpNod);
	}




	static BNode* Find(int const uVal) {
		BNode* nodTmp = nullptr, *nRoot = _topRoot;

		if (nullptr == nRoot) {
			std::cout << "can't find the root node.." << "\n\n";
			return nullptr;
		}

		nodTmp = nRoot;

		for (; nullptr != nodTmp; ) {
			if (VAL(nodTmp) == uVal) break;
				else if (VAL(nodTmp) <  uVal) nodTmp = nodTmp->Right();
					else nodTmp = nodTmp->Left();			
		}

		NULLP(nRoot);
		return (nodTmp);
	}




	void Remove() {
		UINT LT = BNode::LEFT_T; UINT RT = BNode::RIGHT_T;
		UINT LN = 0; UINT RN = 0;
		NOD_DIR lDir = this->_dir; BNode* nodLeft = nullptr, 
		*nodRight = nullptr, *nodParent = nullptr;

		if (!P_ASSERT(this)) return;

		// Number of leaves at the left & right of the current node
		LN = this->Lefts();
		RN = this->Rights();

		if (LN < RN)
			RT = RT - RN; // minus RT by a number of leaves at the right side
		else
			LT = LT - LN; // minus LT by a number of leaves at the left side

		BNode::LEFT_T = LT;
		BNode::RIGHT_T = RT;

		if HAS_PARENT(this) {
			nodParent = this->Parent();
			nodLeft = this->Left();
			nodRight = this->Right();
		}
		
		// discard relation to parent node
		this->links[NOD_DIR::PARENT] = BNaN;

		// discard branches' relations
		if (NOD_DIR::RIGHT == lDir) nodParent->links[NOD_DIR::RIGHT] = BNaN;
			else nodParent->links[NOD_DIR::LEFT] = BNaN;

		// add leftover branches to the top root
		nodParent->Add(nodLeft);
		nodParent->Add(nodRight);
		
		NULL3P(nodParent,nodLeft, nodRight);
	}

private:
	int _value;
	
	static BNode* _topRoot;

	static BNode* _lstNodLeft;
	static BNode* _lstNodRight;

	static BNode* _recentNod;

	static unsigned int LEFT_T;
	static unsigned int RIGHT_T;
	static unsigned int BAL_T;

protected:
	NOD_DIR _dir;
	struct BNode* links[3];

	
	void R_TURNS() {

	}


	void L_TURNS() {

	}


	void Normalize() {
		UINT LT = 0, RT = 0, _bal = 0;

		LT = BNode::LEFT_T;
		RT = BNode::RIGHT_T;

		if (LT > RT) // Rotate right
			R_TURNS();
		else
			// rotate left
			L_TURNS();

	}

	void T_SIGNAL(UINT _signal) {
		if (EVT_BAL == _signal) Normalize();
	}
};

// Static members initialization
BNode* BNode::_topRoot = nullptr;
BNode* BNode::_lstNodLeft = nullptr;
BNode* BNode::_lstNodRight = nullptr;
BNode* BNode::_recentNod = nullptr;

unsigned int BNode::LEFT_T = 0;
unsigned int BNode::RIGHT_T = 0;
unsigned int BNode::BAL_T = 0;

#endif




#if defined PNOD

static inline BNode* ALLOC_N(const int _Value) {
	return new BNode(_Value);
}



void setTopRoot(BNode* _uRoot) { BNode::SET_ROOT(_uRoot); }



BNode* traverseNodes(BNode* _Root, int const _Val) {

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
