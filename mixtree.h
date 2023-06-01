#pragma once
#if !defined(TREE_DIRS)
#define TREE_DIRS

#if !defined(NODE_STORES)
#define NODE_STORES
	#include <map>
#endif

#define P_ASSERT(x) (nullptr != x) 
#define LEN(x) std::strlen(x)

#define NOD_TYPE(x) (x->_dir)
#define ISNULL(x) (nullptr == x) 

#define EMPTY_TEXT(x) ( LEN(x) == 0)
#define NULL_LEFT(x) ( ISNULL(x->links[NOD_DIR::LEFT]) )

#define NULL_RIGHT(x) ( ISNULL(x->links[NOD_DIR::RIGHT]) )

#define LEFT_NOD(x) (x->links[NOD_DIR::LEFT])
#define RIGHT_NOD(x) (x->links[NOD_DIR::RIGHT])

#define TEXT(x) (x->Text())
#define CHAR(x) ((unsigned int)x->Text()[0])


#define CHAR_T(x) (  std::isalpha(x->Text()[0])? CHAR(x) : \
					 (unsigned int)std::atoi(x->Text()) \
) 

#define CHAR_S(s) ( std::isalpha(s[0]) ? s[0] : (unsigned int)std::atoi(s) )

#define LEFT_TEXT(x) ( ISNULL(x->links[NOD_DIR::LEFT])? "empty" : TEXT(LEFT_NOD(x)) )
#define RIGHT_TEXT(x) ( ISNULL(x->links[NOD_DIR::RIGHT])? "empty"  : TEXT(RIGHT_NOD(x)) )

#define LESS_THAN(x, n) ( CHAR_T(x) < CHAR_T(n) )
#define GREATER_THAN(x, n) ( CHAR_T(x) > CHAR_T(n) )

#define LESS_THAN_C(s1,s2) ( CHAR_S(s1) < CHAR_S(s2) )
#define GREATER_THAN_C(s1,s2) ( CHAR_S(s1) > CHAR_S(s2) )

#define IS_EQUAL(x, n) ( !std::strcmp(x->Text(), n->Text() ) )
#define TEXT_EQUAL(s1, s2) ( !std::strcmp(s1,s2) )

#define SET_PARENT(x, _nod_ ) ( _nod_->setParent(x) )


#define SET_LEFT(x, _nod_) {\
	x->setLeft(_nod_); \
	_nod_->setParent(x);\
	_nod_->_dir = NOD_DIR::LEFT;\
	_nod_->N_DX = _nod_->Parent()->N_DX + 1; \
}


#define SET_RIGHT(x, _nod_) {\
	x->setRight(_nod_); \
	_nod_->setParent(x); \
	_nod_->_dir = NOD_DIR::RIGHT; \
	_nod_->N_DX = _nod_->Parent()->N_DX + 2; \
}


#define NULLP(p) p = nullptr
#define NULL2P(p1,p2) p1 = nullptr; p2 = nullptr;
#define NULL3P(p1,p2,p3) p1 = nullptr; p2= nullptr; p3 = nullptr; 

constexpr const std::size_t CHRSZ() { return sizeof(char); }

#define TEXT_ALLOC(_txt) new char[LEN(_txt)*CHRSZ]

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
constexpr const UINT EVT_SIGL = 0x21ff;



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
		this->N_DX = 0;
	};

	BNode(const char* _nodText) : BNode() {
		this->Set(_nodText);
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
	const char* Text() const {
		return _data.Text();
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

	static BNode* T_ROOT() { return BNode::_topRoot; }
	static BNode* recent() { return BNode::_recentNod; }
	static const unsigned int T_LEFT() { return BNode::LEFT_T; }
	static const unsigned int T_RIGHT() { return BNode::RIGHT_T; }


	// Accessor Set() Methods...
	const int Set(const char* _text) {
		if (P_ASSERT(_text)) _data = _text;

		this->_dir = NOD_DIR::PARENT;

		return P_ASSERT(_text);
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
		printf("\n root's id: %s ", P_ASSERT(this) ? this->Text() : "empty");
		printf("\n left's id: %s ", P_ASSERT(this) ? NULL_LEFT(this) ? "empty" : TEXT(this->Left()) : "empty");
		printf("\n right's id: %s ", P_ASSERT(this) ? NULL_RIGHT(this) ? "empty" : TEXT(this->Right()) : "empty");
		RET;
	}

	static void SET_ROOT(BNode* _uRoot) { 
		_topRoot = _uRoot; 
		_topRoot->_dir = NOD_DIR::PARENT; 
	}


	// Access Manipulation Methods...
	void Add(BNode* _uNod) {
		BNode* _tmpNod = this;
		UINT LT = 0, RT = 0, _BAL = 0;

		if (!P_ASSERT(_uNod)) return;

		if LESS_THAN_C(TEXT(_tmpNod), TEXT(_uNod)) {
			if P_ASSERT(_tmpNod->Right()) { _tmpNod = _tmpNod->Right(); _tmpNod->Add(_uNod); }
			else {
				SET_RIGHT(_tmpNod, _uNod); ++BNode::RIGHT_T; _lstNodRight = _uNod;
				_recentNod = _lstNodRight;
			}
		}
		else
			if P_ASSERT(_tmpNod->Left()) { _tmpNod = _tmpNod->Left(); _tmpNod->Add(_uNod); }
			else {
				SET_LEFT(_tmpNod, _uNod); ++BNode::LEFT_T; _lstNodLeft = _uNod;
				_recentNod = _lstNodLeft;
			}

		LT = BNode::LEFT_T;
		RT = BNode::RIGHT_T;
		_BAL = (LT > RT) ? LT - RT : RT - LT;

		if (_BAL > 1) T_SIGNAL(EVT_SIGL);
		
		NULLP(_tmpNod);
	}



	void Insert(BNode* _uNod) {
		BNode* _lastNod = BNode::_lstNodLeft, * _tmpRoot = BNode::_topRoot,
			* _currNod = this;

		// _currNod refers to the existing node, 
		// _uNod is the new node to be inserted to
		if (!P_ASSERT(_uNod)) return;

		if P_ASSERT(_currNod->Parent())
			// insert new node into the tree nodes...
			// since the last leftmost node is supposed to be the lesser in the branches
			if GREATER_THAN_C(TEXT(_lastNod), TEXT(_uNod)) {
				_lastNod->Add(_uNod);
				NULL3P(_lastNod, _tmpRoot, _currNod);
				return;
			}
			else {
				_tmpRoot->Add(_uNod);
				NULL3P(_lastNod, _tmpRoot, _currNod);
				return;
			}

		// if the node to be inserted on is the topmost root node.
		// insert new node to the root node.
		if LESS_THAN_C(TEXT(_currNod), TEXT(_uNod))
			if P_ASSERT(_uNod->Left()) { _uNod = _uNod->Left(); _uNod->Add(_currNod); }
			else { SET_LEFT(_uNod, _currNod); ++BNode::LEFT_T; BNode::_topRoot = _uNod; }
		else
			if P_ASSERT(_uNod->Right()) { _uNod = _uNod->Right(); _uNod->Add(_currNod); }
			else { SET_RIGHT(_uNod, _currNod); ++BNode::RIGHT_T; BNode::_topRoot = _uNod; }

		NULL2P(_currNod, _tmpRoot);
	}


	void Remove() {
		UINT LT = 0, RT = 0;
		NOD_DIR lDir = this->_dir;

		if (!P_ASSERT(this)) return;
		// copy existing node to temporary node
		BNode* _ndTmp = new BNode(*this);

		// decrease the nodes ordinal value
		if P_ASSERT(_ndTmp->Left()) --_ndTmp->Left()->N_DX;
		if P_ASSERT(_ndTmp->Right()) --_ndTmp->Right()->N_DX;

		LT = BNode::LEFT_T;	
		RT = BNode::RIGHT_T;

		(NOD_DIR::LEFT == lDir) ? ((LT > 0) ? --LT : LT) : ((RT > 0) ? --RT : RT);

		BNode::LEFT_T = LT;
		BNode::RIGHT_T = RT;

		// Eliminate branches
		NULL3P(this->links[NOD_DIR::PARENT], this->links[NOD_DIR::LEFT],
			this->links[NOD_DIR::RIGHT]);

		*this = BNode(); // assign empty BNode

		if (NOD_DIR::PARENT == lDir)
			return;
		else {
			// add leftover branches to the top root
			_topRoot->Add(_ndTmp->Left());
			_topRoot->Add(_ndTmp->Right());
		}

		NULLP(_ndTmp);
	}

private:
	CTEXT _data;
	unsigned int N_DX;

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

	static std::map<UINT, BNode> _ndTable;
	

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
		if (EVT_SIGL == _signal) Normalize();
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

static inline BNode* ALLOC_N(const char* _Str) {
	return new BNode(_Str);
}


void setTopRoot(BNode* _uRoot) { BNode::SET_ROOT(_uRoot); }


BNode* traverseNodes(BNode* _Root, CTEXT const& _Str) {

	if (!P_ASSERT(_Root)) return nullptr;

	for (; nullptr != _Root; ) {

		if TEXT_EQUAL(TEXT(_Root), _Str.Text()) return (_Root);
		else if LESS_THAN_C(TEXT(_Root), _Str.Text()) _Root = _Root->Right();
		else _Root = _Root->Left();

		if (!P_ASSERT(_Root)) continue;

	}

	return (_Root);
}



BNode* treeAdd(BNode* _Root, CTEXT const& _Str) {

	BNode* _tmpRoot = _Root, * _tmpNew = nullptr;

	if (!P_ASSERT(_tmpRoot)) return nullptr;

	for (; nullptr != _tmpRoot; ) {
		if TEXT_EQUAL(TEXT(_tmpRoot), _Str.Text()) break;
		else if LESS_THAN_C(TEXT(_tmpRoot), _Str.Text()) {
			_tmpNew = _tmpRoot;
			_tmpRoot = _tmpRoot->Right();

		}
		else {
			_tmpNew = _tmpRoot;
			_tmpRoot = _tmpRoot->Left();
		}

		if (!P_ASSERT(_tmpRoot)) {
			_tmpRoot = _tmpNew;
			_tmpRoot->Add(ALLOC_N(_Str.Text()));
			NULLP(_tmpRoot);
		}
		else continue;

	}

	return (_tmpNew);
}

#endif

