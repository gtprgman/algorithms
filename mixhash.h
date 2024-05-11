/* Using License: GPL v 3.0 */

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
	cElem() : KeyC(0), DataC(0) {}

	cElem(T&& _instance) : KeyC(0), DataC(std::move(_instance))
	{

	}

	~cElem()
	{
		_head = nullptr;
		_next = nullptr;
	}


	// Copy Ctor
	cElem(const cElem<T>& _rElm)
	{
		if (this == &_rElm) return;
		*this = _rElm;
	}


	// Copy Assignment operator
	const cElem<T>& operator= (const cElem<T>& _rElm)
	{
		if (this == &_rElm) return *this;

		this->KeyC = _rElm.KeyC;
		this->DataC = _rElm.DataC;


		return *this;
	}


	// move ctor
	cElem(cElem<T>&& _iElem)
	{
		if (this == &_iElem) return;
		*this = std::move(_iElem);
	}


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

	 
	// returns the '_i' element of the internal array of cHash object
	cElem<Ty>&& elements(const std::size_t _i) const
	{
		return std::move(_pList[_i] );
	}


	/* get the data element of a specific key '_uKey',
	   the key specified as '_uKey' must be the core data object/value
	   not the index or reference to the data object.
	*/
	const Ty operator[](const LongRange _uKey)
	{
		Ty _DataObj;

		std::size_t nPos = hash(_uKey);
		if( !elements(nPos).Empty() ) nPos = probe(nPos, 1);

		_elemPtr = _pList[nPos]._next.get(); 
		
		while (_elemPtr != nullptr)
		{
			if (_elemPtr->KeyC == _uKey)
			{
				_DataObj = _elemPtr->DataC;
				_elemPtr = nullptr;
			}
		}

		return _DataObj;
	}


	void operator= (cElem<Ty>&& _rElem)
	{
		const LongRange _key = _rElem.DataC();
		std::size_t nPos = hash(_key);
		
		if (!Exist(nPos)) {
			_rElem.KeyC = _rElem.DataC();
			_pList[nPos] = std::move(_rElem);
		}
		else
		{
			nPos = probe(nPos, 1);
			if (Exist(nPos))
			{
				if (_pList[nPos]._next == nullptr)
				{
					_pList[nPos]._next = up_create<cElem<Ty>>();
					_pList[nPos]._next->_head = std::unique_ptr<cElem<Ty>>(&_pList[nPos]);
					*_pList[nPos]._next = _rElem;
				}	
				else
				{   
					int kPos = next_N_Links_of(&_pList[nPos]);
					_elemPtr = &_pList[nPos]; // get the first offset Ptr of _pList

					for (int j = 0; j < kPos; j++)
					{
						_elemPtr = _elemPtr->_next.get();
					}
					_elemPtr->_next = up_create<cElem<NODE_T>>();
					*_elemPtr->_next = _rElem;
				}
			 NULLP(_elemPtr);
			}
		}
	}

	
	const bool Exist(const std::size_t _hPos)
	{
		return _pList[_hPos];
	}

private:
	std::size_t _M;

	// a raw pointer to each linked element of _pList
	cElem<Ty>* _elemPtr;

	UNIQUE_ARRAY<cElem<Ty>> _pList;


	const std::size_t hash(const LongRange& _Key) const
	{
		return _Key % (LongRange)_M;
	}


	const std::size_t probe(const LongRange& _Key, const LongRange& _ci) const
	{
		LongRange _home = (LongRange)hash(_Key);
		return (_home + _ci) % _M;
	}


	const bool subExist(cElem<Ty>&& _eLem)
	{
		return _eLem;
	}


	// Take the N number of successive next Ptrs from the _pList[_Offset].
	const int next_N_Links_of(const cElem<Ty>* _Offset)
	{
		int nSkip = -1;
		_elemPtr = (cElem<Ty>*)_Offset;

		if (_elemPtr == nullptr) return -1;

		do
		{
			_elemPtr = _elemPtr->_next.get();
			++nSkip;
		} while (_elemPtr == nullptr);
			
		return nSkip;
	}
	
};

#endif
