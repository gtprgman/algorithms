#pragma once


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
	cElem() {}

	cElem(T&& _instance) : DataC(_instance)
	{

	}
	
	T DataC;
	cElem<T>* _head = nullptr;
	cElem<T>* _next = nullptr;

	const LongRange operator()() const {
		_Counter++;
		return _Counter;
	}

	const LongRange i() const {
		return _Counter;
	}

	
	operator bool() const
	{
		return ( DataC() != T());
	}
	
private:
	static LongRange _Counter;
};


template < class T >
LongRange cElem<T>::_Counter = 0;




template < class T >
class cHash
{
public:
	cHash() {};
	cHash(const std::size_t _sz) :_M(_sz), _pList(std::make_unique<cElem<T>[]>(_sz))
	{

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

	const cElem<T>& operator[](const std::size_t _idx) const
	{
		return _pList[_idx];
	}


	void operator= (cElem<T>&& _rElem)
	{
		Add(std::forward<cElem<T>>(_rElem) );
	}

	
	const bool Exist(const std::size_t _hPos)
	{
		return ( _pList[_hPos] );
	}

private:
	std::size_t _M;
	std::unique_ptr<cElem<T>[]> _pList;


	const std::size_t hash(const LongRange& _Key)
	{
		return _Key % (LongRange)_M;
	}

	const std::size_t probe(const LongRange& _Key, const LongRange& _ci)
	{
		LongRange _home = (LongRange)hash(_Key);
		return (_home + _ci) % _M;
	}

	void Add(const cElem<T>& rElem_)
	{
		std::size_t nPos = hash(rElem_());

		if (!Exist(nPos))
			_pList[nPos] = rElem_;
		else
		{
			nPos = probe(nPos, (rElem_.i() / _M) * rElem_.i());
		}
		
	}
};

#endif