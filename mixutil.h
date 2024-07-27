#pragma once

/* Using GPL v 3.0 License */

/* The header 'mixutil.h' is being designed and prepared to be added any feature functionalities that could add help in reducing
the complexities of any programming keywords or overwhelming statement syntaxis such as the call to std::shared_ptr<T[]> and
std::unique_ptr<T[]>, and many others more .. which features are in the list of pending queue that are waiting to
be implemented in the any time of the future.
*/



#ifndef REQUIRE_H
#define REQUIRE_H
		#include <iostream>
		#include <array>
		#include <cstdarg>
		
		#include <initializer_list>
		#include <forward_list>
		/*
			include any functional headers here..
		*/

using Bit = unsigned char;
using UINT = unsigned int;
using ULONG = unsigned long;
using Byte = unsigned long;
using LongRange = long long;
using LONGFLOAT = long double;

typedef LongRange LINT;
typedef LONGFLOAT LFLOAT;

#define RET std::cout << "\n";

#define PRINT(_t) std::cout << (_t) << "\n";
#define RPRINT(_t) std::cout << (_t) << ", ";

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




template < class Ty >
_NODISCARD constexpr Ty&& _FORWRD(typename std::remove_reference_t<Ty>& _fwArgs) noexcept {
	return static_cast<Ty&&>(_fwArgs);
}


template < class Ty >
_NODISCARD constexpr Ty&& _FORWRD(typename std::remove_reference_t<Ty>&& _fwArgs) noexcept {
	static_assert(!std::is_lvalue_reference_v<Ty>, " can't forward object of a type specified by 'Ty' ");
	return static_cast<Ty&&>(_fwArgs);
}


template < class Ty >
_NODISCARD constexpr std::remove_reference_t<Ty>&& _MOVE(Ty&& _mvArgs) noexcept {
	return static_cast<std::remove_reference_t<Ty>&&>(_mvArgs);
}



template < class _Ty >
struct _Instantiator;


template < class Ty >
struct _Instantiator {
	
	_Instantiator() { Ty(); };

	template < class... Types >
	static Ty&& _Construct(Types&&... aArgs) {
		static_assert(std::is_constructible<Ty, Types...>(), "could not instantiate from a class specified by 'Ty' ");
		return _FORWRD<Ty>(Ty(_FORWRD<Types>(aArgs)...));
	}

};


template < class Ty >
std::unique_ptr<Ty[]> MK_U_ARRAY(unsigned int Nx) {
	return std::make_unique<Ty[]>(Nx);
}


template < class Ty >
std::shared_ptr<Ty[]> MK_S_ARRAY(unsigned int Nx) {
	return std::shared_ptr<Ty[]>(new Ty[Nx]());
}



template < class Ty >
using iList = std::initializer_list<Ty>;



template < class tElem  >
struct iList2 {
	using value_type = tElem;
	using reference_const = const tElem&;
	using pointer_const = const tElem*;
	using iterator = pointer_const;

	constexpr iList2() noexcept :_mFirst(nullptr), _mLast(nullptr)
	{};

	
	// move ctor
	constexpr iList2( iList2<tElem>&& rList2 ) {
		if (this == &rList2) return;
		*this = _MOVE(rList2);
	}

	// overloaded parameterized ctor, not a copy ctor
	constexpr iList2( const std::initializer_list<tElem>& rList ) noexcept {
		*this = rList;

	}

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


	// overloaded assignment operator
	constexpr const iList2<tElem>& operator= ( const std::initializer_list<tElem>& rList ) noexcept {
		_mFirst = rList.begin();  
		_mLast = rList.end();

		return *this;
	}


	// move assignment
	constexpr const iList2<tElem>& operator= ( iList2<tElem>&& rList2 ) {
		if (this == &rList2) return *this;

		_mFirst = rList2._mFirst;
		_mLast = rList2._mLast;

		rList2._mFirst = nullptr;
		rList2._mLast = nullptr;

		return *this;
	}

	_NODISCARD constexpr const value_type operator[](const unsigned int _idx) noexcept {
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



	template <const unsigned _v>
	struct isNumeric
	{
		enum class Type { value = std::is_integral_v<decltype(_v)> };

		using numType = std::conditional < std::is_integral_v<decltype(_v)>, decltype(_v), void > ::type;

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

	struct nullType {
		using value_type = std::nullptr_t;

		nullType(): _nan(nullptr) {};
		nullType(std::nullptr_t const) {}
		
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
		using type = typename P;
		using rootType = typename std::remove_pointer_t<P>;

		enum {
			isPointer = _BOOLC(std::is_pointer_v<P>),
			isRef = _BOOLC(std::is_reference_v<P>)
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

	
	
	template < class P , unsigned int Nx >
	struct ptrTraits<P[Nx]> {
		using type = typename P[Nx];
		using rootType = typename P;
		enum { isPointer = _BOOLC(std::is_pointer_v<type>), 
		       isConstant = _BOOLC(std::is_const_v<rootType>), 			 
		       isArray = _BOOLC(std::is_array_v<type> ), count = Nx, 
		       size = (unsigned int)(Nx * sizeof(P)) };
	};
	
	
	
	template < class P >
	struct ptrTraits<P[]> {
		using type = typename P[];
		using rootType = typename P;
		enum { isPointer = _BOOLC(std::is_pointer_v<type>), 
		       isConstant = _BOOLC(std::is_const_v<rootType> ), 
		       isArray = _BOOLC(std::is_array_v<type>) };
	};

	
	
	template < class P >
	struct ptrTraits<const P*> {
		using type = const typename P*;
		using rootType = const typename P;
		enum { isPointer = _BOOLC(std::is_pointer_v<type> ), 
		       isConstant = _BOOLC(std::is_const_v<rootType> ), 
		       isArray = _BOOLC(std::is_array_v<type> ) };
	};

	
	
	template < class P >
	struct ptrTraits<P* const> {
		using type = typename P* const;
		using rootType = typename P;
		enum { isPointer = _BOOLC(std::is_pointer_v<type> ), 
		       isConstantPointer = true, isArray = _BOOLC(std::is_array_v<type> ) };
	};
	
	
	
	template < class P >
	struct ptrTraits<const P* const> {
		using type = const typename P* const;
		using rootType = const typename P;
		enum {isPointer = _BOOLC(std::is_pointer_v<type> ), 
		      isConstant = _BOOLC(std::is_const_v<rootType> ), 
		      isConstantPointer = true, isArray = _BOOLC(std::is_array_v<type> ) };
	};
	
	
	
	
	template < class ret, class entity >
	struct ptrTraits< ret(_cdecl entity::*)() > {
		using type = ret(_cdecl entity::*)();
		using resultType = typename ret;
		using rootType = typename entity;
		enum {isPointerToMember = _BOOLC(std::is_member_pointer_v<type> ) };
	};
	
	
	
	
	template < class ret, class entity >
	struct ptrTraits < ret(_cdecl entity::*)(...) > {
		using type = ret(_cdecl entity::*)(...);
		using resultType = typename ret;
		using rootType = typename entity;
		enum { isPointerToMember = _BOOLC(std::is_member_function_pointer_v<type> ) };
	};
	
	
	template< class ret, class entity, class... argsT >
	struct ptrTraits < ret (_cdecl entity::*)(argsT&&...) > {
		using type = ret (_cdecl entity::* )(argsT&&...);
		using resultType = typename ret;
		using rootType = typename entity;
		enum { isPointerToMember = _BOOLC(std::is_member_function_pointer_v<type> ) };
	};
	
	
	
	template < class ret >
	struct ptrTraits< ret(_cdecl *)()> {
		using type = ret(_cdecl *)();
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

		inline static void initialize(P _p, LIST const& _ls) {
			for (list_iterator _it = _ls.begin(); _it != _ls.end(); _it++)
				*_p++ = *_it;
		}

	};
	
	
	
	namespace data {
		
		class Bucket  {
		public:
			Bucket() :_data("empty"), _msize(sizeof(Bucket)) { };

			Bucket(std::string const& _uStr) :_data(_uStr), _msize(sizeof(Bucket)) {

			};

			// overloaded copy-ctor
			Bucket(Bucket const& rBuck) {
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

	
	constexpr const bool isRange(int const _C, std::initializer_list<int const> const& _vals) {
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
	void FOR_EACH_PRINT(P begin, P end) {
	   for (; begin != end; begin++)
		std::cout << *begin << ", ";
		
	};

	// an iterator for the print function of any Buckets' types.
	template <>
	void FOR_EACH_PRINT<mix::data::Bucket*>(mix::data::Bucket* begin, mix::data::Bucket* end) {
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
	constexpr void smart_print(mix::data::Bucket* const begin, mix::data::Bucket* const end)
	{
		FOR_EACH_PRINT(begin, end);
	}

		
};
