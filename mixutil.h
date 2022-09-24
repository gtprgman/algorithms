#ifndef REQUIRE_H
#define REQUIRE_H
	#include <iostream>
	#include <memory>
	#include <cstdarg>

	#include <initializer_list>
	#include <forward_list>
#endif

/* Common Creative License's Properties */

/* The header 'mixutil.h' is designed and prepared to be added any feature functionalities that could mostly add help in reducing
   the complexities of any programming keywords or overwhelming statement syntaxis such as any call to std::shared_ptr<T[]> and 
   std::unique_ptr<T[]>, and many others more .. which features are in the list of pending queue that are waited to be implemented 
   in the any times of the future. 
 */

template <class Ty>
using SHARED_ARRAY = std::shared_ptr<Ty[]>;

template <class Ty>
using UNIQUE_ARRAY = std::unique_ptr<Ty[]>;


// unique array factory
template < class Ty, std::size_t SZ = 1 >
class unique_array_ptr
{
public:
	constexpr unique_array_ptr() : _msize(SZ), _mpUnique{ nullptr } {}

	constexpr UNIQUE_ARRAY<Ty>&& create(std::size_t const SIZE = SZ) {
		_msize = SIZE;
		Ty* _ptr = new Ty[SIZE];
		_mpUnique.reset(_ptr);
		_ptr = nullptr;
		return std::move(_mpUnique);
	}

	constexpr UNIQUE_ARRAY<Ty>&& initialize(std::initializer_list<Ty> const& ls)
	{
		std::size_t k = 0;
		UNIQUE_ARRAY<Ty>&& unp = create(_msize);

		for (auto const& v : ls) {
			unp[k++] = v;
			if (k > _msize) break;
		}

		return std::move(unp);
	}

private:
	std::size_t _msize;
	UNIQUE_ARRAY<Ty> _mpUnique;
};




template <class PTR, class fnIter>
void FOR_EACH(PTR begin, PTR end, fnIter const& fnt)
{
	for (; begin != end; begin++)
		fnt(*begin);
}




template <class Ty>
struct Alloc_Share
{
	constexpr Alloc_Share() {};
	constexpr Alloc_Share(std::size_t const SZ) : shr(new Ty[SZ]()) {}
	SHARED_ARRAY<Ty>& get_shared() { return shr; }

	SHARED_ARRAY<Ty>& operator()(std::size_t const) {
		return get_shared();
	}

private:
	SHARED_ARRAY<Ty> shr;
};




template <class Ty>
constexpr Alloc_Share<Ty>&& SHARED_CREATE(std::size_t const SZ) {
	return std::move(Alloc_Share<Ty>(SZ));
}



template <typename Ty, std::size_t SZ = 1>
constexpr void SHARED_INIT(SHARED_ARRAY<Ty>& shrP, std::size_t const init, ...)
{
	va_list vl;
	va_start(vl, init);

	for (std::size_t i = init; i < SZ; i++)
		shrP[i] = va_arg(vl, Ty);

	va_end(vl);
}



// overloaded SHARED_INIT with ' const std::initializer_list<Ty>&'  parameter
template <typename Ty>
constexpr void SHARED_INIT(SHARED_ARRAY<Ty>& shrP, std::initializer_list<Ty> const& lst)
{
	std::size_t i = 0; 

	for (auto const& v : lst)
		shrP[i++] = v;

}


template <class PTR>
constexpr void smart_print(PTR const& begin, PTR const& end)
{
	FOR_EACH(begin, end, [](auto const& _dataP)->decltype(void()) {
		std::cout << _dataP << ", ";
	}
	);
}



