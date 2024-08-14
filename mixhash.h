/* Using License: GPL v. 3.0 
	
	This work is partly inspired from
	the materials taken from the ebook
	" A Practical Introduction to
	Data Structures & Algorithm Analysis "
	    3rd Edition ( C++ Version )
		        by
		Clifford A. Shaffer
*/

#pragma once

#ifdef REQUIRE_H
	using namespace mix::ptr_type;
	using namespace mix::smart_ptr;

#endif


const std::size_t factorial(const std::size_t);
const std::size_t permute(const std::size_t);


#ifndef MX_HASH
#define MX_HASH
 

inline const UINT factorial(const UINT n)
{
	if (n <= 1) return 1;
	return n * factorial(n - 1);
}



inline const std::size_t permute(const std::size_t _TMinusOne)
{

}



// data element for cHash
template <class T,class keyType = unsigned int>
struct cElem
{
	// AType could be a T or T*
	using AType = typename type_aspect_if<T, mix::ptrTraits<T>::isPointer>::type;

	// The T is made sure to be of its original type
	using CType = typename type_aspect_if<T, false>::type;

	// The T is determined to be of as what it was
	using PType = typename mix::ptrTraits<T>::type;

	// the type of key use for hash
	using KeyType = typename keyType;
	
	
	cElem() : KeyC(0), DataC(AType()), _head{ nullptr}, _next{ nullptr }
	{}

	cElem(AType&& _instance) : KeyC(0)
	{
		if (mix::ptrTraits<AType>::isPointer)
			DataC = _instance;
		else
			DataC = std::forward<AType>(_instance);

		_head = nullptr;
		_next = nullptr;
	}

	cElem(const AType& _instance) : KeyC(0)
	{
		DataC = _instance;
		_head = nullptr;
		_next = nullptr;
	}

	~cElem()
	{
		_head = nullptr;
		_next = nullptr;
	}


	// move ctor
	cElem(cElem<AType>&& _iElem)
	{
		if (this == &_iElem) return;
		*this = std::move(_iElem);
	}


	// Move assignment
	cElem<AType>&& operator= (cElem<AType>&& _mElem)
	{
		if (this == &_mElem)
			return std::move(*this);

		this->DataC = std::move(_mElem.DataC);
		this->KeyC = _mElem.KeyC;

		this->_head = std::move(_mElem._head);
		this->_next = std::move(_mElem._next);

		if (std::is_pointer_v<AType>) _mElem.DataC = nullptr;
		else _mElem.DataC = AType();

		_mElem.~cElem();
		return std::move(*this);
	}

	
	
	AType DataC; // DataC could be a pointer or an instance of any std type
	KeyType KeyC;

	uniqueP<cElem<AType>> _head;
	uniqueP<cElem<AType>> _next;

	operator bool() const
	{
		if (mix::ptrTraits<AType>::isPointer)
			return ( DataC != nullptr );
		
		return ( DataC != AType() );
	}

	const bool Empty() const
	{
		if (mix::ptrTraits<AType>::isPointer) return (DataC == nullptr);
		return ( DataC() == AType());
	}

	
	// Get the intrinsic data value of Ty
	const CType Value() const
	{
		return (CType)DataC;
	}


	// disable copy ctor & copy assignment
	cElem(const cElem<AType>&) = delete;
	const cElem<AType>& operator= (const cElem<AType>&) = delete;
};




template <class T>
class cHash;


template < class Ty > /* partial template specialization for cHash<T>, 
  where T = cElem<Ty,keyType>; Ty depends on user supplied type */
class cHash<cElem<Ty>>
{
public:

	using KeyT = typename cElem<Ty>::KeyType;


	cHash() : isPtrC(0), _elemPtr(nullptr), _pList{nullptr} {}
	cHash(const std::size_t _sz) : isPtrC(0), _elemPtr(nullptr), _M(_sz)
	{
		_pList = MK_U_ARRAY<cElem<Ty>>((UINT)_sz);
		isPtrC = mix::ptrTraits<Ty>::isPointer;
	}


	const std::size_t size() const { return _M;  }


	// Returns a calculated hash value
	const KeyT operator()(const KeyT _K) const
	{
		return hash(_K);
	}


	// Returns a recalculated hash by means of linear probing
	const KeyT operator()(const KeyT _K, const KeyT _c) const {
		return probe(_K, _c);
	}

	 
	// get the element at '_i' position of the internal array
	const cElem<Ty>& elements(const std::size_t _i) const
	{
		return _pList[_i];
	}


	/* get the data element of a specific key '_uKey',
	   the key specified as '_uKey' must be the core data object/value
	   not the index or reference to the data object.
	*/
	const Ty get(const KeyT _uKey)
	{
		Ty _DataObj = (mix::ptrTraits<Ty>::isPointer)? nullptr : Ty();

		KeyT nPos = hash(_uKey); // invoke hash method
		
		if (elements(nPos).KeyC != _uKey)
		{
			nPos = probe(nPos, 1);
			_elemPtr = &_pList[nPos];

			while (_elemPtr != nullptr)
			{
				if (_elemPtr->KeyC == _uKey)
				{
					_DataObj = _elemPtr->DataC;
					_elemPtr = nullptr;
				}
				else
				{
					_elemPtr = _elemPtr->_next.get();
				}
			}
		}
		else
		{
			_DataObj = elements(nPos).DataC;
		}
		
		return _DataObj;
	}


	void operator= (cElem<Ty>&& _rElem)
	{
		Ty _key;

		_key = _rElem.DataC;

		KeyT nPos = hash( (KeyT)_key );

		if (nPos > (KeyT)_M) --nPos;

		// if the returned slot position hasn't been occupied by other..
		if (!Exist(nPos)) {
			_rElem.KeyC = (KeyT)_key;
			_pList[nPos] = std::move(_rElem);
			return;
		}
		
		// if the slot is not empty..
		if (Exist(nPos))
		{
			nPos = probe(nPos, 1); // searches other possible empty slot..
			if (!Exist(nPos) ) // if any found slot has not been occupied..
			{
				_rElem.KeyC = (KeyT)_key;
				_pList[nPos] = std::move(_rElem);
				return;
			}
			else
			{
				_rElem.KeyC = (KeyT)_key;
				_elemPtr = (cElem<Ty>*)add_next_links(&_pList[nPos]);

				if ( _elemPtr != nullptr )
				{
					*_elemPtr = std::move(_rElem);
				}
			}
		}
	}

	
	const cElem<Ty>& Exist(const std::size_t _hPos) const
	{
		return _pList[_hPos];
	}



	// Erases all the data elements in the array
	void Sweep() const
	{
		for (std::size_t i = 0; i < _M; i++)
			_pList[i].~cElem();
	}



	
private:
	std::size_t _M;
	
	// a raw pointer to each linked element of _pList
	cElem<Ty>* _elemPtr;

	U_ARRAY<cElem<Ty>> _pList;

	bool isPtrC;

	
	const KeyT hash(const KeyT& _Key) const
	{
		return ( _Key % (KeyT)_M  );
	}


	const KeyT probe(const KeyT& _Key, const KeyT _ci) const
	{
		KeyT _home = hash(_Key);
		return (_home + _ci) % (KeyT)_M;
	}



	/* Add next pointer to the current Linked-List chain. */
	const cElem<Ty>* add_next_links(const cElem<Ty>* _ptr)
	{
		 _elemPtr = (cElem<Ty>*)_ptr;

		if (_elemPtr->_next == nullptr)
		{
			_elemPtr->_next = up_create<cElem<Ty>>();
			return (_elemPtr->_next.get());
		}


		while (_elemPtr->_next != nullptr)
			_elemPtr = _elemPtr->_next.get();

		_elemPtr->_next = up_create<cElem<Ty>>();

		return (_elemPtr->_next.get());
	}

};


#endif

