#pragma once

#ifndef REQUIRE_H
#define REQUIRE_H
		#include <iostream>
		#include <string>
		#include <memory>
		#include <cstdarg>
		
		#include <initializer_list>
		#include <forward_list>
#endif





/* Common Creative License's Properties */

/* The header 'mixutil.h' is designed and prepared to be added any feature functionalities that could add help in reducing
the complexities of any programming keywords or overwhelming statement syntaxis such as the call to std::shared_ptr<T[]> and
std::unique_ptr<T[]>, and many others more .. which features are in the list of pending queue that are waiting to
be implemented in the any time of the future.
*/
namespace mix {

	
	template < class Ty >
	using iList = std::initializer_list<Ty>;



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

	
	
	
	
	template < class Ty >
	struct isPrimitive {
	private:
		typedef typename Ty type_;

	public:
		static const bool Result = std::is_fundamental<type_>::value;
	};
	

	
	
	template < class LV >
	struct add_lvalue {
		using type = typename LV;
	};



	template < class R >
	struct addRef {
		using refType = typename std::add_lvalue_reference<R>::type;
	};


	
	template < class R >
	struct removeRef {
		using type = typename add_lvalue<R>::type;
	};


	
	
	template < class P >
	struct addPtr {
		using ptrType = typename std::add_pointer<P>::type;
	};

	
	
	template < class P  >
	struct removePtr;


	
	template < class P >
	struct removePtr<P*> {
		using type = typename std::remove_pointer<P*>::type;
	};



	
	
	template < class RR >
	struct add_rvalue {
		using type = typename std::add_rvalue_reference<RR>::type;
	};



	
	template < class RR >
	struct remove_rvalue {
		using type = typename std::remove_reference<RR>::type;
	};
	
	
	
	
	
	

	namespace ptr_type {


		// Deleter Object for UNIQUE_ARRAY<Ty>
		template < class Ty >
		struct unique_array_del
		{

		};


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
		template <class Ty, class DelX = unique_array_del<Ty*> >
		using UNIQUE_ARRAY = std::unique_ptr<Ty[], decltype(DelX()) >;
	} // end of ptr_type namespace




	namespace smart_ptr {

		// a single unique factory

		template < class Ty, class delTy = std::default_delete<Ty> >
		constexpr ptr_type::uniqueP<Ty, delTy> up_create(void) {
			std::unique_ptr<Ty, delTy> _up;
			return (_up);
		}

		// a single shared factory with default deleter
		template < class Ty >
		constexpr const ptr_type::shareP<Ty> sp_create(void) {
			return std::make_shared<Ty>();
		}

		// overloaded of a single shared factory with user custom deleter
		template < class Ty, class TyDel = std::default_delete<Ty> >
		constexpr const ptr_type::shareP<Ty> sp_create(int) {
			return std::shared_ptr<Ty>(new Ty(), TyDel());
		}

		// unique array factory
		template < class Ty, class DelX = ptr_type::unique_array_del<Ty*>, std::size_t SZ = 1 >
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
		template <class Ty, class DelY = ptr_type::shared_array_del<Ty*> >
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

	// an iterator for print function of any Buckets' types.
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
