#ifndef REQUIRE_H
#define REQUIRE_H
	#include <iostream>
	#include <memory>
	#include <cstdarg>
#endif

/* Common Creative License's Properties */

/* The header 'mixutil.h' is designed and prepared to be added any features functionality that could mostly help in reducing
   the complexities of any programming tasks that would mostly engaged with frequently issuing of overwhelming statement syntaxis
   such as the issuing of std::shared_ptr<T[]> and std::unique_ptr<T[]>, and many others more .. which features are in the pending
   queue of to be implemented in any time of the future. */

template <class Ty>
using SHARED_ARRAY_TYPE = std::shared_ptr<Ty[]>;



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
	constexpr Alloc_Share(std::size_t const SZ) : shr( new Ty[SZ]() ) {}
	SHARED_ARRAY_TYPE<Ty>& get_shared() { return shr; }

	SHARED_ARRAY_TYPE<Ty>& operator()(std::size_t const){
		return get_shared();
	}

private:
	SHARED_ARRAY_TYPE<Ty> shr;
};




template <class Ty>
constexpr Alloc_Share<Ty>&& SHARED_CREATE(std::size_t const SZ) {
	return std::move(Alloc_Share<Ty>(SZ));
}



template <std::size_t SZ = 1, typename Ty>
constexpr void SHARED_INIT(SHARED_ARRAY_TYPE<int>& shrP, std::size_t const init, ...)
{
	va_list vl;
	va_start(vl, init);

	for (std::size_t i = init; i < SZ; i++)
		shrP[i] = va_arg(vl, Ty);

	va_end(vl);
}





