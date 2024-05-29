/* Using License: GPL v. 3.0 

   This work is partly inspired from the 
   material taken from 'A Practical Introduction to
   Data Structre & Algorithm Analysis 3rd Edition (C++ Version)' 
   (ebook) by Clifford A. Shaffer.

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
template <class T>
struct cElem
{
	cElem() : KeyC(0), DataC(0), _head{ nullptr}, _next{ nullptr } {}

	cElem(T&& _instance) : KeyC(0), DataC(std::forward<T>(_instance))
	{
		_head = nullptr;
		_next = nullptr;
	}

	cElem(const T& _instance) : KeyC(0), DataC(_instance)
	{
		_head = nullptr;
		_next = nullptr;
	}

	~cElem()
	{
		_head = nullptr;
		_next = nullptr;
	}


	// move ctor
	cElem(cElem<T>&& _iElem)
	{
		if (this == &_iElem) return;
		*this = std::move(_iElem);
	}


	// Move assignment
	cElem<T>&& operator= (cElem<T>&& _mElem)
	{
		if (this == &_mElem)
			return std::move(*this);

		this->DataC = std::move(_mElem.DataC);
		this->KeyC = _mElem.KeyC;

		this->_head = std::move(_mElem._head);
		this->_next = std::move(_mElem._next);


		_mElem.~cElem();
		return std::move(*this);
	}


	T DataC;
	LongRange KeyC;

	uniqueP<cElem<T>> _head;
	uniqueP<cElem<T>> _next;

	operator bool() const
	{
		return (DataC() != T());
	}

	const bool Empty() const
	{
		return (DataC() == T());
	}

	
	cElem(const cElem<T>&) = delete;
	const cElem<T>& operator= (const cElem<T>&) = delete;
};



template <class T>
class cHash;



template < class Ty > /* partial template specialization for cHash<T>, 
  where T = cElem<Ty>; Ty depends on user supplied type */

class cHash<cElem<Ty>>
{
public:
	cHash() : _elemPtr(nullptr), _pList{nullptr} {}
	cHash(const std::size_t _sz) : _elemPtr(nullptr), _M(_sz)
	{
		_pList = MK_U_ARRAY<cElem<Ty>>((UINT)_sz);
	}


	const std::size_t size() const { return _M;  }

	// Returns a calculated hash value
	const std::size_t operator()(const LongRange _K) const
	{
		return hash(_K);
	}


	// Returns a recalculated hash by means of linear probing
	const std::size_t operator()(const LongRange _K, const LongRange _c) const {
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
	const Ty get(const LongRange _uKey)
	{
		Ty _DataObj;

		std::size_t nPos = hash(_uKey);
		
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
		const LongRange _key = _rElem.DataC();
		std::size_t nPos = hash(_key);

		if (nPos > _M) --nPos;

		// if the returned slot position hasn't been occupied by other..
		if (!Exist(nPos)) {
			_rElem.KeyC = _key;
			_pList[nPos] = std::move(_rElem);
			return;
		}
		
		// if the slot is not empty..
		if (Exist(nPos))
		{
			nPos = probe(nPos, 1); // searches other possible empty slot..
			if (!Exist(nPos) ) // if any found slot has not been occupied..
			{
				_rElem.KeyC = _key;
				_pList[nPos] = std::move(_rElem);
				return;
			}
			else
			{
				_rElem.KeyC = _key;
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



	// erases all the data elements in the array
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

	

	const std::size_t hash(const LongRange& _Key) const
	{
		return _Key % (LongRange)_M;
	}


	const std::size_t probe(const LongRange& _Key, const LongRange& _ci) const
	{
		LongRange _home = (LongRange)hash(_Key);
		return (_home + _ci) % _M;
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
