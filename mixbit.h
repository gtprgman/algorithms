#pragma once
/* Using License: GPL .v .3.0 */

#ifndef HUFF_TREE
	#ifndef REQUIRE_H
		#include <vector>	 
	#endif
#endif


#ifndef MX_BIT
	#define MX_BIT

#endif

// an integer representation of a hex constant ( A .. F )
enum class INT_HEX : std::int8_t
{
	A = 10, B= 11, C = 12, D= 13, E = 14, F = 15
};



// a list of constants in hexa digit { A .. F }
constexpr std::initializer_list<const int64_t> HxC = { (int8_t)INT_HEX::A, (int8_t)INT_HEX::B, (int8_t)INT_HEX::C, 
												   		(int8_t)INT_HEX::D, (int8_t)INT_HEX::E, (int8_t)INT_HEX::F 
													 };



// a character selector for any constant in hexa digit { A .. F }
static inline char&& HEX_CHR(int64_t&& _i64)
{
	static char _cfx = '0';
	int64_t _hc = 0;

	_hc = (mix::isRange(_i64, HxC))? _i64 : 0;

	_cfx = '0';

	switch (_hc)
	{
			case 10: _cfx =  'A'; break;
			case 11: _cfx =  'B'; break;
			case 12: _cfx =  'C'; break;
			case 13: _cfx =  'D'; break;
			case 14: _cfx =  'E'; break;
			case 15: _cfx =  'F'; break;
		default: break;
	}
	return std::move(_cfx);
}



// an integer selector for any constant in hexa digit { A .. F }
constexpr int8_t HEX_INT(char&& _chxf)
{
	int8_t xCh = 0;

	switch (_chxf)
	{
			case 'A': xCh = (int8_t)INT_HEX::A; break;
			case 'B': xCh = (int8_t)INT_HEX::B; break;
			case 'C': xCh = (int8_t)INT_HEX::C; break;
			case 'D': xCh = (int8_t)INT_HEX::D; break;
			case 'E': xCh = (int8_t)INT_HEX::E; break;
			case 'F': xCh = (int8_t)INT_HEX::F; break;
			default: break;
	}
	return xCh;
}


// a header data structure for storing encoding information of huffman method
struct _Canonical
{
	char _xData;
	int64_t _bitLen;
	int64_t _codeWord;
	int64_t _rle_bit_len;

	operator int64_t() {
		return this->_bitLen;
	}

	operator char() {
		return this->_xData;
	}
};



// a data structure representing a subset of _Canonical
struct Can_Bit : virtual public _Canonical
{
	operator char() {
		return this->_xData;
	}

	operator int64_t() {
		return this->_codeWord;
	}
};



#define SPACE (char)32
#define halfSz(_Tot_) (_Tot_ / 2) - 1

#define bit_str(_x_) _Get_Binary_Str(_x_)
#define int_bit(_x_) _Int_from_Bit_Str(_x_)
#define len_bit(_x_) _Get_Num_of_Bits(_x_)



// calculate how many of bit '1' in the data
constexpr int64_t&& count_bit_set(int64_t&& _x)
{
	int64_t _rcx = 0;

	while (_x > 0)
	{
		if (_x % 2) ++_rcx;
		_x /= 2;
	}

	return int64_t(_rcx);
}



// generates a digit '1' a number of '_reps' times
#define x1_bit(_ch_ptr, _reps)						\
{													\
	repl_char('1', _reps);							\
}



constexpr const UINT oneAdder(const UINT _x)
{
	return _x + (UINT)1;
}



template < class T >
constexpr bool vectorClean(std::vector<T>& _vecT)
{
	if (!_vecT.empty()) _vecT.clear();
	return (_vecT.empty());
}


constexpr unsigned HEXBIT = 4;
constexpr unsigned BYTE = 8;
constexpr unsigned WORD = 16;
constexpr unsigned DWORD = 32;
constexpr unsigned QWORD = 64;
constexpr unsigned MULTIWORDS = 128;

constexpr int64_t i4Mask = 0xF;
constexpr int64_t i8Mask = 255;
constexpr int64_t i16Mask = 65535;
constexpr int64_t i32Mask = 0xFFFFFFFF;

static int64_t&& extract_Ints(int64_t&);
constexpr int64_t&& set_low_bit(int64_t&&);
constexpr int64_t&& range_bit_set(int64_t&&, int64_t&&);

// generates bit '0' a number of n_Bits
static std::string&& zero_bits(const size_t&);

// returns a char representation of an ascii integer
inline static char&& to_char(const int&);

// returns a boolean indicating whether a specified char is an alphanumeric or not
inline static const bool is_alpha_num(const char&);

// returns a real integer of an alphanumeric character
inline static const int chartoint(const char&);

// returns a binary representation of an alphanumeric character
inline static std::string&& alphaNum2Bin(char&&);

// convert a hex string evaluated by 'To_HexF' to its binary representation
static std::string&& HxFs_To_Bin(std::string&&);


// multiple the number of a given character with a definite count
static std::string&& repl_char(char&&, const size_t&);

// Invoker macro for 'num_of_bits<T>::eval()'
template <typename _Ty >
static _Ty&& _Get_Num_of_Bits(_Ty&&);

// Invoker macro for 'to_binary<T>::eval()' 
template < typename _Ty = int>
static std::string&& _Get_Binary_Str(_Ty&&);

// Invoker macro for 'bin_to_dec<T>::eval()'
template <class _T = int>
static _T&& _Int_from_Bit_Str(std::string&&);

// Invoker macro for extract_Ints
static inline int64_t&& Ints_Of(int64_t& _ebx)
{
	return extract_Ints(_ebx);
}

// integrates number of integers in the vector into a single 64 bit integer
static const int64_t mix_integral_constant(const std::vector<int64_t>&);

// generate encoding information table based on Huffman Canonical Method
static void _Gen_Canonical_Info(std::vector<int64_t>&, const std::vector<int64_t>&);

// make sure every codeword integer is unique
static void cni_enforce_unique(std::vector<_Canonical>&);

// packing a series of canonical bits into one integer and return the result as a bits string.
static const intmax_t cni_bits_pack(const std::vector<int64_t>&);

// saves a packed canonical bit to a one specified file
static const size_t save_cni_bit(std::FILE*&, const int64_t&);

// reads a packed canonical bit from one file and parses it to a vector integer
static const int read_cni_bit(std::FILE*&, std::vector<int64_t>&);

// the max. number of bits evaluated by 'BIT_TOKEN()'
unsigned int MAX_BIT = 0;


constexpr int64_t&& HI_HEX(int64_t&& _rbx)
{
	return (i4Mask << 4) & _rbx;
}

constexpr int64_t&& LO_HEX(int64_t&& _rbx)
{
	return (i4Mask & _rbx);
}

constexpr int64_t&& BYTE_PTR_HI(int64_t&& _rbx)
{
	return (i8Mask << 8) & _rbx;
}

constexpr int64_t&& BYTE_PTR_LO(int64_t&& rbx_)
{
	return 0xFF & rbx_;
}

constexpr int64_t&& WORD_PTR_HI(int64_t&& _rdx)
{
	return (i16Mask << 16) & _rdx;
}

constexpr int64_t&& WORD_PTR_LO(int64_t&& rdx_)
{
	return i16Mask & rdx_;
}

constexpr int64_t&& DWORD_PTR_HI(int64_t&& _rax)
{
	return (i32Mask << 32) & _rax;
}

constexpr int64_t&& DWORD_PTR_LO(int64_t&& rax_)
{
	return i32Mask & rax_;
}

// pulls off a byte from the MSB of a specified integer '_rcx'
static inline int64_t&& BYTE_PTR_X(int64_t&& _rcx)
{
	const int64_t _rdi = len_bit(int64_t(_rcx) );
	const int64_t _rsi = _rdi - 7;

	const int64_t _rgx = range_bit_set(int64_t(_rsi), int64_t(_rdi) );
	return _rcx & _rgx;
}


 // pulls off a hex number from the MSB of a specified integer '_rex'
static inline int64_t&& HEX_PTR_X(int64_t&& _rex)
{
	const int64_t _rbx = len_bit(int64_t(_rex) );
	const int64_t _rax = _rbx - 3;
	const int64_t _rfx = range_bit_set(int64_t(_rax), int64_t(_rbx) );
	return _rex & _rfx;
}

/* pulls off a series of bit from a specfied integer '_rax', starting from a specified bit position
   to a definite end bit position */
static inline int64_t&& ANY_PTR_X(int64_t&& _rax, int64_t&& _startBit, int64_t&& _endBit)
{
	const int64_t _rbx = len_bit(int64_t(_rax) );
	const int64_t& _rdi = _endBit;
	const int64_t& _rsi = _startBit;

	const int64_t _rfx = range_bit_set(int64_t(_rsi), int64_t(_rdi) );
	return _rax & _rfx;
}


/* generates number of set bits on the LSB of a data unit. (little - endian).
   NB: {
	 bit indices are zer0-based oriented. Eg. to specify the first 8 bits or
	 any n-bits are set, the rule is: set_low_bit( (n-1) ), so the first 8 bits
     would be: set_low_bit(7). The first 4 bit hex number would be: set_low_bit(3)
     The first 16 bits number (65535) would be: set_low_bit(15).
   }
 */
constexpr int64_t&& set_low_bit(int64_t&& _Max)
{
	int64_t _TotSum = 0;
	const int64_t _hi = _Max;

	for (int64_t _bi = _hi; _bi >= 0;  _bi--)
	{
		_TotSum += (int64_t)( 1 * std::pow(2, _bi) );
	}

	return int64_t(_TotSum);
}



/* generates a range of set bits from the initial bit position to a determined bit position of a data unit.
   The indices of bit are based on the same rule as 'set_low_bit()'. (little-endian) 
   Eg. To specify the first 8 bits are set, you should call range_bit_set() with argument 0 
       as the min and 7 as the max argument.
   
*/
constexpr int64_t&& range_bit_set(int64_t&& _Min, int64_t&& _Max)
{
	int64_t _Sum = 0;
	const int64_t _start_bit = _Min, _end_bit = _Max;

	if (_start_bit < 0 || _end_bit < 0) {
		std::cerr << " \n the specified ranges yield to negative bit indices ..\n";
		return 0;
	}

	for (int64_t _bi = _start_bit; _bi <= _end_bit; _bi++)
	{
		_Sum += (int64_t)(1 * (int64_t)std::pow(2, _bi) );
	}

	return int64_t(_Sum);
}


// get '_n_Bits' from the LSB of a specified integer ' _valX '.
constexpr int64_t&& get_n_of_lsb(int64_t&& _valX, int64_t&& _n_Bits)
{
	int64_t _v = 0;
	const int64_t _b = 0b1;

	for (int64_t _x = 0; _x < _n_Bits; _x++)
	{
		_v |= _valX & (_b << _x);
	}
	return int64_t(_v);
}


// get 'N_Bits' from the MSB of a specified integer '_Vx'.
static inline int64_t&& get_n_of_msb(int64_t&& _Vx, int64_t&& N_Bits)
{
	const int64_t _tot_Bits = (int64_t)_Get_Num_of_Bits(int64_t(_Vx) ) - 1;
	const int64_t _rbx = (int64_t)0b1 << _tot_Bits;
	int64_t _rdx = 0;

	_rdx = _rbx;

	for (int64_t _n = 0; _n < N_Bits; _n++)
	{
		_rdx |= _Vx & (_rbx >> _n);
	}
	return int64_t(_rdx);
}



// returns a specific named token which evaluates to a max. number of bits.
constexpr uint64_t&& BIT_TOKEN(int64_t&& nBits)
{
	if (nBits > 0 && nBits <= HEXBIT) MAX_BIT = HEXBIT;
	else if (nBits > HEXBIT && nBits <= BYTE) MAX_BIT = BYTE;
	else if (nBits > BYTE && nBits <= WORD) MAX_BIT = WORD;
	else if (nBits > WORD && nBits <= DWORD) MAX_BIT = DWORD;
	else if (nBits > DWORD && nBits <= QWORD) MAX_BIT = QWORD;
	else MAX_BIT = MULTIWORDS;

	return MAX_BIT;
}



/* a data structure for storing every byte portion of an integer value.
   A little-endian order is being assumed, so the first two elements of _eax
   represent the MSB of a 32-bit integer. */
struct _Int_Bits
{
	_Int_Bits() : _eax{ 0,0,0,0 } {}

	_Int_Bits(_Int_Bits const& _DWORD)
	{
		if (this == &_DWORD) return;
		*this = _DWORD;
	}

	const _Int_Bits& operator= (const _Int_Bits& _rDWORD)
	{
		if (this == &_rDWORD) return *this;

		this->_eax[0] = _rDWORD._eax[0];
		this->_eax[1] = _rDWORD._eax[1];
		this->_eax[2] = _rDWORD._eax[2];
		this->_eax[3] = _rDWORD._eax[3];

		return *this;
	}

	int _eax[4] = { 0,0,0,0 };
};


template <const unsigned>
struct fixN;


// invert every bit in the bit array.
inline static void invert_bits(bool _pb[], const unsigned nBits)
{
	for (unsigned j = 0; j < nBits; j++)
	{
		_pb[j] = !_pb[j];
	}
}


// get a bit character from a position in the bit array as specified by '_pb'.
inline static const char* char_from_bit(const bool _pb[], const int _index)
{
	return (_pb[_index])? (char*)"1" : (char*)"0";
}


// get a value from a position in the bit array 
inline static const unsigned n_of_bit(const bool _pb[], const int _index)
{
	return _pb[_index];
}


// obtain a full bits string from the bit array specified by '_pb'.
inline static const std::string str_from_bits(const bool _pb[], const unsigned nBits)
{
	char* _ss = new char[nBits];

	for (unsigned i = 0; i < nBits; i++)
		_ss[i] = (_pb[i])? '1' : '0';

	_ss[nBits] = '\0';
	return _ss;
}


/* obtain a bits from a string data, this includes the most significant bits padded
   to the left of primary bits */
inline static const bool* bits_from_str(const std::string& _cBits)
{

	const int64_t numBits = (int64_t)std::strlen(_cBits.data());
	uint64_t nZeros = BIT_TOKEN(int64_t(numBits)) - numBits;

	bool* _pb = new bool[nZeros + numBits];

	// pad trailing bits with '0'
	for (unsigned z = 0; z < nZeros; z++) _pb[z] = false;

	// top-up leading bits with '_cBits'
	for (unsigned i = 0; i < numBits; i++)
	{
		_pb[nZeros++] = (_cBits.c_str()[i] == 49)? true : false;
	}

	return _pb;
}



// uppercase the specified char
constexpr char&& upCase(int&&);

// downcase the specified char
constexpr char&& downCase(int&&);

// scan a specific string pattern within the target string and return the found position in the target string.
static int&& strPos(const char*, const char*);

// scan a byte value within the specified string pattern and return the found position in the string being scanned
static int&& strNPos(const char*, const int);

// scan a substring within the target string and return the found substring in the target string.
inline static std::string&& scanStr(const char*, const char*);

inline static const char* concat_str(char* , const char*);

// take the n number of characters from the left end of the string
inline static std::string&& lstr(const char*, const std::size_t&);

/* get a number of characters from a string, starting from a position specified
   by '_Start', then take the number of characters specified by '_End'. The position is
   numbered from [0] to [n - 1], but the number of characters snapped is based on common
   sense of counting.
*/
inline static std::string&& snapStr(const char*, const size_t&, const size_t&);

/* replaces 'x' number of characters of a string with a character specified by '_tpChr' ,
   the offset of the string is starting from 0 to [n - 1]. The 'x' number of characters
   is specified using common counting principle.
*/
inline static std::string&& tapStr(const char*, const char&, const size_t&, const size_t&);

/* supersede part of a string with a specified substring at a specified position in the target string.
   Zero-based index array accesses is assumed */
inline static std::string&& tapStrBy(const char*, const char*, const int&);

/* pad the right end of a string with a number of unique characters specified by '_padC'.
   the '_Count' argument is specified using common arithmetic principle. */
inline static std::string&& rtrimx(const char*, const size_t&, const char&);

// removes extra spaces before and after the specified letter words
inline static std::string&& LRTrim(const char*);

/*pad the left end of a string with a number of unique chars specified by '_padCh',
  assumed zero-based index array accesses and arithmetic counting principle. */
inline static std::string&& ltrimx(const char*, const size_t&, const char& _padCh);

// take the n number of characters from the right end of the string
inline static std::string&& rstr(const char*, const std::size_t&);

// Convert alphanumeric string '0,1,2..9' to integer
inline static int64_t&& strtoint(std::string&&);

inline static std::string&& inttostr(const int64_t&);

// return the exact number of bits that composing a value '_n'
static uint64_t&& proper_bits(int64_t&&);

// return the least significant portion of bits of a specified value '_v'
static int64_t&& LoPart(int64_t&&);

// return the most significant portion of bits of a specified value '_v'
static int64_t&& HiPart(int64_t&&);

// splits an integer into its composing bit factors and stored it to a vector
inline static void parseInt(int64_t&, std::vector<int64_t>&);

// parses each integer of a vector into its 8 bit composing binary form.
inline static void parseByte(std::vector<int>&, const std::vector<int64_t>&);

// merge the MSB and LSB portions together to form a single unit of data
constexpr int64_t&& MergeBits(int64_t&&, int64_t&&);

inline static const char* reverse_str(const char*);


inline static std::string&& repl_char(char&& _aChar, const size_t& _Count)
{
	static std::string _repStr = "\0";

	if (!_Count || (int64_t)_Count < 0) return std::move(_repStr);

	char* _repC = new char[_Count];

	for (size_t i = 0; i < _Count; i++) _repC[i] = char(_aChar);

	
	_repC[_Count] = 0;

	_repStr = _repStr.assign(_repC);


	NULLP(_repC);
	return std::move(_repStr);
}




static inline std::string&& zero_bits(const size_t& n_Bits)
{
	intmax_t xZeros = n_Bits;
	static std::string _ci = " ";

	if (!xZeros || xZeros < 0 ) xZeros = 1; // minimum number of '0' should at least 1

	_ci = repl_char('0', xZeros);
	return std::move(_ci);
}



// evaluate to how much number of bits that made up a constant value '_v'
template < class T >
struct num_of_bits
{
	using type = typename T;

	static type&& eval(type&& _v)
	{
		static type&& cnt = 0;
		type&& _val = type(_v);

		cnt = 0;

		if (!_val) {
			cnt = 1;
			return std::move(cnt);
		}

		while (_val > 0)
		{
			_val /= 2;
			++cnt;
		}

		return std::move(cnt);
	}
};



// convert a specified decimal constant to its binary form
template < typename T >
struct to_binary
{
	using value_type = typename std::remove_reference_t<T>;

	static inline std::string&& eval(value_type&& _dec)
	{
		value_type _q = 0;
		value_type _bsz = num_of_bits<value_type>::eval(value_type(_dec) );
		_value = _dec;

		if (!_value) {
			_bs = "0";
			return std::move(_bs);
		}

		if (_bsz > 0)
			_bs = new char[_bsz]; 

		for (value_type i = 0; i < _bsz && _value > 0; i++)
		{
			_q = _value % 2;
			_bs[i] = (_q)? '1' : '0';
			_value = (_value > 1)? _value /= 2 : 0;
		}

		_value = 0;
		_bs[_bsz] = 0;
		_bs = (char*)reverse_str(_bs.data());
		return std::move(_bs);
	}

private:
	static value_type _value;
	static std::string _bs;
};

// static members initializer
template <class T>
typename to_binary<T>::value_type to_binary<T>::_value = 0;

template <class T>
std::string to_binary<T>::_bs = "\0";



template <class T >
struct bin_to_dec
{
	using value_type = typename std::remove_reference_t<T>;

	// the bit string is assumed to be in little-endian order.
	static inline value_type&& eval(std::string&& _strBits)
	{
		std::size_t lenMax = _strBits.size();
		std::size_t _maxBit = lenMax - 1;
		value_type k = 0, b = 0;

		_Dec = 0;

		for (std::size_t i = _maxBit; i > 0; i--)
		{
			b = (_strBits[i] == 49)? 1 : 0;
			_Dec += b * (value_type)std::pow(2, k++);
		}

		b = (_strBits[0] == 49) ? 1 : 0;
		_Dec += b * (value_type)std::pow(2, _maxBit);

		return std::move(_Dec);
	}

private:
	static value_type _Dec;
};
// static member initializer
template <class T>
typename bin_to_dec<T>::value_type bin_to_dec<T>::_Dec = 0;



template < class T, bool _Val = std::is_integral_v<T>,
            class _Ty = std::conditional_t<_Val, intmax_t, mix::nullType> >
struct To_HexF {
	using val_type = typename std::remove_reference_t<_Ty>;

	static inline std::string&& eval(val_type&& val_i64)
	{
		_hxs.assign( hex_str(val_type(val_i64) ) );
		
		return std::move(_hxs);
	}

private:
	static std::string _hxs;
	static std::vector<val_type> _x16c;

	static inline std::string&& hex_str(val_type&& _i64)
	{
		val_type _m64 = 0;
		static std::string _hxf = "\0";

		vectorClean(_x16c);

		if (_i64 <= 0) return std::move(_hxf);

		while (_i64 > 0)
		{
			_m64 = _i64 % 16;
			_x16c.push_back(_m64);
			_i64 /= 16;
		}

		mix::generic::STL_Content_Reverse(_x16c);

		for (const val_type& _ix : _x16c)
		{
			_hxf = concat_str((char*)_hxf.data(), (HEX_CHR(val_type(_ix)) == '0')? inttostr(_ix).c_str() : 
													new char[2] {HEX_CHR(val_type(_ix)), '\0'} );
		}

		vectorClean(_x16c);
		return std::move(_hxf);
	}
};

// static member initialization ..
template <class T, bool _v, class _Ty>
std::string To_HexF<T, _v, _Ty>::_hxs = "\0";

template <class T, bool _v, class _Ty>
std::vector<typename To_HexF<T, _v, _Ty>::val_type> To_HexF<T, _v, _Ty>::_x16c = {};



static inline std::string&& HxFs_To_Bin(std::string&& _xhFs)
{
	int _xc = 0;
	size_t _hxSz = 0;
	std::vector<std::string> _vcBin;
	static std::string _hxsBin;

	_hxsBin = "\0";

	for (std::string::iterator _xItr = _xhFs.begin(); _xItr != _xhFs.end(); _xItr++)
	{
		_xc = HEX_INT(char(*_xItr));

		if (_xc) _vcBin.push_back(bit_str(_xc));
		else _vcBin.push_back(alphaNum2Bin(char(*_xItr)));

		_xc = 0;
	}
	
	for (const auto& _xb : _vcBin) {
		_hxSz = _xb.size();

		_hxsBin = concat_str(_hxsBin.data(), (_hxSz == 4)? _xb.c_str() :
								concat_str(zero_bits(1).data(), _xb.c_str()));
	}

	vectorClean(_vcBin);
	return std::move(_hxsBin);
}


template <typename _Ty>
static inline _Ty&& _Get_Num_of_Bits(_Ty&& _ax)
{
	return num_of_bits<_Ty>::eval(_Ty(_ax) );
}


template < typename _Ty >
static inline std::string&& _Get_Binary_Str(_Ty&& _Dx)
{
	using _Type = std::remove_reference_t<_Ty>;
	
	const _Type HXZ = 4;
	const _Type _LenDX = num_of_bits<_Type>::eval(_Type(_Dx));
	const _Type _DiffT = HXZ - _LenDX;
	static std::string _StrBin = "\0";
	char* _zeroBit = (char*)zero_bits(_DiffT).c_str();

	_StrBin = (char*)" ";

	_StrBin = concat_str((char*)_StrBin.c_str(), (_DiffT >= 4)? to_binary<_Type>::eval(_Type(_Dx)).c_str() :
				concat_str(_zeroBit, to_binary<_Type>::eval(_Type(_Dx)).c_str()));
			

	return std::move(_StrBin);
}


template <class _T >
static inline _T&& _Int_from_Bit_Str(std::string&& Bit_Str)
{
	return bin_to_dec<_T>::eval(Bit_Str.data());
}


static inline const int64_t mix_integral_constant(const std::vector<int64_t>& v_Ints)
{
	int64_t rbx = 0, nShift = 0, bitsz = 0;
	const size_t v_size = v_Ints.size();

	for (size_t vi = 0; vi < v_size; vi++)
	{
		bitsz = len_bit(int64_t(v_Ints[vi]));
		nShift = (bitsz <= 4)? 4 : bitsz;

		rbx = (rbx)? (rbx << nShift) | v_Ints[vi] : rbx | v_Ints[vi];
	}

	return rbx;
}



static inline void _Gen_Canonical_Info(std::vector<_Canonical>& _cBit, const std::vector<_Canonical>& _Codes)
{
	int64_t _len1 = 0, _len2 = 0, _bi = 0, _xDiff = 0;
	const std::size_t _codeSize = _Codes.size();
	std::vector<_Canonical> _CnTemp = {};
	_Canonical _Canon = _Codes[0];

	_len1 = _Canon._bitLen;

	std::string&& _si = zero_bits(_len1);
	_bi = strtoint(_si.data());
	_Canon._codeWord = _bi;

	_CnTemp.push_back(_Canon); _Canon = {};

	for (size_t z = 1; z < _codeSize; z++)
	{
		_Canon = _Codes[z - 1];
		_len1 = len_bit(int64_t(_Canon._codeWord));
		_len2 = _Codes[z]._bitLen;

		if (_len2 > _len1)
		{
			_xDiff = _len2 - _len1;
			_bi = _Canon._codeWord;
			++_bi;
			_bi <<= _xDiff;
		}
		else if (_len2 < _len1)
		{
			_xDiff = _len1 - _len2;
			_bi = _Canon._codeWord;
			--_bi;
			_bi >>= _xDiff;

		}
		else if (_len2 == _len1)
		{
			_bi = _Canon._codeWord;
			++_bi;
		}

		_Canon = _Codes[z];
		_Canon._codeWord = _bi;

		_CnTemp.push_back(_Canon );
		_Canon = {};
	}

	for (std::vector<_Canonical>::iterator _CiT = _CnTemp.begin(); _CiT != _CnTemp.end(); _CiT++)
		_cBit.push_back(*_CiT);

}


static inline void cni_enforce_unique(std::vector<_Canonical>& cniDat)
{
	intmax_t _cw = 0;
	const size_t cnSize = cniDat.size();

	for (size_t _ci = 0; _ci < cnSize; _ci++)
	{
		_cw = cniDat[_ci]._codeWord;
		_cw += _ci;
		cniDat[_ci]._codeWord = _cw;
	}
	
}


static inline const intmax_t cni_bits_pack(const std::vector<int64_t>& _canVec)
{
	int64_t _x = 0;
	const size_t canSz = _canVec.size();

		for (size_t t = 0; t < canSz; t++)
		{
			_x <<= len_bit(int64_t(_canVec[t] ));
			_x |= _canVec[t];
		}
		
		return _x;
}


static inline const size_t save_cni_bit(std::FILE*& _fHandle, const int64_t& v_bit)
{
	if (!_fHandle) {
		std::cerr << "\n Can't open file with the specified I/O handle. \n";
		return 0;
	}
	

	std::string::iterator _xIt;
	std::string _tmpS = "\0";
	std::string _hexF = To_HexF<int64_t>::eval(int64_t(v_bit));
	std::string _BitStr = HxFs_To_Bin(_hexF.c_str());
	char* _pStr = nullptr;

	const size_t _BitSize = _BitStr.size();
	const size_t _totBytes = (size_t)(_BitSize / 8 );
	const size_t _totBits = _totBytes * 8;
	const size_t _BitDifft = (_totBits < _BitSize)? _BitSize - _totBits : _totBits - _BitSize;
	size_t _bytesWritten = 0;

	int _xRec = 0;

	_xIt = _BitStr.begin();
	_pStr = (char*)_xIt._Ptr;

	for (size_t z = 0; z < _totBytes; z++)
	{
		_tmpS = scanStr(_pStr, lstr(_pStr, 8).c_str());

		if (!_tmpS.empty())
		{
			_xRec = int_bit(_tmpS.c_str());
			if (_xRec) std::fputc(_xRec, _fHandle);
			_xRec = 0; _bytesWritten += 8;
		}

		_xIt += 8;
		_pStr = (char*)_xIt._Ptr;
		_tmpS = "\0";
	}

	if (_BitDifft > 0)
	{
		_tmpS = rstr(_BitStr.c_str(), _BitDifft);
		_xRec = int_bit(_tmpS.c_str());
		if (_xRec) std::fputc(_xRec, _fHandle); _bytesWritten += _BitDifft;
	}

	_hexF.clear();
	_BitStr.clear();

	NULLP(_pStr);

	return _bytesWritten;
}


static inline const int read_cni_bit(std::FILE*& _fHandle, std::vector<int64_t>& Int_Bit)
{
	int read_bit = 0, read_size = 0;

	if (!_fHandle) return 0;

	while ((read_bit = std::fgetc(_fHandle)) > -1 )
	{
		Int_Bit.push_back(read_bit);
		++read_size;
	}

	
	return read_size;
}



static inline uint64_t&& proper_bits(int64_t&& _n)
{
	const int64_t _nBits = num_of_bits<int64_t>::eval(int64_t(_n) );
	const int64_t _maxBits = BIT_TOKEN(int64_t(_nBits) );

	return _maxBits;
}


static inline int64_t&& LoPart(int64_t&& _v)
{
	const int64_t bit_width = len_bit(int64_t(_v) );
	const int64_t half_bit = bit_width / 2;
	const int64_t bit_order_difft = bit_width - half_bit;
	const int64_t low_mask = set_low_bit(int64_t(bit_order_difft) );
	const int64_t low_order_value = low_mask & _v;

	return int64_t(low_order_value);
}


static inline int64_t&& HiPart(int64_t&& _v)
{
	const int64_t bit_wide = len_bit(int64_t(_v) );
	const int64_t bit_half = bit_wide / 2;
	const int64_t bit_difft = bit_wide - bit_half;
	const int64_t bit_high_mask = range_bit_set(int64_t(bit_half), int64_t(bit_wide) );
	const int64_t high_order_value = bit_high_mask & _v;
	
	return int64_t(high_order_value);
}


inline static void parseInt(int64_t&& _rdx, std::vector<int64_t>& _Ints)
{
	int64_t _rbx = 0, _rcx = 0;

	while ((_rbx = Ints_Of(_rdx)) > 0)
	{
		while ( (_rcx = len_bit(int64_t(_rbx) )) > 16 )
				_rbx >>= 8;

		_Ints.push_back(_rbx);
	}
}

inline static void parseByte(std::vector<int>& _iBytes, const std::vector<int64_t>& _Ints)
{
	int64_t _rdx = 0, _rcx = 0,
			_eax = 0, _ebx = 0;

	int		_ax = 0, _ah = 0, _al = 0,
			_dx = 0, _dh = 0, _dl = 0;

	for (const int64_t& i64 : _Ints)
	{
		_rdx = i64; _rcx = len_bit(int64_t(_rdx) );
		
		if (_rcx <= 8)
		{
			_dx = (int)_rdx;
			_iBytes.push_back(_dx);
		}
		else if (_rcx > 8 && _rcx <= 16)
		{
			_dh = (int)BYTE_PTR_HI(int64_t(_rdx)) >> 8; _iBytes.push_back(_dh);
			_dl = (int)BYTE_PTR_LO(int64_t(_rdx)); _iBytes.push_back(_dl);
		}
		else if (_rcx > 16 && _rcx <= 32)
		{
			_ax = (int)WORD_PTR_HI(int64_t(_rdx)) >> 16; _ah = (int)BYTE_PTR_HI((int64_t)_ax) >> 8; _al = (int)BYTE_PTR_LO((int64_t)_ax);
			_dx = (int)WORD_PTR_LO(int64_t(_rdx));  _dh = (int)BYTE_PTR_HI((int64_t)_dx) >> 8; _dl = (int)BYTE_PTR_LO((int64_t)_dx);

			_iBytes.push_back(_ah); _iBytes.push_back(_al);
			_iBytes.push_back(_dh); _iBytes.push_back(_dl);

		}
		else if (_rcx > 32 && _rcx <= 64)
		{
			_eax = DWORD_PTR_HI(int64_t(_rdx)) >> 32;
			_ebx = DWORD_PTR_LO(int64_t(_rdx) );

			_ax = (int)WORD_PTR_HI(int64_t(_eax) ) >> 16; _ah = (int)BYTE_PTR_HI((int64_t)_ax) >> 8; _al = (int)BYTE_PTR_LO((int64_t)_ax);
			_dx = (int)WORD_PTR_LO(int64_t(_eax) ); _dh = (int)BYTE_PTR_HI((int64_t)_dx) >> 8; _dl = (int)BYTE_PTR_LO((int64_t)_dx);

			_iBytes.push_back(_ah); _iBytes.push_back(_al);
			_iBytes.push_back(_dh); _iBytes.push_back(_dl);

			_ax = (int)WORD_PTR_HI(int64_t(_ebx) ) >> 16; _ah = (int)BYTE_PTR_HI((int64_t)_ax) >> 8; _al = (int)BYTE_PTR_LO((int64_t)_ax);
			_dx = (int)WORD_PTR_LO(int64_t(_ebx) ); _dh = (int)BYTE_PTR_HI((int64_t)_dx) >> 8; _dl = (int)BYTE_PTR_LO((int64_t)_dx);

			_iBytes.push_back(_ah); _iBytes.push_back(_al);
			_iBytes.push_back(_dh); _iBytes.push_back(_dl);
		}

	}
}



constexpr int64_t&& MergeBits(int64_t&& _Hi, int64_t&& _Lo)
{
	int64_t _Bits = 0b0;

	_Bits = _Hi | _Lo;

	return int64_t(_Bits);
}



/* extract the composing bit factors out of an integer '_v'.
*/
static inline int64_t&& extract_Ints(int64_t& _v)
{
	int64_t _d1 = -1, bit_width = 0;

	bit_width = len_bit(int64_t(_v) );

	if (bit_width <= 8)
	{
		_d1 = (_v > 0)? (int64_t)255 & _v : -1;
		_v -= _d1;

	}
	else if (bit_width > 8 )
	{
		_d1 = (_v > 0)? BYTE_PTR_X(int64_t(_v) ) : -1;
		_v -= _d1;
	}
	
	
	return int64_t(_d1);
}



// fixed point numeric type
template <const unsigned BITS>
struct fixN
{
	fixN();
	fixN(const double);
	fixN(const int);

	void operator= (const double);
	void operator= (const int);
	const double operator()();
	const int operator()(const int) const;
	const double decimal();
	const double rational();

	operator int() const;
	operator double() const;

private:
	int _nVal;

	const int scale = BITS / 2;

	const int fXMask = ((int)std::pow(2, BITS) - 1) & ((int)std::pow(2, scale) - 1);
	const int iXMask = 0xFFFF << scale;

	const double toFix(const double);
	const double toDouble(const int);
	const int toFixInt(const int);
	const int fixtoInt(const int);
};




inline static char&& to_char(const int& _c)
{
	static char _ch = 0;
	const int _nc = _c;

	_ch = 0;

	if (_c >= 65 && _c <= 90)
		_ch = 90 - (90 - _nc);
	else if (_c >= 97 && _c <= 122)
		_ch = 122 - (122 - _nc);
	else
		_ch = _nc;


	return std::move(_ch);
}



inline static const bool is_alpha_num(const char& _chx)
{
	if (_chx >= '0' && _chx <= '9') return _BOOLC(1);
	else return _BOOLC(0);
}



inline static const int chartoint(const char& _ch)
{
	if (is_alpha_num(_ch))
		return _ch ^ 48;
	else return -1;
}



inline static std::string&& alphaNum2Bin(char&& _hxc)
{
	int _x = chartoint(_hxc);
	static std::string _hxfs ;
	
	_hxfs = "\0";
	_hxfs = std::string( (_x > 0)? bit_str(_x) : inttostr(0) );
		
	return std::move(_hxfs);
}



constexpr char&& upCase(int&& _c)
{
	if (_c <= 0) return _c;

	// is _c already in upper case??
	if (_c >= 65 && _c <= 90)
		return _c;

	
	int _ch = 0b00100000 ^ _c;

	return char(_ch);
}


constexpr char&& downCase(int&& _cAlpha)
{
	if (_cAlpha <= 0) return _cAlpha;

	// is _cAlpha already in lower case??
	if (_cAlpha >= 97 && _cAlpha <= 122)
		return _cAlpha;

	int _cLow = 0b01100000 | _cAlpha;

	return char(_cLow);
}


static inline int&& strPos(const char* _aStr, const char* _cStr)
{
	const std::size_t _Sz1 = std::strlen(_aStr), _Sz2 = std::strlen(_cStr);
	static int _Pos = 0;
	bool _bFound = false;

	_Pos = 0;

	if (!_Sz1 || !_Sz2) return std::move(_Pos);
	if (_Sz2 > _Sz1) return std::move(_Pos);

	for (std::size_t gf = 0; gf < _Sz1; gf++, _Pos++)
	{
		if (std::strncmp(&_aStr[gf], _cStr, _Sz2)) continue;
		else {
			_bFound = true;
			break;
		}
	}
	return (_bFound)? std::move(_Pos): std::move(- 1);
}



static inline int&& strNPos(const char* _StSrc, const int _chr)
{
	static int _iPos = 0;
	const int _maxSz = (int)std::strlen(_StSrc);
	bool _cFnd = false;

	_iPos = 0;

	for (_iPos = 0; _iPos < _maxSz; _iPos++)
	{
		if (_StSrc[_iPos] == _chr) {
			_cFnd = true;
			break;
		}
	}

	return (_cFnd)? std::move(_iPos) : std::move(- 1);
}



inline static std::string&& scanStr(const char* _Str0, const char* _searchStr)
{
	static std::string _SF = "\0";
	const size_t _SrcLen = std::strlen(_Str0), _SearchLen = std::strlen(_searchStr);

	_SF = "\0";

	if (!_Str0 || !_searchStr) return std::move(_SF);
	if (!_SrcLen || !_SearchLen) return std::move(_SF);
	if (_SearchLen > _SrcLen) return std::move(_SF);

	for (size_t _i = 0; _i < _SrcLen; _i++)
	{
		if (std::strncmp(&_Str0[_i], _searchStr, _SearchLen)) continue;
		else
		{
			_SF = std::string(std::strncpy(_SF.data(), &_Str0[_i], _SearchLen));
			break;
		}
	}
	return std::move(_SF);
}


inline static const char* concat_str(char* _target, const char* _str)
{
	const std::size_t lenz = std::strlen(_target),
	lenS = std::strlen(_str);

	char* _pStr = new char[lenz + lenS];

	if (_pStr) std::memset(_pStr, 0, lenz + lenS);

	std::strncpy(_pStr, _target, lenz);
	std::strncpy(&_pStr[lenz], _str, lenS);
	_pStr[lenz + lenS] = 0;

	return _pStr;
}


inline static const char* reverse_str(const char* _str)
{
	const int64_t lenz = std::strlen(_str), _max = lenz - 1;
	char* _ps = new char[lenz];

	for (int64_t i = 0,j = _max; j >= 0; )
		_ps[i++] = _str[j--];

	_ps[lenz] = 0;
	return _ps;
}


// take the n number of characters from the left end of the string
inline static std::string&& lstr(const char* _srcStr, const std::size_t& _nGrab)
{
	static std::string _str = "\0";
	const size_t lenMax = std::strlen(_srcStr);

	_str = "\0";

	if (_nGrab > lenMax || _nGrab <= 0) return std::move(_str);

	_str = std::string(std::strncpy(_str.data(), _srcStr, _nGrab));

	_str[_nGrab] = 0;
	return std::move(_str);
}



inline static std::string&& snapStr(const char* _srcStr, const size_t& _Start, const size_t& _End)
{
	static std::string _snpStr = "\0";

	_snpStr = "\0";

	if (!_srcStr) return std::move(_snpStr);

	int64_t _xBegin = (int64_t)_Start;

	if (_xBegin < 0 ) return std::move(_snpStr);

	const size_t SrcLen = std::strlen(_srcStr);

	if (_Start > SrcLen) return std::move(_snpStr);
	
	if (_End > SrcLen) return std::move(_snpStr);

	const size_t n_Snap = _Start + _End;

	if (n_Snap > SrcLen) return std::move(_snpStr);
	
	_snpStr = std::string(std::strncpy(_snpStr.data(), &_srcStr[_Start], _End));

	return std::move(_snpStr);
}


inline static std::string&& tapStr(const char* _pStr, const char& _tpChr, const size_t& _First, const size_t& _Count)
{
	static std::string _tpStr = "\0";

	_tpStr = "\0";

	if (!_pStr) return std::move(_tpStr);

	const size_t _SrcLen = std::strlen(_pStr);

	if ((int64_t)_First < 0 || _First > _SrcLen) return std::move(_tpStr);

	const size_t n_Tap = _First + _Count;

	if (n_Tap > _SrcLen) return std::move(_tpStr);

	_tpStr.assign(_pStr);

	for (size_t i = _First; i < n_Tap; i++) _tpStr[i] = _tpChr;


	return std::move(_tpStr);
}


static inline std::string&& tapStrBy(const char* _aStr, const char* _aSubstitute, const int& _startPos)
{
	static std::string _tappedStr = "\0";
	static std::string::iterator _Start, _End;
	const std::size_t _maxSz = std::strlen(_aStr),  _Len = std::strlen(_aSubstitute);
	
	_tappedStr = "\0"; 

	if (!_maxSz || !_Len) return std::move(_tappedStr);

	_tappedStr = std::string(std::strncpy(_tappedStr.data(), _aStr, _maxSz));

	_Start = _tappedStr.begin() + _startPos;
	_End = _tappedStr.begin() + _startPos + _Len;

	_tappedStr = std::string( _tappedStr.replace(_Start, _End, _aSubstitute) );
	
	return std::move(_tappedStr);
}


inline static std::string&& rtrimx(const char* _ssStr, const size_t& _Count, const char& _padC = ' ')
{
	static std::string _rtms = "\0";
	char* _Start = nullptr;

	_rtms = "\0";

	if (!_ssStr || (int64_t)_Count <= 0) return std::move(_rtms);

	const size_t _ssLen = std::strlen(_ssStr);
	const size_t _nPads = _ssLen - _Count;
	std::string _padStr = repl_char(char(_padC), _Count);

	_rtms = std::string(std::strncpy(_rtms.data(), _ssStr, _ssLen));

	_Start = &_rtms[_nPads];

	_Start = std::strncpy(_Start, _padStr.c_str(), _Count);


	return std::move(_rtms);
}


inline static std::string&& ltrimx(const char* _uStr, const size_t& _Count, const char& _padCh = ' ')
{
	static std::string _LStr = "\0";
	static std::string::iterator _LiBegin, _LiEnd;

	_LStr = "\0";

	if (!_uStr || (int64_t)_Count <= 0) return std::move(_LStr);

	const size_t _uMax = std::strlen(_uStr);

	_LStr = std::string(std::strncpy(_LStr.data(), _uStr, _uMax));

	for (_LiBegin = _LStr.begin(), _LiEnd = _LiBegin + _Count; _LiBegin < _LiEnd; _LiBegin++) *_LiBegin = _padCh;

	return std::move(_LStr);
}


// take the n number of characters from the right end of the string
inline static std::string&& rstr(const char* _sStr, const std::size_t& _nChars)
{
	static std::string _rStr = "\0";
	const size_t maxLen = std::strlen(_sStr);

	_rStr = "\0";

	if (_nChars > maxLen || _nChars <= 0) return std::move(_rStr);

	_rStr = std::string(std::strncpy(_rStr.data(), &_sStr[maxLen - _nChars], _nChars) );

	return std::move(_rStr);
}


inline static std::string&& LRTrim(const char* _Sstr)
{
	static std::string _LRTrmStr = "\0";
	static std::string::iterator _LRI;
	const size_t _nMax = std::strlen(_Sstr);
	size_t _i = 0, _iMax = 0;

	_LRTrmStr = "\0";

	if (!_Sstr || (int64_t)_nMax <= 0) return std::move(_LRTrmStr);

	std::string _tmpStr;

	_LRTrmStr = std::string(std::strncpy(_LRTrmStr.data(), _Sstr, _nMax));

	// Trimming the left spaces trail before the string
	for (_LRI = _LRTrmStr.begin(); _LRI != _LRTrmStr.end(); _LRI++)
	{
		if (*_LRI == SPACE) continue;
		else {
			_tmpStr = concat_str(_tmpStr.data(), _LRI._Ptr); break;
		}
	}

	// Trimming the right spaces trail after the string
	for (_iMax = _tmpStr.size(); _tmpStr[_iMax] == SPACE; _iMax--)
		_tmpStr[_iMax] = '\0';


	_LRTrmStr.assign(_tmpStr);

	return std::move(_LRTrmStr);
}




// bit status information
template <typename BitSZ = unsigned int>
struct bitInfo
{
	bitInfo(): X(0), _Alpha(0),numBits(0) {};

	bitInfo(char const _C, const int _x, const BitSZ _bitSz)
	{
		this->X = _x;
		this->_Alpha = _C;
		this->numBits = _bitSz;
	}

	~bitInfo() = default;
	
	operator char() const {
		return this->_Alpha;
	}

	operator int() const {
		return this->numBits;
	}

	int X;  
	char _Alpha;
	BitSZ numBits;
};


// a data structure of a Pair of Bit and Byte
struct BPAIR
{
	BPAIR() :_data(0), _val(0), bit_len(0) {};
	BPAIR(unsigned char&& _a) : _data(_a), _val(0), bit_len(0) {};

	BPAIR(intmax_t&& _v): _val(_v), _data(0), bit_len(len_bit(intmax_t(_val)))
	{
	};

	BPAIR(unsigned char&& _a, intmax_t&& _v) : _data(_a), _val(_v), bit_len( len_bit(intmax_t(_val) ) )
	{
	};

	BPAIR(intmax_t&& _v, unsigned char&& _a) :_data(_a), _val(_v), bit_len( len_bit(intmax_t(_val)) )
	{
	};

	~BPAIR() = default;

	BPAIR(BPAIR&& _mvBpa) noexcept
	{
		if (this == &_mvBpa) return;
		this->_data = _mvBpa._data;
		this->_val = _mvBpa._val;
		this->bit_len = len_bit(intmax_t(_val));

		_mvBpa._data = 0;
		_mvBpa._val = 0;
		_mvBpa.bit_len = 0;

	}

	BPAIR(const BPAIR& _rBpa)
	{
		if (this == &_rBpa) return;
		this->_data = _rBpa._data;
		this->_val = _rBpa._val;
		this->bit_len = len_bit(intmax_t(_val));
	}

	const BPAIR& operator= (const BPAIR& _bpa)
	{
		if (this == &_bpa) return *this;
		this->_data = _bpa._data;
		this->_val = _bpa._val;
		this->bit_len = len_bit(intmax_t(_val));

		return *this;
	}

	BPAIR&& operator= (BPAIR&& _rvBpa) noexcept
	{
		if (this == &_rvBpa) return std::move(*this);
		this->_data = _rvBpa._data;
		this->_val = _rvBpa._val;
		this->bit_len = len_bit(intmax_t(_val));

		_rvBpa._data = 0;
		_rvBpa._val = 0;
		_rvBpa.bit_len = 0;

		return std::move(*this);
	}

	operator unsigned char() const {
		return this->_data;
	}


	operator intmax_t() const {
		return this->_val;
	}

	unsigned char _data; // byte
	intmax_t _val , bit_len;  // encoded value & bit length
};



// Implementations to fixN Class..
template < const unsigned BITS >
fixN<BITS>::fixN(): _nVal(0){}


template < const unsigned BITS >
fixN<BITS>::fixN(const double x)
{
		*this = x;
}


template<const unsigned BITS>
fixN<BITS>::fixN(const int x)
{
		*this = x;
}


template < const unsigned BITS >
void fixN<BITS>::operator= (const double x)
{
	_nVal = (int)toFix(x);
}


template <const unsigned BITS >
void fixN<BITS>::operator= (int x)
{
	_nVal = toFixInt(x);
}
	

template <const unsigned BITS >
const double fixN<BITS>::operator()()
{
	return toDouble(_nVal);
}

	
template <const unsigned BITS >
const int fixN<BITS>::operator()(const int c) const
{
	return fixtoInt(_nVal);
}

	
template < const unsigned BITS >
const double fixN<BITS>::decimal()
{
	return toDouble((_nVal & iXMask ));
}


template <const unsigned BITS >
const double fixN<BITS>::rational()
{
	return  toDouble((_nVal & fXMask) );
}

	
template <const unsigned BITS>
const double fixN<BITS>::toFix(const double x)
{
	return x *(double)(1 << scale);
}


template <const unsigned BITS>
const double fixN<BITS>::toDouble(const int x)
{
	return (double)x / (double)(1 << scale);
}

	
template <const unsigned BITS>
const int fixN<BITS>::toFixInt(int x)
{
		return x << scale;
}


template <const unsigned BITS>
const int fixN<BITS>::fixtoInt(const int x)
{
		return x >> scale;
}


template <const unsigned BITS>
fixN<BITS>::operator double() const
{
	return toDouble(_nVal);
}


template <const unsigned BITS>
fixN<BITS>::operator int() const
{
	return toFixInt(_nVal);

}

// return how much number of decimal digits which appeared in a constant integer '_v'.
inline static const int64_t num_of_dec(const int64_t& _v)
{
	if (_v <= 0) return 0;

	int64_t _dec = _v, _count = 0, nMod=0;

	while (_dec > 0)
	{
		++_count;
		nMod = _dec % 10;
		_dec /= 10;
	}

	return _count;
}


inline static int64_t&& strtoint(std::string&& _sNum)
{
	const std::size_t _len = _sNum.size();
	char* _sf = new char[_len];
	std::string _sfs = "\0";
	intmax_t _bv = 0;
	static intmax_t _result = 0;

	_result = 0;

	std::memset(_sf, 0, _len);
	std::strncpy(_sf, _sNum.data(), _len);

	const intmax_t _iNum = strPos(_sNum.data(), "-");
	intmax_t _maxPos = (intmax_t)(_len - 1), _c = 0, _Exp = 0;

	if (_iNum >= 0)
	{
		_sf = std::strncpy(&_sf[1], &_sNum[1], _maxPos);
		_sf[_maxPos] = 0;
		--_maxPos;
	}

		for (intmax_t i = _maxPos; i >= 0; i--)
		{
			_c = _sf[i] ^ 0b00110000;
			_bv = _bv + _c * (intmax_t)std::pow(10, _Exp++);
		};

	
	_result = ((_iNum) >= 0)? _bv - (2 * _bv) : _bv;
	
	_sfs.clear();
	std::memset(_sf, 0, _len);
	return std::move(_result);
}


inline static std::string&& inttostr(const intmax_t& nVal)
{
	// max. spaces for negative integer
	const intmax_t nDigits = oneAdder( (unsigned int)num_of_dec((intmax_t)std::abs(nVal))); 

	// max. spaces for positive integer.
	const intmax_t nDecs = (nDigits > 1)? (nDigits - 1) : nDigits; 

	char _ch;  static std::string _ss = "\0";
	intmax_t nDiv = std::abs(nVal), _mod = 0, cnt = 0,decDigs = 0;

	std::string _tmpS = "\0";

	_ss = "\0";

	// if value is 0 (zero)
	if (!nDiv) {
		_ss = " ";
		std::memset(_ss.data(), 0, 1);
		_ss[0] = 48;
		_ss[1] = 0;
		return std::move(_ss);
	}

	// if value is negative
	if (nVal < 0) {
		_tmpS = repl_char(SPACE, nDigits);
		_ss.assign(_tmpS);
		std::memset(_ss.data(), 0, nDigits);
		_ss[0] = '-';
		decDigs = nDigits;
		cnt++;
	}
	else
	{
		_tmpS = repl_char(SPACE, nDecs);
		_ss.assign(_tmpS);
		std::memset(_ss.data(), 0, nDecs);
		decDigs = nDecs;
		cnt++;
	}
	

	while (nDiv > 0)
	{
		_mod = nDiv % 10;
		_ch = '0' + (char)_mod;
		_ss[decDigs - cnt] = _ch;
		nDiv /= 10; 
		cnt++;
	}

	_ss[decDigs] = 0;
	
	return std::move(_ss);
}


