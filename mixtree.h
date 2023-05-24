#pragma once
#if !defined(TREE_DIRS)
#define TREE_DIRS

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
}


#define SET_RIGHT(x, _nod_) {\
x->setRight(_nod_); \
_nod_->setParent(x); \
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

#endif


#if !defined(PNOD)
#define PNOD

#endif



#if !defined(NDIR)
#define NDIR
enum NOD_DIR { UNKNOWN = -1, PARENT = 0, LEFT = 1, MIDDLE = 2, RIGHT = 3 };
#endif


