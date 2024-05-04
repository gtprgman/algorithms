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
	cElem() : KeyC(0), DataC(0), _Extra(nullptr) {}

	cElem(T&& _instance) : KeyC(0),DataC(std::move(_instance)), _Extra(nullptr)
	{

	}
	
	~cElem()
	{
		_Extra = nullptr;
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
		this->_Extra = std::move(_mElem._Extra);

	
		_mElem.~cElem();
		return std::move(*this);
	}


	T DataC;
	LongRange KeyC;

	UNIQUE_ARRAY<cElem<T>> _Extra;

	const LongRange operator()() const {
		_Counter++;
		return _Counter;
	}

	// returns the i position of the item
	const LongRange i()  {
		return _Counter;
	}

	
	operator bool() const
	{
		return ( DataC() != T());
	}
	
	
	// disabling old-style manner
	cElem(const cElem<T>&) = delete;
	const cElem<T>& operator= (const cElem<T>&) = delete;

private:
	static LongRange _Counter;

};


template < class T >
LongRange cElem<T>::_Counter = 0;





template <class T>
class cHash;



template < class Ty > /* partial template specialization for cHash<T>, 
  where T = cElem<Ty>; Ty depends on user supplied type */

class cHash<cElem<Ty>>
{
public:
	cHash() : _pList{nullptr} {}
	cHash(const std::size_t _sz) :_M(_sz)
	{
		_pList = _Master.create(_sz);
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

	 
	// returns the '_i' element of the internal array
	cElem<Ty>&& elements(const std::size_t _i) const
	{
		return std::move(_pList[_i] );
	}


	void operator= (cElem<Ty>&& _rElem)
	{
		const LongRange _key = _rElem();
		std::size_t nPos = hash(_key);

		if (!Exist(nPos)) {
			_rElem.KeyC = _rElem.i();
			_pList[nPos] = std::move(_rElem);
		}
		else
		{
			nPos = probe(nPos, (_rElem.i() / _M) * _rElem.i());
			if (Exist(nPos))
			{
			/*
				

			*/
			}

		}
	}

	
	const bool Exist(const std::size_t _hPos)
	{
		return _pList[_hPos];
	}

private:
	std::size_t _M;
	UNIQUE_ARRAY<cElem<Ty>> _pList;
	unique_array_ptr<cElem<Ty>> _Master;


	const std::size_t hash(const LongRange& _Key) const
	{
		return _Key % (LongRange)_M;
	}


	const std::size_t probe(const LongRange& _Key, const LongRange& _ci) const
	{
		LongRange _home = (LongRange)hash(_Key);
		return (_home + _ci) % _M;
	}

};

#endif
