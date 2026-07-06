#pragma once

/* Using GPL v 3.0 License */

/* The header 'mixutil.h' is being designed and prepared to be added any feature functionalities that could add help in reducing
the complexities of any programming keywords or overwhelming statement syntaxis such as the call to std::shared_ptr<T[]> and
std::unique_ptr<T[]>, and many others more .. which features are in the list of pending queue that are waiting to
be implemented in the any time of the future.
*/



#ifndef REQUIRE_H
#define REQUIRE_H
		#include <ctime>
		#include <iostream>
		#include <array>
		#include <vector>
		#include <queue>
		#include <cstdarg>
		#include <thread>
		#include <initializer_list>
		#include <list>
		#include <forward_list>
		/*
			include any functional headers here..
		*/

using UC = unsigned char;
using SIGNLESS = unsigned int;
using LongRange = long long;
using LONGFLOAT = long double;

typedef LongRange LINT;
typedef LONGFLOAT LFLOAT;

template < class T >
struct junk_store
{
	T _value;
	bool _saved;
};

#define RET std::cout << "\n";

template <typename T >
static constexpr void PRINT(const T& _t) { std::cout << _t << "\n"; }

template <typename T >
static constexpr void RPRINT(T const& _t) { std::cout << _t; }

template < typename T >
static constexpr void RPRINTC(T const& _t) { std::cout << _t << ","; }


inline void RET2() {
	RET;
	RET;
}


#define ISNULL(p) ( p == nullptr )

#define NULLP(p) ( p = nullptr )

#define NULL2P(p1,p2) {\
	p1 = nullptr; \
	p2 = nullptr; \
}
#endif



#if !defined(MIX_C)
#define MIX_C
	#define _TYPE(_ty) decltype(_ty)
	#define _BOOLC(_ty) std::bool_constant<_ty>::value
#endif



#if !defined(MIX_TREE)
#define MIX_TREE
	//#include "C:\PROJECTS\MIXUTIL\Libs\mixtree.h"
#endif



template < class Ty >
_NODISCARD static constexpr Ty&& _FORWRD(typename std::remove_reference_t<Ty>& _fwArgs) noexcept {
	return static_cast<Ty&&>(_fwArgs);
}


template < class Ty >
_NODISCARD static constexpr Ty&& _FORWRD(typename std::remove_reference_t<Ty>&& _fwArgs) noexcept {
	static_assert(!std::is_lvalue_reference_v<Ty>, " can't forward object of a type specified by 'Ty' ");
	return static_cast<Ty&&>(_fwArgs);
}


template < class Ty >
_NODISCARD static constexpr std::remove_reference_t<Ty>&& _MOVE(Ty&& _mvArgs) noexcept {
	return static_cast<std::remove_reference_t<Ty>&&>(_mvArgs);
}



template < class _Ty  >
struct _Instantiator;


template < class Ty >
struct _Instantiator {
	
	typedef Ty Result;
	

	template < class... Types >
	static inline constexpr Ty&& _Construct(Types&&... aArgs) {
		static_assert(std::is_constructible<Ty, Types...>(), "could not instantiate from a type specified by 'Ty' ");
		return _FORWRD<Ty&&>(Ty(_FORWRD<Types>(aArgs)...));
	}

};


template < class Ty >
std::unique_ptr<Ty[]> MK_U_ARRAY(const std::ptrdiff_t Nx) {
	return std::make_unique<Ty[]>(Nx);
}


template < class Ty >
std::shared_ptr<Ty[]> MK_S_ARRAY(const std::ptrdiff_t Nx) {
	return std::shared_ptr<Ty[]>(new Ty[Nx]());
}



template < class Ty >
using iList = std::initializer_list<Ty>;



template < class tElem  >
struct iList2 {
	using value_type = tElem;
	using reference_const = const tElem&;
	using pointer = tElem*;
	using pointer_const = const tElem*;
	using iterator = pointer_const;

	constexpr iList2() noexcept :  _mFirst(nullptr), _mLast(nullptr)
	{};

	
	// move ctor
	constexpr iList2( iList2<tElem>&& rList2 ) {
		if (this == &rList2) return;
		*this = _MOVE(rList2);
	}

	// overloaded parameterized ctor, not a copy ctor
	constexpr iList2( const std::initializer_list<tElem>& rList ) noexcept {
		_mFirst = rList.begin();
		_mLast = rList.end();
	}

	// Overloaded parameterized ctor
	constexpr iList2(const std::vector<tElem>& v_list) noexcept {
		const tElem* _begin = v_list.begin()._Ptr, *_end = v_list.end()._Ptr;
		const ptrdiff_t max_size = _end - _begin;
		_mFirst = _begin;
		_mLast = _end;
	};


	template < unsigned int Nx >
	constexpr iList2(const std::array<tElem,Nx>& _arry) {
		_mFirst = _arry.data();
		_mLast = (_arry.data() + Nx);
	};
	
	
	template < unsigned int Nx >
	constexpr iList2(tElem const (&_arr)[Nx]) {
		_mFirst = _arr;
		_mLast = (_arr + Nx);
	}
	
	
	_NODISCARD constexpr iterator begin() const noexcept { return _mFirst;  }

	_NODISCARD constexpr iterator end() const noexcept { return _mLast;  }

	_NODISCARD constexpr std::size_t size() const noexcept {
		return static_cast<std::size_t>(_mLast - _mFirst);
	}
	
	_NODISCARD constexpr tElem _max() const noexcept
	{
		tElem _v = 0, _Min = 0, _Max = 0;
		ptrdiff_t iter_diff_t = 0;

		for (pointer _t = (pointer)_mFirst; _t < (pointer)_mLast; _t++)
		{
			iter_diff_t = _mLast - _t;
			_v = *_t; _Min = (iter_diff_t > 1)? *(_t + 1) : *_t;
			_Max = std::max(_Max, _Min, std::less<tElem>());
		}
		return _Max;
	}


	_NODISCARD constexpr tElem _min() const noexcept
	{
		tElem _y = 0, _min = 0, _max = 0;
		ptrdiff_t yter_diff_t = 0;

		for (pointer _yt = (pointer)_mFirst; _yt < (pointer)_mLast; _yt++)
		{
			yter_diff_t = _mLast - _yt;
			_y = *_yt; _min = (yter_diff_t > 1)? *(_yt + 1) : *_yt;
			_min = std::min(_y, _min, std::less<tElem>());
		}
		return _min;
	}

	// overloaded assignment operator
	constexpr const iList2<tElem>& operator=(const std::initializer_list<tElem>& rList) noexcept {
		_mFirst = rList.begin();
		_mLast = rList.end();
		return *this;
	};

	
	// move assignment
	constexpr const iList2<tElem>& operator= (iList2<tElem>&& rList2) {
		if (this == &rList2) return *this;

		_mFirst = rList2._mFirst;
		_mLast = rList2._mLast;

		rList2._mFirst = nullptr;
		rList2._mLast = nullptr;

		return *this;
	};

	_NODISCARD constexpr const value_type operator[](const unsigned int& _idx) noexcept {
		return *(_mFirst + _idx);
	}

	
	// disable copy & copy assignment
	iList2( iList2<tElem>& ) = delete;
	constexpr iList2( const iList2<tElem>& ) = delete;
	const iList2<tElem>& operator= (iList2<tElem> const&) = delete;
	
private:
	pointer_const _mFirst, _mLast;
};




template < class Ty >
struct UnRef {
	using type = typename std::remove_reference_t<Ty>;
};




template < class Ty, Ty _val >
struct _NUMC {
	using numType = typename std::integral_constant<Ty, _val>;
	using value_type = typename std::integral_constant<Ty, _val>::value_type;

	// type cast operator
	constexpr operator value_type() const {
		return _val;
	}
	// function object operator
	_NODISCARD constexpr value_type operator()() const {
		return _val;
	}
};



template <bool _t, class Ty1, class Ty2 >
struct typeSelect;


template <class Ty1, class Ty2>
struct typeSelect<true, Ty1, Ty2> {
	using type = typename Ty1;
};

template <class Ty1, class Ty2>
struct typeSelect<false,Ty1, Ty2> {
	using type = typename Ty2;
};



template <class T, bool _t = false>
struct type_aspect_if
{
	using type = typename std::remove_pointer_t<T>;
};


template < class Ty >
struct type_aspect_if< Ty, true >
{
	using type = typename Ty;
};



	template < class Ty >
	struct isPrimitive {
	private:
		typedef typename Ty type_;

	public:
		static const bool Result = std::is_fundamental<type_>::value;
	};
	


	struct isNumeric
	{
		template < typename T >
		static const bool eval(const T _v) {
			return std::is_integral_v<decltype(_v)>;
		}

	};

	
	template < class LV >
	struct add_lvalue {
		using type = typename std::remove_extent_t<LV>;
	};



	template < class R >
	struct addRef {
		using refType = typename R&;
	};


	
	template < class R >
	struct removeRef {
		using type = typename std::add_lvalue_reference_t<R>;
	};


	
	template < class P >
	struct addPtr {
		using ptrType = typename std::add_pointer_t<P>;
	};

	
	
	template < class P  >
	struct removePtr;


	
	template < class P >
	struct removePtr<P*> {
		using type = typename std::remove_pointer_t<P*>;
	};



	template < class RR >
	struct add_rvalue {
		using type = typename std::add_rvalue_reference_t<RR>;
	};



	template < class RR >
	struct remove_rvalue {
		using type = typename std::remove_reference_t<RR>;
	};
	

	

	namespace mix {

		namespace data {

			class Bucket {
			public:
				Bucket() :_data("empty"), _msize(sizeof(Bucket)) {};

				Bucket(std::string const& _uStr) :_data(_uStr), _msize(sizeof(Bucket)) {

				};

				// overloaded copy-ctor
				Bucket(Bucket const& rBuck) : _msize(sizeof(rBuck)) {
					if (this == &rBuck) return;
					*this = rBuck;
				}

				~Bucket() {}

				// assignment operator
				const Bucket& operator= (const Bucket& rBuck) {
					if (this == &rBuck) return *this;

					this->_data = rBuck._data;
					this->_msize = sizeof(rBuck);

					return *this;
				}

				void set(std::string const& uStr) { _data = uStr; }

				std::size_t const& size(void) const { return _msize; }
				std::string const& data(void) const { return _data; }


			private:
				std::size_t _msize;
				std::string _data;
			};


		}; // end of data namespace


		struct nullType {
			using value_type = std::nullptr_t;

			nullType() : _nan(nullptr) {};
			nullType(std::nullptr_t const) : _nan(std::nullptr_t()) {}

			_NODISCARD operator value_type() const {
				return _nan;
			}

			_NODISCARD operator nullType() const noexcept {
				return _nan;
			}

			_NODISCARD std::nullptr_t const operator()() const noexcept {
				return _nan;
			}


		private:
			value_type _nan;
		};




		template <class P >
		struct ptrTraits
		{
		private:
			using type = typename std::remove_reference_t<P>;
			using rootType = std::conditional_t<std::is_pointer_v<type>, type*, type>;
		public:
			enum {
				isPointer = _BOOLC(std::is_pointer_v<rootType>),
				isReference = _BOOLC(std::is_reference_v<rootType>)
			};
		};



		template <>
		struct ptrTraits<std::nullptr_t> {
			using type = nullType;
		};


		template < class P >
		struct ptrTraits<P*> {
			using type = typename P*;
			using rootType = typename P;
			enum {
				isPointer = _BOOLC(std::is_pointer_v<type>),
				isConstant = _BOOLC(std::is_const_v<rootType>),
				isArray = _BOOLC(std::is_array_v<type>)
			};
		};



		template < class P, ptrdiff_t Nx >
		struct ptrTraits<P[Nx]> {
			using type = typename P[Nx];
			using rootType = typename P;
			enum {
				isPointer = _BOOLC(std::is_pointer_v<type>),
				isConstant = _BOOLC(std::is_const_v<rootType>),
				isArray = _BOOLC(std::is_array_v<type>), count = Nx,
				size = (unsigned int)(Nx * sizeof(P))
			};
		};



		template < class P >
		struct ptrTraits<P[]> {
			using type = typename P[];
			using rootType = typename P;
			enum {
				isPointer = _BOOLC(std::is_pointer_v<type>),
				isConstant = _BOOLC(std::is_const_v<rootType>),
				isArray = _BOOLC(std::is_array_v<type>)
			};
		};



		template < class P >
		struct ptrTraits<const P*> {
			using type = const typename P*;
			using rootType = const typename P;
			enum {
				isPointer = _BOOLC(std::is_pointer_v<type>),
				isConstant = _BOOLC(std::is_const_v<rootType>),
				isArray = _BOOLC(std::is_array_v<type>)
			};
		};



		template < class P >
		struct ptrTraits<P* const> {
			using type = typename P* const;
			using rootType = typename P;
			enum {
				isPointer = _BOOLC(std::is_pointer_v<type>),
				isConstantPointer = true, isArray = _BOOLC(std::is_array_v<type>)
			};
		};



		template < class P >
		struct ptrTraits<const P* const> {
			using type = const typename P* const;
			using rootType = const typename P;
			enum {
				isPointer = _BOOLC(std::is_pointer_v<type>),
				isConstant = _BOOLC(std::is_const_v<rootType>),
				isConstantPointer = true, isArray = _BOOLC(std::is_array_v<type>)
			};
		};




		template < class ret, class entity >
		struct ptrTraits< ret(_cdecl entity::*)() > {
			using type = ret(_cdecl entity::*)();
			using resultType = typename ret;
			using rootType = typename entity;
			enum { isPointerToMember = _BOOLC(std::is_member_pointer_v<type>) };
		};



		template < class ret, class entity >
		struct ptrTraits < ret(_cdecl entity::*)(...) > {
			using type = ret(_cdecl entity::*)(...);
			using resultType = typename ret;
			using rootType = typename entity;
			enum { isPointerToMember = _BOOLC(std::is_member_function_pointer_v<type>) };
		};


		template< class ret, class entity, class... argsT >
		struct ptrTraits < ret(_cdecl entity::*)(argsT&&...) > {
			using type = ret(_cdecl entity::*)(argsT&&...);
			using resultType = typename ret;
			using rootType = typename entity;
			enum { isPointerToMember = _BOOLC(std::is_member_function_pointer_v<type>) };
		};



		template < class ret >
		struct ptrTraits< ret(_cdecl*)()> {
			using type = ret(_cdecl*)();
			using resultType = typename ret;
			enum { isFunctionPointer = _BOOLC(true) };
		};


		template < class ret >
		struct ptrTraits< ret(_cdecl*)(...) > {
			using type = ret(_cdecl*)(...);
			using resultType = typename ret;
			enum { isFunctionPointer = _BOOLC(true) };
		};



		template < class ret, class... ArgsT >
		struct ptrTraits< ret(_cdecl*)(ArgsT&&...) > {
			using type = ret(_cdecl*)(ArgsT&&...);
			using resultType = typename ret;
			enum { isFunctionPointer = _BOOLC(true) };
		};


		template < class P >
		struct _init_p {

			using value_type = typename std::remove_pointer_t<P>;
			using list_iterator = typename iList<value_type>::iterator;
			using LIST = iList<value_type>;

			static inline void initialize(P _p, LIST const& _ls) {
				for (list_iterator _it = _ls.begin(); _it != _ls.end(); _it++)
					*_p++ = *_it;
			}

		};


		/*
		  this one is credited to: https://www.cppstories.com/2024/unroll-templates-lambdas-and-fold/
		*/
		namespace auto_looper
		{
			template <class Fn, std::size_t... I>
			void loopsn(const Fn& _fn, const std::index_sequence<I...>&)
			{
				(_fn(I), ...);
			}

			template <const size_t N, class Fn >
			void forLoop(const Fn& _fnc)
			{
				loopsn(_fnc, std::make_index_sequence<N>());
			}

		}






		constexpr const bool isRange(int8_t const& _C, std::initializer_list<int8_t const> const& _vals) {
			bool isElem = false;

			for (auto const& _c : _vals) {
				if (_C == _c) {
					isElem = true; break;
				}

			};
			return isElem;
		}




		namespace ptr_type {

			// Deleter Object for UNIQUE_ARRAY<Ty>
			template < class Ty >
			struct unique_array_del;


			template < class Ty >
			struct unique_array_del<Ty*>  // Ty = Ty*
			{
				bool operator()(Ty* _uty) {
					static_assert(sizeof(_uty) > 0, "unknown parameter type for '_uty' ");
					delete[] _uty;
					return(nullptr == _uty);
				}
			};


			//Deleter Object for SHARED_ARRAY<Ty>

			template < class Ty >
			struct shared_array_del {};

			template < class Ty >
			struct shared_array_del<Ty*> {
				bool operator()(Ty* _uty) {
					static_assert(sizeof(_uty) > 0, "invalid parameter type for '_uty' ");
					delete[] _uty;
					return(nullptr == _uty);
				}
			};




			// a single unique pointer type
			template < class Ty, class Del1X = std::default_delete<Ty> >
			using uniqueP = std::unique_ptr<Ty, Del1X>;

			// a single shared pointer type
			template < class Ty >
			using shareP = std::shared_ptr<Ty>;

			// a shared pointer to array
			template <class Ty >
			using SHARED_ARRAY = std::shared_ptr<Ty[]>;

			// a unique pointer to array
			template <class Ty, class DelX = std::default_delete<Ty[]> >
			using UNIQUE_ARRAY = std::unique_ptr<Ty[], decltype(DelX()) >;

			template < class _Ty >
			using S_ARRAY = std::shared_ptr<_Ty[]>;

			template< class _Ty, class _Dx = std::default_delete<_Ty[]> >
			using U_ARRAY = std::unique_ptr<_Ty[], decltype(_Dx()) >;

			// wrapper of std::make_unique<Ty>
			template < class _Ty, class... _Types >
			constexpr mix::ptr_type::uniqueP<_Ty> _MAKE_U(_Types&&... aArgs) {
				return std::make_unique<_Ty>(aArgs...);
			}

			// wrapper of std::make_shared<Ty>
			template < class _Ty, class... _Types >
			constexpr mix::ptr_type::shareP<_Ty> _MAKE_S(_Types&&... aArgs) {
				return std::make_shared<_Ty>(aArgs...);
			}



		} // end of ptr_type namespace




		namespace smart_ptr {

			// a single unique factory

			template < class Ty, class delTy = std::default_delete<Ty>, class... Types >
			constexpr ptr_type::uniqueP<Ty, delTy> up_create(Types&&... _u) {
				return std::make_unique<Ty>((_u)...);
			}

			// a single shared factory with default deleter
			template < class Ty, class... Types >
			constexpr const ptr_type::shareP<Ty> sp_create(Types&&... _a) {
				return std::make_shared<Ty>((_a)...);
			}

			// overloaded of a single shared factory with user custom deleter
			template < class Ty, class TyDel = std::default_delete<Ty>, class... Types >
			constexpr const ptr_type::shareP<Ty> sp_create(int, Types&&... _a) {
				return std::shared_ptr<Ty>(new Ty((_a)...), TyDel());
			}

			// unique array factory
			template < class Ty, class DelX = std::default_delete<Ty[]>, std::size_t SZ = 1 >
			class unique_array_ptr
			{
			public:
				constexpr unique_array_ptr() : _msize(SZ), _mpUnique{ nullptr } {}

				constexpr ptr_type::UNIQUE_ARRAY<Ty, DelX>&& create(std::size_t const SIZE = SZ) {
					_msize = SIZE;
					Ty* _ptr = new Ty[SIZE];
					_mpUnique.reset(_ptr);
					_ptr = nullptr;
					return std::move(_mpUnique);
				}

				constexpr ptr_type::UNIQUE_ARRAY<Ty, DelX>&& initialize(std::initializer_list<Ty> const& ls)
				{
					std::size_t k = 0;
					ptr_type::UNIQUE_ARRAY<Ty, DelX>&& unp = create(_msize);

					for (auto const& v : ls) {
						unp[k++] = v;
						if (k > _msize) break;
					}

					return std::move(unp);
				}

			private:
				std::size_t _msize;
				ptr_type::UNIQUE_ARRAY<Ty, DelX> _mpUnique;
			};


			// shared array factory
			template <class Ty, class DelY = std::default_delete<Ty[]> >
			struct Alloc_Share
			{
				constexpr Alloc_Share() : shr(nullptr) {};

				constexpr Alloc_Share(std::size_t const SZ) : shr(new Ty[SZ](), DelY()) {}

				constexpr Alloc_Share<Ty>&& shared_create(std::size_t const SZ) {
					return std::forward<Alloc_Share<Ty>&&>(Alloc_Share<Ty>(SZ));
				}


				template < std::size_t SZ >
				constexpr const ptr_type::SHARED_ARRAY<Ty>& initialize(std::size_t const init, ...)
				{
					va_list vl;
					va_start(vl, init);

					for (std::size_t i = init; i < SZ; i++)
						shr[i] = va_arg(vl, Ty);

					va_end(vl);

					return shr;
				}

				// overloaded ' Alloc_Share<Ty>::initialize() ' by std::initializer_list<Ty> parameter type.
				constexpr const ptr_type::SHARED_ARRAY<Ty>& initialize(std::initializer_list<Ty> const& lst)
				{
					std::size_t i = 0;

					for (auto const& v : lst)
						shr[i++] = v;

					return shr;
				};

				const ptr_type::SHARED_ARRAY<Ty>& get_shared() { return shr; }

				const ptr_type::SHARED_ARRAY<Ty>& operator()() {
					return get_shared();
				}

			private:
				ptr_type::SHARED_ARRAY<Ty> shr;

			};



		} // end of smart_ptr namespace

		// an iterator for the print function of any primitive types.
		template < class P >
		void FOR_EACH_PRINT(P& begin, P& end) {
			for (; begin != end; begin++)
				std::cout << *begin << ", ";

		};

		// an iterator for the print function of any Buckets' types.
		template <>
		void FOR_EACH_PRINT<mix::data::Bucket*>(mix::data::Bucket*& begin, mix::data::Bucket*& end) {
			for (; begin != end; begin++)
				std::cout << begin->data() << ", ";
		}



		// a printer function for any standard primitive types
		template <class PTR >
		constexpr void smart_print(PTR const& begin, PTR const& end)
		{
			FOR_EACH_PRINT(begin, end);
		}


		// an overloaded printer function for any Buckets' types
		template <mix::data::Bucket*>
		constexpr void smart_print(const mix::data::Bucket*& begin, const mix::data::Bucket*& end)
		{
			FOR_EACH_PRINT(begin, end);
		}



		namespace generic
		{
			// comparer functor for int
			struct numLess
			{
				const bool operator()(const intmax_t& _1st, const intmax_t& _2nd)
				{
					return (_1st < _2nd);
				}
			};

			struct numGreater
			{
				const bool operator() (const int64_t& _a, const int64_t& _b)
				{
					return (_a > _b);
				}
			};

			struct numEqual
			{
				const bool operator() (const int64_t& _x, const int64_t& _y)
				{
					return (_x == _y);
				}
			};


			// comparer functor for any type T
			template < class T >
			struct NLess
			{
				const bool operator()(const T& _1st, const T& _2nd)
				{
					return (_1st < _2nd);
				}
			};


			template < class T >
			struct NGreat
			{
				const bool operator()(const T& _1st, const T& _2nd)
				{
					return (_1st > _2nd);
				}
			};


			template < class T >
			struct NEqual
			{
				const bool operator() (const T& _t1, const T& _t2)
				{
					return (_t1 == _t2);
				}
			};


			template <>
			struct NLess<char>
			{
				const bool operator()(const char& _1st, const char& _2nd)
				{
					return (_1st < _2nd);
				}
			};


			template <>
			struct NGreat<char>
			{
				const bool operator()(const char& _1st, const char& _2nd)
				{
					return (_1st > _2nd);
				}
			};


			template <>
			struct NEqual<char>
			{
				const bool operator() (const char& _c1, const char& _c2)
				{
					return (_c1 == _c2);
				}
			};


			// display the content of any STL-like container
			template <class _STL, class v_type = typename _STL::value_type,
				class _Iter = typename _STL::iterator, class _Pointer = _Iter, class _FnPrint >
			static inline void STL_Print(const _Iter& _Begin, const _Iter& _End, const _FnPrint& _printFn)
			{
				const _Iter _ptr0 = _Begin, _ptr1 = _End;

				if (_ptr0 < _Begin || _ptr1 > _End) return;
				if (_ptr0 > _ptr1) return;

				int _cnt = 0;
				for (_Iter _p = _ptr0; _p < _ptr1; _p++, ++_cnt)
				{
					_printFn((v_type)*_p);
					++_cnt;
					if (_cnt > 79) RET;
				}
			}


			// reverses the contents of any STL-like container
			template < class _Ty, class _STL = _Ty, class _FwdItr = typename _STL::iterator,
				class _BackItr = typename _STL::reverse_iterator, class _iType = typename _FwdItr::value_type >
			static inline void STL_Content_Reverse(const _Ty& _Conty)
			{
				const size_t max_elems = _Conty.size();
				_iType* _pType = new _iType[max_elems];
				size_t _x = 0;

				for (_BackItr& rItr = (_BackItr&)_Conty.rbegin(); rItr != (_BackItr&)_Conty.rend(); rItr++)
					if (_x < max_elems) _pType[_x++] = *rItr;

				_x = 0;

				for (_FwdItr& rFwd = (_FwdItr&)_Conty.begin(); rFwd != (_FwdItr&)_Conty.end(); rFwd++)
					*rFwd = _pType[_x++];


				delete[] _pType;
				_pType = mix::nullType();
			}

			// prints out the content of any buffer
			template <typename _T, class _FnPrint>
			static inline void BUFF_Print(_T* _xBuffer, const std::size_t& _maxBuf, const _FnPrint& _printFn)
			{

				for (std::size_t _zi = 0; _zi < _maxBuf; _zi++)
					_printFn(((_T*)_xBuffer)[_zi]);
			}


			// fast sort algorithm performs on data elements in the Vector
			template < class _Iter, class _Other = typename _Iter::value_type, class _Pred >
			inline void fast_sort(const _Iter& _First, const _Iter& _End,
				_Pred _fCmp, const std::ptrdiff_t _maxElems = 0)
			{
				if (_First._Ptr == nullptr || _End._Ptr == nullptr) return;
				if (_First > _End) return;

				const std::ptrdiff_t _MaxSz = (_maxElems > 0) ? _maxElems : (_End - _First) - 1;

				_Other _vTemp;

				for (std::ptrdiff_t k = 0; k < _MaxSz; k++)
				{
					for (_Iter j = _First; j < _End; j++)
					{
						while ((_End - j) > 1 && _fCmp(*j, *(j + 1)))
						{
							++j;
						}

						while ((_End - j) > 1 && !_fCmp(*j, *(j + 1)))
						{
							_vTemp = *(j + 1);
							*(j + 1) = *j;

							*j = _vTemp;
							++j;
						}
					}
				}
			}


			// Use threading processes to sort each subdivided section of a data set.
			template <class _Iter, class _Pred >
			inline void t_sort(const _Iter& _Begin, const _Iter& _End, const double& _dvRatio,
				_Pred _fCmp, const ptrdiff_t _maxElem = 0)
			{

				if (_Begin._Ptr == nullptr || _End._Ptr == nullptr) return;
				if (_Begin > _End) return;

				const std::ptrdiff_t _maxSz = (_maxElem > 0) ? _maxElem : (_End - _Begin) - 1;

				if (_maxSz < 10) {
					fast_sort(_Begin, _End, _fCmp, _maxSz);
					return;
				}

				const std::ptrdiff_t _dvSz = (std::ptrdiff_t)(_dvRatio * _maxSz);
				const std::ptrdiff_t _nDivs = (std::ptrdiff_t)(_maxSz / _dvSz) - 1;

				_Iter _L = _Begin, _R = _L + (_dvSz - 1);

				mix::ptr_type::U_ARRAY<std::thread> _uT = MK_U_ARRAY<std::thread>(_nDivs);

				for (std::ptrdiff_t _t = 0; _t < _nDivs; _t++)
				{
					_uT[_t] = std::thread{ [_L, _R, _dvSz, &_fCmp]() {
						fast_sort(_L, _R, _fCmp, _dvSz);
					  } };

					_uT[_t].join();
					_L = _R;
					_R = _R + (_dvSz - 1);
					if ((_End - _R) <= 1) goto cleanUp;
				}

			cleanUp:
				fast_sort(_Begin, _End, _fCmp, _maxSz);
				_uT.reset(nullptr);
				_uT.release();
			}


			/*
			 The elements in the '_Source_List' should maintain its random ordering position to produce the desired outcome.
			*/
			template < class _T, class _STL, class _Pred = std::less<_T> >
			auto STL_Min_Value = [](_STL& _Result_List, const _STL& _Source_List, _Pred _fCmp = std::less<_T>() )
				{
					using _Iter = typename _STL::iterator;
					junk_store<_T> _PUSHED = { 0, false };
					_T _yt = 0, _yT = 0, _yMin = 0; ptrdiff_t iter_diff_yt = 0;
					iList2<_T> _internal_;
					const _Iter& _First = (_Iter&)_Source_List.begin(), &_Last = (_Iter&)_Source_List.end();

					_Result_List = {}; _internal_ = {};

					for (_Iter& _It = (_Iter&)_First; _It < (_Iter&)_Last; _It++)
					{
						iter_diff_yt = _Last - _It;
						_yt = *_It; _yT = (iter_diff_yt > 1)? *(_It + 1) : _yt;
						_yMin = std::min(_yt, _yT, _fCmp);

						if (_yMin == _PUSHED._value)
						{
							_PUSHED = { 0, false };
							continue;
						}

						_PUSHED._value = _yMin;
						_PUSHED._saved = true;
						_Result_List.push_back(_PUSHED._value);
					}

					_internal_ = _Result_List;
					/*
						mix::generic::STL_Print<std::vector<_T>>(_Result_List.begin(), _Result_List.end(), RPRINTC<_T>);
						RET;
					*/
					return _internal_._min();
				};



			/*
			 The elements in the 'Source_List' should maintain its random ordering position to produce the desired outcome.
			*/
			template < class _T, class _STL, class _Pred = std::less<_T>>
			auto STL_Max_Value = [](_STL& Result_List, const _STL& Source_List, _Pred _fCmp = std::less<_T>())
				{
					using _Iter = typename _STL::iterator;
					junk_store<_T> _is_pushed = { 0,false };
					_T _vt = 0, _vT = 0, _vMax = 0; ptrdiff_t iter_diff_p = 0;
					iList2<_T> _internal_list;
					const _Iter& _Begin = (_Iter&)Source_List.begin(), & _End = (_Iter&)Source_List.end();

					Result_List = {}; _internal_list = {};

					for (_Iter& _it = (_Iter&)_Begin; _it < (_Iter&)_End; _it++)
					{
						iter_diff_p = _End - _it;
						_vt = *_it; _vT = (iter_diff_p > 1)? *(_it + 1) : _vt;

						_vMax = std::max(_vt, _vT, _fCmp);

						if (_vMax == _is_pushed._value)
						{
							_is_pushed = { 0,false };
							continue;
						}

						_is_pushed._value = _vMax;
						_is_pushed._saved = true;
						Result_List.push_back(_is_pushed._value);
					}
					/*
						mix::generic::STL_Print<std::vector<_T>>(Result_List.begin(), Result_List.end(), RPRINTC<_T>);
						RET;
					*/
					_internal_list = Result_List;
					return _internal_list._max();
				};



			/*
			 acts just like the std::priority_queue<_T, _STL, _Pred >
			*/
			template < class _T, class _STL = std::vector<_T>, class _Pred = std::less<typename _STL::value_type > >
			struct STL_Priority_Queue
			{
				using value_type = typename _STL::value_type;
				using reference = typename _STL::value_type&;
				using pointer = typename _STL::value_type*;
				using const_pointer = typename _STL::iterator::pointer;
				using Iter = typename _STL::iterator;

				static_assert(std::is_same_v<_T, value_type>, "container adapter requires consistent types.");

				STL_Priority_Queue() : _priority_id(nullptr), _priority_master(nullptr), _fCmp{ _Pred() }, _Cont{}
				{
					make_heap(0);
				};

				inline void push(value_type&& _item)
				{
					manage_heap(value_type(_item));
				}

				// confirm the aggregations pushed on the heap
				inline void update_heap()
				{
					push_confirm();
				}

				inline const value_type& top() { return _Cont.front(); }

				// pop off the last element that's mostly fit to the '_Pred' criteria.
				inline const value_type&& pop()
				{
					const size_t max_elems = _Cont.size();
					value_type tmp_v = _Cont[max_elems - 1];
					_Cont.pop_back();
					return value_type(tmp_v);
				}

				inline bool&& empty() const { return bool(_Cont.empty()); }

				inline size_t&& size() const { return _Cont.size(); }

				inline void dispose_off()
				{
					for (_priority_card* _pd = _priority_master; _pd != nullptr; _pd = _pd->_next)
					{
						_garbage_list.emplace_back(_pd);
					}
				}

			private:
				value_type _tmp1, _tmp2;
				_Pred _fCmp;
				_STL _Cont;

				struct _priority_card {
					_priority_card() : _prev(nullptr), _next(nullptr), counter_(0), priority_level(0)
					{

					}

					value_type data_value;
					int counter_, priority_level;

					_priority_card* _prev;
					_priority_card* _next;
				};

				_priority_card* _priority_id, *_priority_master;

				inline void make_heap(value_type&& _x) // called when 'STL_Priority_Queue<_T, _STL, _Pred>' is firstly constructed.
				{
					if (nullptr == _priority_id)
					{
						_priority_id = new _priority_card();
						_priority_id->counter_ = 0;
						_priority_master = _priority_id;
						return;
					}

					_priority_id->_next = new _priority_card();
					_priority_id->_next->data_value = _x;
					_priority_id->_next->_prev = _priority_id;
					_priority_id = _priority_id->_next;
					_priority_id->counter_++;
				}

				inline void manage_heap(value_type&& _item_value)
				{
					value_type _reverse;
					_priority_card* _pc = nullptr;

					if (!_priority_id->counter_)
					{
						_priority_id->data_value = _item_value;
						_priority_id->counter_++;
						return;
					}

					make_heap(value_type(_item_value)); // '_priority_id' updated to the recent linked-list.
					
					// manage priority in heap spaces
					for (_priority_card* _pc = _priority_master; _pc != nullptr; _pc = _pc->_next)
					{
						if (nullptr == _pc->_next) continue;

						_tmp1 = _pc->data_value;
						_tmp2 = (nullptr != _pc->_next)? _pc->_next->data_value : _tmp1;
						_tmp1 = _fCmp(_tmp1, _tmp2)? _tmp1 : _tmp2;

						if (_tmp1 != _pc->data_value)
						{
							_reverse = (nullptr != _pc->_next)? _pc->data_value : 0;
							if (_reverse) _pc->_next->data_value = _reverse;
							_pc->data_value = _tmp1;
						}
					}
				}

			protected:
				std::vector<std::unique_ptr<_priority_card>> _garbage_list;

				inline void push_confirm()
				{
					_priority_card* _pdi = _priority_master;

					while (nullptr != _pdi)
					{
						_Cont.push_back(_pdi->data_value);
						_pdi = _pdi->_next;
					}
				}
			};


			/*
			 Perform binary search on the data elements in the vector, the user must specify
			 a comparer functor that operates on data in the vector and provides a comparison result to the
			 search function plus the data element itself should be convertible to an integer value.
			*/
			template <class _Iter, class _Other = typename _Iter::value_type, class _Pred >
			static inline const bool vector_search(const _Iter& _Begin, const _Iter& _Last,
				const _Other& _LookUp_Value, _Pred _fCompare, _Iter& _foundElem)
			{
				if (_Begin._Ptr == nullptr || _Last._Ptr == nullptr) return false;

				const std::ptrdiff_t _MaxSz = (_Last - _Begin) - 1;
				_Iter L = _Begin, R = _Last, L1 = L, R1 = R;
				std::ptrdiff_t M = 0, nSeek = 0;
				_Other vector_value;
				const _Other& lookup_value = _LookUp_Value;

				M = _MaxSz / 2;
				vector_value = *(L + M);

				do {
					if (_fCompare((_Other)lookup_value, (_Other)vector_value))
					{
						L = L1;
						R = L + M;
					}
					else if (!_fCompare((_Other)lookup_value, (_Other)vector_value)) {
						L = L1 + M;
						R = R1;
					}
					else if ((_Other)lookup_value == (_Other)vector_value)
					{
						break;
					}

					L1 = L;
					R1 = R;
					M = (R - L) / 2;

					vector_value = *(L + M); // new value
					_foundElem = L + M;

					++nSeek;

					if (L < _Begin || R  > _Last) break;
					if (nSeek > _MaxSz) break;

				} while ((_Other)vector_value != (_Other)lookup_value);

				return ((_Other)lookup_value == (_Other)vector_value);
			}
		}; // End of generic namespace
	};







