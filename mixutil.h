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

/* The header 'mixutil.h' is designed and prepared to be added any feature functionalities that could mostly add help in reducing
   the complexities of any programming keywords or overwhelming statement syntaxis such as any call to std::shared_ptr<T[]> and 
   std::unique_ptr<T[]>, and many others more .. which features are in the list of pending queue that are waited to be implemented 
   in the any times of the future. 
 */

namespace mix {

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

		template < class Ty >
		using fnShareDel = bool(*)(Ty*);
		
		// a singe unique pointer type
		template < class Ty >
		using uniqueP = std::unique_ptr<Ty>;

		// a single shared pointer type
		template < class Ty >
		using shareP = std::shared_ptr<Ty>;


		template <class Ty >
		using SHARED_ARRAY = std::shared_ptr<Ty[]>;

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
			constexpr Alloc_Share() {};
			constexpr Alloc_Share(std::size_t const SZ) : shr(new Ty[SZ](), DelY() ) {}

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


	template <class PTR, class fnIter>
	void FOR_EACH(PTR begin, PTR end, fnIter const& fnt)
	{
		for (; begin != end; begin++)
			fnt(*begin);
	}




	template <class PTR>
	constexpr void smart_print(PTR const& begin, PTR const& end)
	{
		FOR_EACH(begin, end, [](auto const& _dataP)->decltype(void()) {
			std::cout << (_dataP) << ", ";
			}
		);
	}
	
	
	
	namespace data {
		class Bucket {
		public:
			Bucket() :_data("empty"), _msize(sizeof(Bucket)) { };
			Bucket(std::string const& _uStr) :_data(_uStr), _msize(sizeof(Bucket)) {

			};

			// overloaded copy-ctor
			Bucket(Bucket const& rBuck) {
				if (this == &rBuck) return;
				*this = rBuck;
			}

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

};
