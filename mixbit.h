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
static inline const int64_t count_bit_set(int64_t& _x)
{
	int64_t _rcx = 0;

	while (_x > 0)
	{
		if (_x % 2) ++_rcx;
		_x /= 2;
	}

	return _rcx;
}


// generates a digit '1' a number of '_reps' times
#define x1_bit(_ch_ptr, _reps)						\
{													\
	repl_char(_ch_ptr, '1', _reps);					\
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

const int64_t extract_Ints(int64_t&);
constexpr int64_t set_low_bit(const int64_t&);
constexpr int64_t range_bit_set(const int64_t&, const int64_t&);

// generates bit '0' a number of n_Bits
static const std::string zero_bits(const int64_t&);

// forward declaration prototype of repl_char() function
static const char* repl_char(char*&, char&&, const int&);

// Invoker macro for 'num_of_bits<T>::eval()'
static const int64_t _Get_Num_of_Bits(const int64_t&);

// Invoker macro for 'to_binary<T>::eval()'
static const std::string _Get_Binary_Str(const int64_t&);

// Invoker macro for 'bin_to_dec<T>::eval()'
static const int64_t _Int_from_Bit_Str(const std::string&);

// Invoker macro for extract_Ints
static const int64_t Ints_Of(int64_t& _ebx)
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
static const std::string cni_bits_pack(const std::vector<int64_t>&);

// saves a packed canonical bit to a one specified file
static const size_t save_cni_bit(const std::string&, const int64_t&);

// reads a packed canonical bit from one file and parses it to a vector integer
static const int read_cni_bit(const std::string&, std::vector<int64_t>&);

// the max. number of bits evaluated by 'BIT_TOKEN()'
unsigned int MAX_BIT = 0;


constexpr int64_t HI_HEX(const int64_t& _rbx)
{
	return (i4Mask << 4) & _rbx;
}

constexpr int64_t LO_HEX(const int64_t& _rbx)
{
	return (i4Mask & _rbx);
}

constexpr int64_t BYTE_PTR_HI(const int64_t& _rbx)
{
	return (i8Mask << 8) & _rbx;
}

constexpr int64_t BYTE_PTR_LO(const int64_t& rbx_)
{
	return 0xFF & rbx_;
}

constexpr int64_t WORD_PTR_HI(const int64_t& _rdx)
{
	return (i16Mask << 16) & _rdx;
}

constexpr int64_t WORD_PTR_LO(const int64_t& rdx_)
{
	return i16Mask & rdx_;
}

constexpr int64_t DWORD_PTR_HI(const int64_t& _rax)
{
	return (i32Mask << 32) & _rax;
}

constexpr int64_t DWORD_PTR_LO(const int64_t& rax_)
{
	return i32Mask & rax_;
}

// pulls off a byte from the MSB of a specified integer '_rcx'
const int64_t BYTE_PTR_X(const int64_t& _rcx)
{
	const int64_t _rdi = len_bit(_rcx);
	const int64_t _rsi = _rdi - 7;

	const int64_t _rgx = range_bit_set(_rsi, _rdi);
	return _rcx & _rgx;
}


 // pulls off a hex number from the MSB of a specified integer '_rex'
const int64_t HEX_PTR_X(const int64_t& _rex)
{
	const int64_t _rbx = len_bit(_rex);
	const int64_t _rax = _rbx - 3;
	const int64_t _rfx = range_bit_set(_rax, _rbx);
	return _rex & _rfx;
}

/* pulls off a series of bit from a specfied integer '_rax', starting from a specified bit position
   to a definite end bit position */
const int64_t ANY_PTR_X(const int64_t& _rax, const int64_t& _startBit, const int64_t& _endBit)
{
	const int64_t _rbx = len_bit(_rax);
	const int64_t& _rdi = _endBit;
	const int64_t& _rsi = _startBit;

	const int64_t _rfx = range_bit_set(_rsi, _rdi);
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
constexpr int64_t set_low_bit(const int64_t& _Max)
{
	int64_t _TotSum = 0;
	const int64_t _hi = _Max;

	for (int64_t _bi = _hi; _bi >= 0;  _bi--)
	{
		_TotSum += (int64_t)( 1 * std::pow(2, _bi) );
	}

	return _TotSum;
}



/* generates a range of set bits from the initial bit position to a determined bit position of a data unit.
   The indices of bit are based on the same rule as 'set_low_bit()'. (little-endian) 
   Eg. To specify the first 8 bits are set, you should call range_bit_set() with argument 0 
       as the min and 7 as the max argument.
   
*/
constexpr int64_t range_bit_set(const int64_t& _Min, const int64_t& _Max)
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

	return _Sum;
}


// get '_n_Bits' from the LSB of a specified integer ' _valX '.
constexpr int64_t get_n_of_lsb(const int64_t& _valX, const int64_t& _n_Bits)
{
	int64_t _v = 0;
	const int64_t _b = 0b1;

	for (int64_t _x = 0; _x < _n_Bits; _x++)
	{
		_v |= _valX & (_b << _x);
	}
	return _v;
}


// get 'N_Bits' from the MSB of a specified integer '_Vx'.
static inline const int64_t get_n_of_msb(const int64_t& _Vx, const int64_t& N_Bits)
{
	const int64_t _tot_Bits = (int64_t)_Get_Num_of_Bits(_Vx) - 1;
	const int64_t _rbx = (int64_t)0b1 << _tot_Bits;
	int64_t _rdx = _rbx;

	for (int64_t _n = 0; _n < N_Bits; _n++)
	{
		_rdx |= _Vx & (_rbx >> _n);
	}
	return _rdx;
}



// returns a specific named token which evaluates to a max. number of bits.
constexpr unsigned BIT_TOKEN(const int64_t& nBits)
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

	const unsigned numBits = (unsigned)std::strlen(_cBits.data());
	unsigned nZeros = BIT_TOKEN(numBits) - numBits;

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
inline static const char upCase(const int);

// downcase the specified char
inline static const char downCase(const int);

// scan a specific string pattern within the target string and return the found position in the target string.
inline static const int strPos(const char*, const char*);

// scan a byte value within the specified string pattern and return the found position in the string being scanned
inline static const int strNPos(const char*, const int);

// scan a substring within the target string and return the found substring in the target string.
inline static const char* scanStr(const char*, const char*);

inline static const char* concat_str(char* , const char*);

// take the n number of characters from the left end of the string
inline static const char* lstr(const char*, const std::size_t);

/* get a number of characters from a string starting from a position specified
   by '_start', then take n characters specified by '_nChars'. The position is
   zero-based array indexes.
*/
inline static const char* snapStr(const char*, const int, const int);

/* replace number of characters of a string with a character specified by '_tpChr' ,
   the position in the string is zero-based array indexes.
*/
inline static const char* tapStr(const char*, const char, const int, const int);

/* supersede part of a string with a specified substring at a specified position in the target string.
   Zero-based index array accesses is assumed */
inline static const char* tapStrBy(const char*, const char*, const int);

/* pad the right end of a string with number of unique characters specified by '_padC'.
   the '_Count' argument is based on zero index array accesses. */
inline static const char* rtrimx(const char*, const int, const char);

// removes extra spaces before and after the specified letter words
inline static const char* LRTrim(const char*);

/*pad the left end of a string with number of unique chars specified by '_padCh' ,
  using zero-based index array accesses. */
inline static const char* ltrimx(const char*, const int, const char _padCh);

// take the n number of characters from the right end of the string
inline static const char* rstr(const char*, const std::size_t);

// Convert alphanumeric string '0,1,2..9' to integer
inline static const int64_t strtoint(std::string&&);

inline static const char* inttostr(const int64_t&);

// return the exact number of bits that composing a value '_n'
inline static const unsigned proper_bits(const int64_t&);

// return the least significant portion of bits of a specified value '_v'
inline static const int64_t LoPart(const int64_t&);

// return the most significant portion of bits of a specified value '_v'
inline static const int64_t HiPart(const int64_t&);

// splits an integer into its composing bit factors and stored it to a vector
inline static void parseInt(int64_t&, std::vector<int64_t>&);

// parses each integer of a vector into its 8 bit composing binary form.
inline static void parseByte(std::vector<int>&, const std::vector<int64_t>&);

// merge the MSB and LSB portions together to form a single unit of data
inline static const int64_t MergeBits(const int64_t&, const int64_t&);

inline static const char* rtrim(const char*);

inline static const char* reverse_str(const char*);

// replicate the given char 'aChar' a number of '_repSize' times and stored it to the memory pointed to by '_dest'
inline static const char* repl_char(char*&, char&&, const int&);



inline static const char* repl_char(char*& _dest, char&& aChar, const int& _repSize)
{
	if (_dest == nullptr) _dest = new char[_repSize];
	char* _tmp = _dest;

	for (int i = 0; i < _repSize; i++)
		*_tmp++ = aChar;


	*_tmp++ = 0;
	NULLP(_tmp);

	return _dest;
}



static inline const std::string zero_bits(const int64_t& n_Bits)
{
	char* _Ch = new char[n_Bits];
	std::string _ci = repl_char(_Ch, '0', (int)n_Bits);

	return _ci;
}



// evaluate to how much number of bits that made up a constant value '_v'
template < class T >
struct num_of_bits
{
	using type = typename T;

	static const type eval(const type _v)
	{
		type cnt = 0;
		type _val = _v;

		while (_val > 0)
		{
			_val /= 2;
			++cnt;
		}

		return cnt;
	}
};



// convert a specified decimal constant to its binary form
template < typename T >
struct to_binary
{
	using value_type = typename T;

	static inline const std::string eval(const value_type _dec)
	{
		value_type _q = 0;
		value_type _bsz = num_of_bits<value_type>::eval(_dec);
		_value = _dec;

		if (_bsz > 0)
			_bs = new char[_bsz]; 

		for (value_type i = 0; i < _bsz && _value > 0; i++)
		{
			_q = _value % 2;
			_bs[i] = (_q) ? '1' : '0';
			_value = (_value > 1) ? _value /= 2 : 0;
		}

		_value = 0;
		_bs[_bsz] = 0;
		_bs = (char*)reverse_str(_bs.data());
		return _bs;
	}

private:
	static value_type _value;
	static std::string _bs;
};

// static members initializer
template <class T>
T to_binary<T>::_value = 0;

template <class T>
std::string to_binary<T>::_bs = "\0";


template <class T >
struct bin_to_dec
{
	using value_type = typename T;

	// the bit string is assumed to be in little-endian order.
	static inline const value_type eval(const std::string&& _strBits)
	{
		std::size_t lenMax = _strBits.size();
		std::size_t _maxBit = lenMax - 1;
		value_type k = 0, b = 0;

		_Dec = 0;

		for (std::size_t i = _maxBit; i > 0; i--)
		{
			b = (_strBits[i] == 49) ? 1 : 0;
			_Dec += b * (value_type)std::pow(2, k++);
		}

		b = (_strBits[0] == 49) ? 1 : 0;
		_Dec += b * (value_type)std::pow(2, _maxBit);

		return _Dec;
	}

private:
	static value_type _Dec;
};
// static member initializer
template <class T>
T bin_to_dec<T>::_Dec = 0;



static inline const int64_t _Get_Num_of_Bits(const int64_t& _ax)
{
	if (!_ax) return 1;
	else
		return num_of_bits<int64_t>::eval(_ax);
}


static inline const std::string _Get_Binary_Str(const int64_t& _Dx)
{
	const int64_t& _Abs = _Dx;
	std::string bit_str;

	if (!_Abs || _Abs < 0) return "0";
	else
		bit_str = to_binary<int64_t>::eval(_Abs).data();

	return bit_str;
}



static inline const int64_t _Int_from_Bit_Str(const std::string& Bit_Str)
{
	return bin_to_dec<int64_t>::eval(Bit_Str.data());
}


static inline const int64_t mix_integral_constant(const std::vector<int64_t>& v_Ints)
{
	int64_t rbx = 0, nShift = 0, bitsz = 0;
	const size_t v_size = v_Ints.size();

	for (size_t vi = 0; vi < v_size; vi++)
	{
		bitsz = len_bit(v_Ints[vi]);
		nShift = (bitsz <= 4)? 4 : bitsz;

		rbx = (rbx)? (rbx << nShift) | v_Ints[vi] : rbx | v_Ints[vi];
	}

	return rbx;
}



static inline void _Gen_Canonical_Info(std::vector<_Canonical>& _cBit, const std::vector<_Canonical>& _Codes)
{
	std::string _si;
	int64_t _len1 = 0, _len2 = 0, _bi = 0, _xDiff = 0;
	const std::size_t _codeSize = _Codes.size();
	
	_Canonical _Canon = _Codes[0];

	_len1 = _Canon._bitLen;

	_si = zero_bits(_len1);
	_bi = strtoint(_si.data());
	_Canon._codeWord = _bi;

	_cBit.push_back(_Canon); _Canon = {};

	for (size_t z = 1; z < _codeSize; z++)
	{
		_Canon = _cBit[z - 1];
		_len1 = _Get_Num_of_Bits(_Canon._codeWord);
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

		_cBit.push_back(_Canon );
		_Canon = {};
	}
}


static inline void cni_enforce_unique(std::vector<_Canonical>& cniDat)
{
	int64_t cw = cniDat[0]._codeWord, dw = 0, n_diff = 0;
	const size_t cnSize = cniDat.size();

	for (size_t j = 1; j < cnSize; j++)
	{
		dw = cniDat[j]._codeWord;

		if (cw == dw)
		{
			cniDat[j]._codeWord = cniDat[j - 1]._codeWord + 1;
		}
		else if (dw > cw)
		{
			n_diff = dw - cw;
			cniDat[j]._codeWord = dw - (n_diff / 2);
		}
		else if (dw < cw)
		{
			n_diff = cw - dw;
			cniDat[j]._codeWord = dw + (n_diff / 2);
		}
		cw = cniDat[j]._codeWord;
	}
}


static inline const std::string cni_bits_pack(const std::vector<int64_t>& _canVec)
{
	int64_t _x = 0;
	std::string _xBitStr;
	const size_t canSz = _canVec.size();

		for (size_t t = 0; t < canSz; t++)
		{
			_x <<= len_bit(_canVec[t]);
			_x |= _canVec[t];
		}
		

	_xBitStr = bit_str(_x);
	
	return _xBitStr;
}


static inline const size_t save_cni_bit(const std::string& _File, const int64_t& v_bit)
{
	size_t saved_size = 0;
	int i8_bit = 0,hi8_bit = 0, lo8_bit = 0;
	int64_t bit_cni = 0, b0Pos = 0, b1Pos = 0,biz_diff = 0, x_bit = v_bit;
	std::FILE* _FCni = std::fopen(_File.data(), "wb");

	const int64_t bit_size = len_bit(v_bit);	
	
	b0Pos = bit_size - 7; b1Pos = bit_size;

	const int64_t byte_size = (const int64_t)bit_size / 8;

	for (int64_t t = 0; t < byte_size; t++)
	{
		bit_cni = ANY_PTR_X(x_bit, b0Pos, b1Pos); x_bit -= bit_cni;

		while (len_bit(bit_cni) > 16) bit_cni >>= 8;

		if (len_bit(bit_cni) > 8) bit_cni >>= 4;

		std::fputc((int)bit_cni, _FCni); ++saved_size;

		b1Pos = --b0Pos; // back-skip the last position of bit series in the LSB
		b0Pos = b1Pos - 7;
	}

	bit_cni = x_bit;

	if (len_bit(bit_cni) <= 8) {
		std::fputc((int)bit_cni, _FCni); ++saved_size;
	}
	

	if (_FCni) std::fclose(_FCni);
	return saved_size;
}


static inline const int read_cni_bit(const std::string& _File, std::vector<int64_t>& Int_Bit)
{
	int read_bit = 0, read_size = 0;
	std::FILE* _FBit = std::fopen(_File.data(), "rb");

	if (!_FBit) return 0;

	while ((read_bit = std::fgetc(_FBit)) > -1 )
	{
		Int_Bit.push_back(read_bit);
		++read_size;
	}

	if (_FBit) std::fclose(_FBit);

	return read_size;
}



inline static const unsigned proper_bits(const int64_t& _n)
{
	const int64_t _nBits = num_of_bits<int64_t>::eval(_n);
	const unsigned int _maxBits = BIT_TOKEN(_nBits);

	return _maxBits;
}


inline static const int64_t LoPart(const int64_t& _v)
{
	const int64_t bit_width = len_bit(_v);
	const int64_t half_bit = bit_width / 2;
	const int64_t bit_order_difft = bit_width - half_bit;
	const int64_t low_mask = set_low_bit(bit_order_difft);
	const int64_t low_order_value = low_mask & _v;

	return low_order_value;
}


inline static const int64_t HiPart(const int64_t& _v)
{
	const int64_t bit_wide = len_bit(_v);
	const int64_t bit_half = bit_wide / 2;
	const int64_t bit_difft = bit_wide - bit_half;
	const int64_t bit_high_mask = range_bit_set(bit_half, bit_wide);
	const int64_t high_order_value = bit_high_mask & _v;
	
	return high_order_value;
}


inline static void parseInt(int64_t& _rdx, std::vector<int64_t>& _Ints)
{
	int64_t _rbx = 0, _rcx = 0;

	while ((_rbx = Ints_Of(_rdx)) > 0)
	{
		while ( (_rcx = len_bit(_rbx)) > 16 )
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
		_rdx = i64; _rcx = len_bit(_rdx);
		
		if (_rcx <= 8)
		{
			_dx = (int)_rdx;
			_iBytes.push_back(_dx);
		}
		else if (_rcx > 8 && _rcx <= 16)
		{
			_dh = (int)BYTE_PTR_HI(_rdx) >> 8; _iBytes.push_back(_dh);
			_dl = (int)BYTE_PTR_LO(_rdx); _iBytes.push_back(_dl);
		}
		else if (_rcx > 16 && _rcx <= 32)
		{
			_ax = (int)WORD_PTR_HI(_rdx) >> 16; _ah = (int)BYTE_PTR_HI((int64_t)_ax) >> 8; _al = (int)BYTE_PTR_LO((int64_t)_ax);
			_dx = (int)WORD_PTR_LO(_rdx);  _dh = (int)BYTE_PTR_HI((int64_t)_dx) >> 8; _dl = (int)BYTE_PTR_LO((int64_t)_dx);

			_iBytes.push_back(_ah); _iBytes.push_back(_al);
			_iBytes.push_back(_dh); _iBytes.push_back(_dl);

		}
		else if (_rcx > 32 && _rcx <= 64)
		{
			_eax = DWORD_PTR_HI(_rdx) >> 32;
			_ebx = DWORD_PTR_LO(_rdx);

			_ax = (int)WORD_PTR_HI(_eax) >> 16; _ah = (int)BYTE_PTR_HI((int64_t)_ax) >> 8; _al = (int)BYTE_PTR_LO((int64_t)_ax);
			_dx = (int)WORD_PTR_LO(_eax); _dh = (int)BYTE_PTR_HI((int64_t)_dx) >> 8; _dl = (int)BYTE_PTR_LO((int64_t)_dx);

			_iBytes.push_back(_ah); _iBytes.push_back(_al);
			_iBytes.push_back(_dh); _iBytes.push_back(_dl);

			_ax = (int)WORD_PTR_HI(_ebx) >> 16; _ah = (int)BYTE_PTR_HI((int64_t)_ax) >> 8; _al = (int)BYTE_PTR_LO((int64_t)_ax);
			_dx = (int)WORD_PTR_LO(_ebx); _dh = (int)BYTE_PTR_HI((int64_t)_dx) >> 8; _dl = (int)BYTE_PTR_LO((int64_t)_dx);

			_iBytes.push_back(_ah); _iBytes.push_back(_al);
			_iBytes.push_back(_dh); _iBytes.push_back(_dl);
		}

	}
}





inline static const int64_t MergeBits(const int64_t& _Hi, const int64_t& _Lo)
{
	int64_t _Bits = 0b0;

	_Bits = _Hi | _Lo;

	return _Bits;
}



/* extract the composing bit factors out of an integer '_v'.
*/
int64_t const extract_Ints(int64_t& _v)
{
	int64_t _d1 = -1, bit_width = 0;

	bit_width = len_bit(_v);

	if (bit_width <= 8)
	{
		_d1 = (_v > 0)? (int64_t)255 & _v : -1;
		_v -= _d1;

	}
	else if (bit_width > 8 )
	{
		_d1 = (_v > 0)? BYTE_PTR_X(_v) : -1;
		_v -= _d1;
	}
	
	
	return _d1;
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



inline static const char to_char(int const _c)
{
	char _ch = 0;
	const int _nc = _c;

	if (_c >= 65 && _c <= 90)
		_ch = 90 - (90 - _nc);
	else if (_c >= 97 && _c <= 122)
		_ch = 122 - (122 - _nc);
	else
		_ch = _nc;


	return _ch;
}



inline static const int to_int(char const _ch)
{
	return _ch;
}



inline static const char upCase(const int _c)
{
	if (_c <= 0) return _c;

	// is _c already in upper case??
	if (_c >= 65 && _c <= 90)
		return _c;

	
	int _ch = 0b00100000 ^ _c;

	return _ch;
}


inline static const char downCase(const int _cAlpha)
{
	if (_cAlpha <= 0) return _cAlpha;

	// is _cAlpha already in lower case??
	if (_cAlpha >= 97 && _cAlpha <= 122)
		return _cAlpha;

	int _cLow = 0b01100000 | _cAlpha;

	return _cLow;
}


inline static const int strPos(const char* _aStr, const char* _cStr)
{
	const std::size_t _Sz1 = std::strlen(_aStr), _Sz2 = std::strlen(_cStr);
	int _Pos = 0;
	bool _bFound = false;

	if (!_Sz1 || !_Sz2) return 0;
	if (_Sz2 > _Sz1) return -1;

	for (std::size_t gf = 0; gf < _Sz1; gf++, _Pos++)
	{
		if (std::strncmp(&_aStr[gf], _cStr, _Sz2)) continue;
		else {
			_bFound = true;
			break;
		}
	}
	return (_bFound)? _Pos: -1;
}



inline static const int strNPos(const char* _StSrc, const int _chr)
{
	int _iPos = 0;
	const int _maxSz = (int)std::strlen(_StSrc);
	bool _cFnd = false;

	for (_iPos = 0; _iPos < _maxSz; _iPos++)
	{
		if (_StSrc[_iPos] == _chr) {
			_cFnd = true;
			break;
		}
	}

	return (_cFnd)? _iPos : -1;
}



inline static const char* scanStr(const char* _Str0, const char* _searchStr)
{
	const std::size_t _lenZ = std::strlen(_Str0), _lenX = std::strlen(_searchStr);
	
	if (!_lenX || !_lenZ) return nullptr;
	if (_lenX > _lenZ) return nullptr;

	char* _SF = nullptr;

	for (std::size_t fi = 0; fi < _lenZ; fi++)
	{
		if (std::strncmp(&_Str0[fi], _searchStr, _lenX)) continue;
		_SF = (char*)&_Str0[fi];
		break;
	}

	return _SF;
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
inline static const char* lstr(const char* _srcStr, const std::size_t _nGrab)
{
	char* _ps = nullptr;
	const std::size_t lenX = std::strlen(_srcStr);

	if (_nGrab > lenX) return nullptr;

	_ps = new char[_nGrab];

	for (std::size_t i = 0; i < _nGrab; i++)
		_ps[i] = _srcStr[i];


	_ps[_nGrab] = 0;

	return _ps;
}


/* get a number of characters from a string starting at a position specified
   by '_start', then take n characters specified by '_nChars'. The position is
   zero-based array indexes.
*/
inline static const char* snapStr(const char* _srcStr, const int _start, const int _nChars)
{
	char* _snp = nullptr;
	const unsigned int _ssz = (unsigned int)std::strlen(_srcStr);
	const unsigned int _nGrab = (_start + _nChars) - 1; 

	if (_nGrab > _ssz) return nullptr;

	_snp = new char[_nGrab];

	std::memset(_snp, 0, (std::size_t)_nGrab);

	for (int j = 0, i = _start; j < _nChars; i++, j++)
		_snp[j] = _srcStr[i];

	_snp[_nGrab] = 0;

	return _snp;
}


/* replace number of characters of a string with a character specified by '_tpChr' ,
   the position in the string is zero-based array indexes. 
*/
inline static const char* tapStr(const char* _pStr, const char _tpChr, const int _First, const int _Count)
{
	char* _tpStr = nullptr;
	const unsigned int _lenT = (unsigned int)std::strlen(_pStr);
	const unsigned int _nTap = (_First + _Count) - 1; 

	if (_nTap > _lenT) return nullptr;

	_tpStr = new char[_lenT];

	std::strncpy(_tpStr, _pStr, (std::size_t)_lenT);

	for (unsigned int j = _First; j <= _nTap; j++)
		_tpStr[j] = _tpChr;

	_tpStr[_lenT] = 0;

	return _tpStr;
}


static inline const char* tapStrBy(const char* _aStr, const char* _aSubstitute, const int _startPos)
{
	const std::size_t _maxSz = std::strlen(_aStr),  _Len = std::strlen(_aSubstitute);

	char* _begin = (char*)_aStr, *_end = (char*)((_aStr + _maxSz) - 1);
	
	char* _bckup = _begin;

	_bckup += (_startPos + 1);

	if (_bckup > _end)
		_bckup = _begin;

	std::strncpy(_bckup, _aSubstitute, _Len);
	*(_end + 1) = 0;

	NULL2P(_begin, _end);
	NULLP(_bckup);

	return _aStr;
}

/* pad the right end of a string with number of unique characters specified by '_padC'.
   the '_Count' argument is based on zero index array accesses. */
inline static const char* rtrimx(const char* _ssStr, const int _Count, const char _padC = ' ')
{
	char* _rtms = nullptr;
	const unsigned int _ssLen = (unsigned int)std::strlen(_ssStr);
	const unsigned int _rStart = _ssLen - 1;
	int _nPads = (int)(_ssLen - _Count); 

	if ((unsigned int)_Count > _ssLen) return nullptr;

	_rtms = new char[_ssLen];

	std::strncpy(_rtms, _ssStr, (std::size_t)_ssLen);

	for (int r = _rStart; r >= _nPads; r--)
		_rtms[r] = _padC;


	_rtms[_ssLen] = 0;

	return _rtms;
}


/*pad the left end of a string with number of unique chars specified by '_padCh' ,
  using zero-based index array accesses. */
inline static const char* ltrimx(const char* _uStr, const int _Count, const char _padCh = ' ')
{
	const unsigned int lenSt = (unsigned int)std::strlen(_uStr);
	char* _lPadStr = new char[lenSt];

	if ((unsigned int)_Count > lenSt) return nullptr;

	std::strncpy(_lPadStr, _uStr, lenSt);

	for (int i = 0; i < _Count; i++)
		_lPadStr[i] = _padCh;

	_lPadStr[lenSt] = 0;
	return _lPadStr;
}


// take the n number of characters from the right end of the string
inline static const char* rstr(const char* _sStr, const std::size_t _nChars)
{
	const std::size_t lenR = std::strlen(_sStr) - 1;
	char* _pss = nullptr;

	if (_nChars == 0 || _nChars > lenR) return nullptr;

	_pss = new char[_nChars];

	for (std::size_t j = 0; j < _nChars; j++)
		_pss[j] = _sStr[lenR - j];

	
	_pss[_nChars] = 0;

	return _pss;
}


inline static const char* LRTrim(const char* _Sstr)
{
	const int _maxLen = (int)std::strlen(_Sstr);
	char* _newSt = (char*)"";
	int _jPos = 0, _k = 0;

	// skipping through the first blank character spaces
	for (_jPos = 0; _jPos < _maxLen; _jPos++)
	{
		if (_Sstr[_jPos] == 32) continue;
		else break;
	}

	for (; _jPos < _maxLen; _jPos++, _k++)
		_newSt = (char*)concat_str(_newSt, &_Sstr[_jPos]);


	_newSt[_k] = 0;
	return _newSt;
}


inline static const char* rtrim(const char* _string)
{
	const std::size_t Len = std::strlen(_string), _Max = Len - 1;
	char* _bss = new char[_Max];

	std::memset(_bss, 0, _Max);
	std::strncpy(_bss, _string, _Max);
	_bss[_Max] = 0;
	
	return _bss;
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
	BPAIR() :_data('0'), _val(0), bit_len(0) {};
	BPAIR(const char _a) : _data(_a), _val(0), bit_len(0) {};

	BPAIR(const int64_t _v) : _val(_v), _data('0'), bit_len(_Get_Num_of_Bits(_v))
	{
	
	};

	BPAIR(const char _a, const int64_t _v) : _data(_a), _val(_v), bit_len(_Get_Num_of_Bits(_v) ) 
	{
	
	};

	BPAIR(const int64_t _v, const char _a) :_data(_a), _val(_v), bit_len(_Get_Num_of_Bits(_v) ) 
	{
		
	};

	~BPAIR() = default;

	BPAIR(BPAIR&& _mvBpa)
	{
		if (this == &_mvBpa) return;
		*this = std::move(_mvBpa);
	}

	BPAIR(const BPAIR& _rBpa)
	{
		if (this == &_rBpa) return;
		*this = _rBpa;
	}

	const BPAIR& operator= (const BPAIR& _bpa)
	{
		if (this == &_bpa) return *this;
		this->_data = _bpa._data;
		this->_val = _bpa._val;
		this->bit_len = _Get_Num_of_Bits(_bpa._val);

		return *this;
	}

	BPAIR&& operator= (BPAIR&& _rvBpa) noexcept
	{
		if (this == &_rvBpa) return std::move(*this);
		this->_data = _rvBpa._data;
		this->_val = _rvBpa._val;
		this->bit_len = _Get_Num_of_Bits(_rvBpa._val);

		_rvBpa.~BPAIR();

		return std::move(*this);
	}

	operator char() const {
		return this->_data;
	}


	operator int64_t() const {
		return this->_val;
	}

	char _data; // byte
	int64_t _val /* bit */, bit_len;
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


inline static const int64_t strtoint(std::string&& _sNum)
{
	const std::size_t _len = _sNum.size();
	char* _sf = new char[_len];
	std::string _sfs;
	int64_t _bv = 0;

	std::memset(_sf, 0, _len);
	std::strncpy(_sf, _sNum.data(), _len);

	int64_t _iNum = strPos(_sNum.data(),"-"),
		_maxPos = (int64_t)(_len - 1), 
		_c = 0, _result = 0,
		_Exp = 0;

	if (_iNum >= 0)
	{
		_sf = std::strncpy(&_sf[1], &_sNum[1], _maxPos);
		_sf[_maxPos] = 0;
		--_maxPos;
	}

		for (int64_t i = _maxPos; i >= 0; i--)
		{
			_c = _sf[i] ^ 0b00110000;
			_bv = _bv + _c * (int64_t)std::pow(10, _Exp++);
		};

	
	_result = ((_iNum) >= 0)? _bv - (2 * _bv) : _bv;
	
	_sfs.clear();
	std::memset(_sf, 0, _len);
	return _result;
}


inline static const char* inttostr(const int64_t& nVal)
{
	// max. spaces for negative integer
	const int64_t nDigits = oneAdder( (UINT)num_of_dec((int64_t)std::abs(nVal))); 

	// max. spaces for positive integer.
	const int64_t nDecs = (nDigits > 1)? (nDigits - 1) : nDigits; 

	char _ch;  char* _ss = nullptr;
	int64_t nDiv = std::abs(nVal), _mod = 0, cnt = 0,decDigs = 0;


	// if value is 0 (zero)
	if (!nDiv) {
		_ss = new char[1];
		std::memset(_ss, 0, 1);
		_ss[0] = 48;
		_ss[1] = 0;
		return _ss;
	}

	// if value is negative
	if (nVal < 0) {
		_ss = new char[nDigits];
		std::memset(_ss, 0, nDigits);
		_ss[0] = '-';
		decDigs = nDigits;
		cnt++;
	}
	else
	{
		_ss = new char[nDecs];
		std::memset(_ss, 0, nDecs);
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
	
	return _ss;
}




