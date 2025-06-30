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
	int _bitLen;
	int _codeWord;
	int _rle_bit_len;

	operator int() {
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

	operator int() {
		return this->_codeWord;
	}
};


#define SPACE (char)32
#define halfSz(_Tot_) (_Tot_ / 2) - 1


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

constexpr int i4Mask = 0xF;
constexpr int i8Mask = 255;
constexpr int i16Mask = 65535;
constexpr int i32Mask = 0xFFFFFFFF;

const int extract_byte(const int&);

// generates bit '0' a number of n_Bits
static const std::string zero_bits(const int&);

// forward declaration prototype of repl_char() function
static const char* repl_char(char*, const char&, const int&);

// Invoker macro for 'num_of_bits<T>::eval()'
static const int _Get_Num_of_Bits(const int&);

// Invoker macro for 'to_binary<T>::eval()'
static const std::string _Get_Binary_Str(const int&);

// Invoker macro for 'bin_to_dec<T>::eval()'
static const int _Int_from_Bit_Str(const std::string&);

// Invoker macro for extract_byte
static const int byte_of(const int& _ebx)
{
	return extract_byte(_ebx);
}

// generate encoding information table based on Huffman Canonical Method
static void _Gen_Canonical_Info(std::vector<int>&, const std::vector<int>&);

// packing a series of canonical bits into one integer and return the result as a bits string.
static const std::string cni_bits_pack(const std::vector<int>&);

// saves a packed canonical bit to one specified file
static const int save_cni_bit(const std::string&, const int&);

// reads a packed canonical bit from one file and parses it to a vector integer
static const int read_cni_bit(const std::string&, std::vector<int>&);

// merges the read integers into one single packed canonical bit again
static const size_t merge_cni_bit(const std::vector<int>&, int&);

// the max. number of bits evaluated by 'BIT_TOKEN()'
unsigned int MAX_BIT = 0;


constexpr int HI_HEX(const int _Bx)
{
	return (i4Mask << 4) & _Bx;
}

constexpr int LO_HEX(const int _Bx)
{
	return (i4Mask & _Bx);
}

constexpr int BYTE_PTR_HI(const int _Bx)
{
	return (i8Mask << 8) & _Bx;
}

constexpr int BYTE_PTR_LO(const int Bx_)
{
	return 0xFF & Bx_;
}

constexpr int WORD_PTR_HI(const int _EDX)
{
	return (i16Mask << 16) & _EDX;
}

constexpr int WORD_PTR_LO(const int EDX_)
{
	return i16Mask & EDX_;
}


/* generates number of set bits on the LSB of a data unit. (little - endian)
   NB: bit indices are specified based on average user habitual convention style. */
constexpr int set_low_bit(const int& _Max_to_Zero)
{
	int _TotSum = 0;
	const double _hi = (double)(_Max_to_Zero - 1);

	for (double _bi = _hi; _bi >= 0;  _bi--)
	{
		_TotSum += (int)( 1 * std::pow(2, _bi) );
	}

	return (int)_TotSum;
}



/* generates a range of set bits from the initial bit position to a determined bit position of a data unit.
   The indices of bit are specified using average user habitual convention style. (little-endian) */
constexpr int range_bit_set(const int& _Min, const int& _Max)
{
	double _Sum = 0;
	const double _start_bit = (double)(_Min - 1), _end_bit = (double)(_Max - 1);

	for (double _bi = _start_bit; _bi <= _end_bit; _bi++)
	{
		_Sum += (int)(1 * std::pow(2, _bi) );
	}

	return (int)_Sum;
}


// get '_n_Bits' from the LSB of a specified integer ' _valX '.
constexpr int get_n_of_lsb(const int& _valX, const int& _n_Bits)
{
	int _v = 0;
	const int _b = 0b1;

	for (int _x = 0; _x < _n_Bits; _x++)
	{
		_v |= _valX & (_b << _x);
	}
	return _v;
}


// get 'N_Bits' from the MSB of a specified integer '_Vx'.
static inline const int get_n_of_msb(const int& _Vx, const int& N_Bits)
{
	const int _tot_Bits = _Get_Num_of_Bits(_Vx) - 1;
	const int _bx = 0b1 << _tot_Bits;
	int _dx = _bx;

	for (int _n = 0; _n < N_Bits; _n++)
	{
		_dx |= _Vx & (_bx >> _n);
	}
	return _dx;
}



// returns a specific named token which evaluates to a max. number of bits.
constexpr unsigned BIT_TOKEN(const unsigned nBits)
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


// 32-Bit Data Ordering structure
struct _32Bit
{
	_32Bit() : AX(0) {};
	_32Bit(const int _Ex) { this->setValue(_Ex); };

	const int Byte() const;

	const int32_t LoWord() const;
	const int32_t HiWord() const;

	const int16_t LoByte() const;
	const int16_t HiByte() const;

	const int Value() const;
	const int BitLength() const;

	void setValue(const int);
	
private:
	int AX = 0;
	int EAX[4] = { 0,0,0,0 };
};



struct BitN
{
	BitN();
	BitN(const int);
	BitN(const char*);
	BitN(const std::initializer_list<bool>&);

	void setBits(const std::string&&);
	const int value_from_bitstr(const std::string&);
	const std::string& Bits();
	const std::string& toBits(const unsigned);
	const int value_from_bitlist(const iList2<bool>&);
	const std::string& to_signed_bits(const int);
	const std::string& to_fixed_point_bits(const std::string&, const double);
	const std::size_t bitSize() const;
	void operator()() const;

private:
	std::string _bitStr;
	std::size_t _bitLen;
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
inline static const int strtoint(std::string&&);

inline static const char* inttostr(const int);

// return the exact number of bits that composing a value '_n'
inline static const unsigned int proper_bits(const int);

// return the least significant portion of bits of a specified value '_v'
inline static const int LoPart(const int);

// return the most significant portion of bits of a specified value '_v'
inline static const int HiPart(const int);

// extract specific portion of each data with [BYTE PTR] attribute and store it to the Vector
inline static void parseByte(const int, std::vector<int>&);

// merge the MSB and LSB portions together to form a single unit of data
inline static const int32_t MergeBits(const int, const int);

inline static const char* rtrim(const char*);

inline static const char* reverse_str(const char*);

// replicate the given char 'aChar' a number of '_repSize' times and stored it to the memory pointed to by '_dest'
inline static const char* repl_char(char*, const char&, const int&);

// decomposes a given packed int into its original bit form
inline static const int unPack(const int&, int const&, int const&);


inline static const char* repl_char(char* _dest, const char& aChar, const int& _repSize)
{
	_dest = new char[_repSize];
	std::memset(_dest, 0, _repSize);

	for (int i = 0; i < _repSize; i++)
		_dest[i] = aChar;


	_dest[_repSize] = 0;
	return _dest;
}



static inline const std::string zero_bits(const int& n_Bits)
{
	char* _Ch = new char[n_Bits];
	std::string _ci = repl_char(_Ch, '0', n_Bits);

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
			_val >>= 1;
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
		int _bsz = num_of_bits<unsigned int>::eval(_dec);
		_value = _dec;

		if (_bsz > 0)
			_bs = new char[_bsz]; 

		for (int i = 0; i < _bsz; i++)
		{
			_bs[i] = (_value % 2)? 49 : 48;
			_value >>= 1;
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
		int k = 0, b = 0;

		_Dec = 0;

		for (std::size_t i = _maxBit; i > 0; i--)
		{
			b = (_strBits[i] == 49)? 1 : 0;
			_Dec += b * (int)std::pow(2, k++);
		}

		b = (_strBits[0] == 49)? 1 : 0;
		_Dec += b * (int)std::pow(2, _maxBit);

		return _Dec;
	}

private:
	static value_type _Dec;
};
// static member initializer
template <class T>
T bin_to_dec<T>::_Dec = 0;



static inline const int _Get_Num_of_Bits(const int& _ax)
{
	if (!_ax) return 1;
	else
	  return num_of_bits<int>::eval(_ax);
}


static inline const std::string _Get_Binary_Str(const int& _Dx)
{
	int _Abs = _Dx;
	std::string bit_str;

	if (_Abs < 0)  _Abs = std::abs(_Abs);

	if (!_Abs) return "0";
	else
	  bit_str = to_binary<int>::eval(_Abs).data();

	return bit_str;
}



static inline const int _Int_from_Bit_Str(const std::string& Bit_Str)
{
	return bin_to_dec<int>::eval(Bit_Str.data());
}



static inline void _Gen_Canonical_Info(std::vector<_Canonical>& _cBit, const std::vector<_Canonical>& _Codes)
{
	std::string _si;
	int _len1 = 0, _len2 = 0, _bi = 0, _xDiff = 0;
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


static inline const std::string cni_bits_pack(const std::vector<int>& _canVec)
{
	int _x = 0b0;
	std::string _xBitStr;
	const std::size_t canSize = _canVec.size();

	for (size_t t = 0; t < canSize; t++)
	{
		_x <<= _Get_Num_of_Bits(_canVec[t]);
		_x |= _canVec[t];
	}

	_xBitStr = concat_str((char*)zero_bits(1).data(), _Get_Binary_Str(_x).data());
	
	return _xBitStr;
}


static inline const int save_cni_bit(const std::string& _File, const int& cni_bit)
{
	int saved_size = 0;
	int bit_cni = 0, bit_size = 0;
	std::FILE* _FCni = std::fopen(_File.data(), "wb");

	if (!_FCni) return 0;

	while ((bit_cni = byte_of(cni_bit)) > 0)
	{
		while ((bit_size = _Get_Num_of_Bits(bit_cni)) > 16)
			bit_cni >>= 16;

		std::fputc(BYTE_PTR_HI(bit_cni) >> 8, _FCni);
		std::fputc(BYTE_PTR_LO(bit_cni), _FCni);
		saved_size += 2;
	}

	if (_FCni) std::fclose(_FCni);
	
	return saved_size;
}


static inline const int read_cni_bit(const std::string& _File, std::vector<int>& Int_Bit)
{
	int read_bit = 0;
	int read_size = 0;
	std::FILE* _FBit = std::fopen(_File.data(), "rb");

	if (!_FBit) return 0;

	while ((read_bit = std::fgetc(_FBit)) > 0 )
	{
		Int_Bit.push_back(read_bit);
		read_bit = 0;
		++read_size;
	}

	if (_FBit) std::fclose(_FBit);

	return read_size;
}


static inline const size_t merge_cni_bit(const std::vector<int>& v_bit, int& bit_cni)
{
	size_t merged_size = 0;
	const size_t vbnSize = v_bit.size(), bit_size = vbnSize * 8;

	if (bit_size <= 8) bit_cni = v_bit[0];

	else if (bit_size > 8 && bit_size <= 16 )
	{
		bit_cni |= v_bit[0] << 8;
		bit_cni |= v_bit[1];
	}
	else if (bit_size > 16 && bit_size <= 32)
	{
		bit_cni |= v_bit[0] << 24;
		bit_cni |= v_bit[1] << 16;
		bit_cni |= v_bit[2] << 8;
		bit_cni |= v_bit[3];
	}

	return merged_size;
}



inline static const unsigned int proper_bits(const int _n)
{
	const unsigned int _nBits = num_of_bits<int>::eval(_n);
	const unsigned int _maxBits = BIT_TOKEN(_nBits);

	return _maxBits;
}


inline static const int LoPart(const int _v)
{
	int _Result = 0b0;
	
	MAX_BIT = proper_bits(_v);

	switch (MAX_BIT)
	{
		case 8:
			_Result = (i4Mask & _v);
			break;

		case 16:
			_Result = (i8Mask & _v);
			break;

		case 32:
			_Result = (i16Mask & _v);
			break;

		default:
			break;
	}

	return _Result;

}


inline static const int HiPart(const int _v)
{
	int _ResX = 0b0;

	MAX_BIT = proper_bits(_v);

	switch (MAX_BIT)
	{
	case 8:
		_ResX = (i4Mask << 4) & _v;
		break;

	case 16:
		_ResX = (i8Mask << 8) & _v;
		break;

	case 32:
		_ResX = (i16Mask << 16) & _v;
		break;
	}

	return _ResX;
}


inline static void parseByte(const int _EDX, std::vector<int>& _Bytes)
{
	int _Val = 0;

	_Val = WORD_PTR_HI(_EDX);

	if (_Val)
	{
		_Val >>= 16;
		_Bytes.push_back(_Val);
	}

	_Val = HiPart(WORD_PTR_LO(_EDX) );

	if (_Val)
	{
		_Val >>= 8;
		_Bytes.push_back(_Val);
	}

	_Val = LoPart(WORD_PTR_LO(_EDX));

	if (_Val) _Bytes.push_back(_Val);
}


inline static const int32_t MergeBits(const int _Hi, const int _Lo)
{
	int32_t _Bits = 0b0;

	_Bits = _Hi | _Lo;

	return _Bits;
}



/* extract byte by byte portion of an integer '_v'.
   If '_v' is more than a 16 bit wide integer, then
   extract_byte will successively extract each 16 bit
   portion of its high & low order part, otherwise 
   '_v' is extracted per byte by byte basis.
*/
int const extract_byte(const int& _v)
{
	static int _v1 = 0;
	static int _count = 0;

	MAX_BIT = proper_bits(_v);

	if (MAX_BIT <= BYTE)
	{
		_v1 = (_count)? 0 : 255 & _v;
		++_count;

		return _v1;
	}
	else if (MAX_BIT > WORD)
	{
		if (_count > 1)
		{
			_count = 0;
			_v1 = 0;
			goto endResult;
		}
		
		_v1 = (_count > 0)? (65535 & _v) : WORD_PTR_HI(_v);
		++_count;

		endResult:
			return _v1;
	}

	switch (_count)
	{
	case 0:
		_v1 = (255 << 8) & _v;
		++_count;
		break;

	case 1:
		_v1 = _v - _v1;
		++_count;
		break;

	case 2:
		_v1 = 0;
		_count = 0;
	default:
		break;
	}

	return _v1;
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

	char* _SF = new char[_lenX];
	std::memset(_SF, 0, _lenX);

	for (std::size_t fi = 0; fi < _lenZ; fi++)
	{
		if (std::strncmp(&_Str0[fi], _searchStr, _lenX)) continue;
		_SF = std::strncpy(_SF, &_Str0[fi], _lenX);
		break;
	}

	_SF[_lenX] = 0;
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
	const std::size_t lenz = std::strlen(_str), _max = lenz - 1;
	char* _ps = new char[lenz];

	for (int i = 0,j = (int)_max; j >= 0; )
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


// a data structure of a packed data information
struct packed_info
{
	packed_info() :
		_BITLEN(0), _PACKED(0), L_BIT(0), R_BIT(0), _Reg{ _Int_Bits() }
	{ };

	packed_info(packed_info const& _rPif)
	{
		if (this == &_rPif) return;
		*this = _rPif;
	}

	packed_info const& operator= (packed_info const& _rPif)
	{
		if (this == &_rPif) return *this;

		this->_BITLEN = _rPif._BITLEN;
		this->_PACKED = _rPif._PACKED;
		this->L_BIT = _rPif.L_BIT;
		this->R_BIT = _rPif.R_BIT;
		this->_Reg = _rPif._Reg;

		return *this;
	}


	~packed_info()
	{

	}

	// the total bit length of a packed integer value.
	int _BITLEN;

	// the first & second char packed into one integer value.
	int _PACKED;

	// the number of encoded bits on the MSB part of the _PACKED.
	int L_BIT;  
	
	// the number of encoded bits on the LSB part of the _PACKED.
	int R_BIT;

	// a member for represent the complete bits of _PACKED.
	_Int_Bits _Reg;
};



// a data structure of a Pair of Bit and Byte
struct BPAIR
{
	BPAIR() :_data('0'), _val(0), bit_len(0) {};
	BPAIR(const char _a) : _data(_a), _val(0), bit_len(0) {};

	BPAIR(const int _v) : _val(_v), _data('0'), bit_len(_Get_Num_of_Bits(_v))
	{
	
	};

	BPAIR(const char _a, const int _v) : _data(_a), _val(_v), bit_len(_Get_Num_of_Bits(_v) ) 
	{
	
	};

	BPAIR(const int _v, const char _a) :_data(_a), _val(_v), bit_len(_Get_Num_of_Bits(_v) ) 
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


	operator int() const {
		return this->_val;
	}

	char _data; // byte
	int _val /* bit */, bit_len;
};



// pack the entire bits of the vector into a 'BPAIR' vector
template < typename T >
static inline void bitsPack(std::vector<BPAIR>& _packed, const std::vector<bitInfo<T>>& _vb)
{
	int _bx = 0b0, _Ax = 0b0;
	const std::size_t _vcSz = _vb.size(), _nIter = 1;
	const std::size_t _bpSz = _packed.size();

	std::size_t _loopn = 0;

	T _n = 0;

	if (_bpSz < _vcSz)
		_packed.resize(_bpSz + (_vcSz - _bpSz));
	

	for (std::size_t i = 0; i < _vcSz; i++)
	{
		_bx = _vb[i].X;
		_n = _vb[i].numBits;

		_Ax <<= _n;
		_Ax |= _bx;

		_loopn++;

		if ((_loopn > _nIter) || ((i + 1) >= _vcSz))
		{
			_Ax = 0b0;
			_loopn = 0;
		}
	}
}


inline static const int unPack(const int& _Packed, int const& _leftBits, int const& _rightBits)
{
	MAX_BIT = proper_bits(_Packed);
	const int bit_count = MAX_BIT - _rightBits;
	const int _hiMsk = range_bit_set(_rightBits, _rightBits + bit_count);
	const int _LeftUnPacked = (_hiMsk & _Packed) >> _rightBits;

	return _LeftUnPacked;
}



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
inline static const int num_of_dec(const int _v)
{
	if (_v <= 0) return 0;

	int _dec = _v, _count = 0, nMod=0;

	while (_dec > 0)
	{
		++_count;
		nMod = _dec % 10;
		_dec /= 10;
	}

	return _count;
}


inline static const int strtoint(std::string&& _sNum)
{
	const std::size_t _len = _sNum.size();
	char* _sf = new char[_len];
	std::string _sfs;
	int _bv = 0;

	std::memset(_sf, 0, _len);
	std::strncpy(_sf, _sNum.data(), _len);

	int _iNum = strPos(_sNum.data(),"-"),
		_maxPos = (int)(_len - 1), 
		_c = 0, _result = 0,
		_Exp = 0;

	if (_iNum >= 0)
	{
		_sf = std::strncpy(&_sf[1], &_sNum[1], _maxPos);
		_sf[_maxPos] = 0;
		--_maxPos;
	}

		for (int i = _maxPos; i >= 0; i--)
		{
			_c = _sf[i] ^ 0b00110000;
			_bv = _bv + _c * (int)std::pow(10, _Exp++);
		};

	
	_result = ((_iNum) >= 0)? _bv - (2 * _bv) : _bv;
	
	_sfs.clear();
	std::memset(_sf, 0, _len);
	return _result;
}


inline static const char* inttostr(const int nVal)
{
	// max. spaces for negative integer
	const int nDigits = oneAdder(num_of_dec(std::abs(nVal))); 

	// max. spaces for positive integer.
	const int nDecs = (nDigits > 1)? (nDigits - 1) : nDigits; 

	char _ch;  char* _ss = nullptr;
	int nDiv = std::abs(nVal), _mod = 0, cnt = 0,decDigs = 0;


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



// Implementation to BitN class..

	BitN::BitN():_bitLen(0) {}

	BitN::BitN(const int _val)  
	{
		_bitStr = (_val > 0)? toBits(_val) : to_signed_bits(_val);
		
	}

	BitN::BitN(const char* _strFixBits)
	{
		to_fixed_point_bits(_strFixBits, 0.1);
	}

	
	BitN::BitN(const std::initializer_list<bool>& _bitL) : _bitLen(0)
	{

		toBits(value_from_bitlist(_bitL) );
	}


	void BitN::setBits(const std::string&& _bitL)
	{
		_bitStr = _bitL;
		_bitLen = _bitStr.size();
	}


	const int BitN::value_from_bitstr(const std::string& _bits)
	{
		int _v = bin_to_dec<int>::eval(_bits.data());

		return _v;
	}


	const std::string& BitN::Bits()
	{
		return _bitStr;
	}


	const std::string& BitN::toBits(const unsigned _val)
	{
		unsigned _dec = _val;

		_bitStr.clear(); 

		_bitStr = to_binary<unsigned int>::eval(_val);
		
		_bitLen = _bitStr.size();
		return _bitStr;
	}


	const int BitN::value_from_bitlist(const iList2<bool>& _bits)
	{
		int _bExp = 0, _v = 0;
		_bExp = (int)(_bits.size() - 1); 

		for (iList2<bool>::iterator _Bit = _bits.begin(); _Bit != _bits.end(); _Bit++)
		{
			_v += (*_Bit) * (0b1 << _bExp--);
		}
		RET;

		return _v;
	}


	// obtain a 2-complement bits string from a specific signed value ( + / - )
	const std::string& BitN::to_signed_bits(const int _signed_v)
	{
		_bitStr.clear();
		bool* _bits = nullptr;
		unsigned _v = 0;

		unsigned _dec = (_signed_v < 0)? std::abs(_signed_v) : _signed_v;
		_bitStr = toBits(_dec);
		MAX_BIT = BIT_TOKEN((unsigned)std::strlen(_bitStr.data()));

		// if a negative decimal value
		if (_signed_v < 0)
		{
			_bits = (bool*)bits_from_str(_bitStr);
			
			_bitStr = "\0";
			
			invert_bits(_bits, MAX_BIT);

			// get the inverted bits string
			_bitStr = str_from_bits(_bits, MAX_BIT);

			// convert the inverted bits string into a value
			_v = value_from_bitstr(_bitStr);

			++_v; // added 1 to the inverted bits

			// convert the value into bits string again
			_bitStr = "\0";
			_bitStr = toBits(_v);

			// convert the bits string into a bit array
			delete[] _bits; // clear previous allocated bits
			_bits = (bool*)bits_from_str(_bitStr);

			// convert back the bit array into bits string
			_bitStr = "\0";
			_bitStr = str_from_bits(_bits, MAX_BIT);
			_bitLen = _bitStr.size();
			
		}

		delete[] _bits;
		return _bitStr;
	}

	const size_t BitN::bitSize() const { return _bitStr.size(); }


	// returns a string representation of a fixed point binary bits.
	const std::string& BitN::to_fixed_point_bits(const std::string& _cf, const double _epsilon)
	{
		size_t _sz = std::strlen(_cf.data());
		size_t _point = _cf.find_first_of(".", 0);
		int _decPart = atoi(_cf.substr(0, _point).data());
		int _frac = atoi(_cf.substr(_point + 1, _sz - (_point + 1)).data());

		size_t _fracZeros = _sz - (_point + 1); // would be used in 2^(-n)
		double _dFrac = _frac; // fraction part

		toBits((unsigned)_decPart); // result is stored to _bitStr
		_bitStr = strcat((char*)_bitStr.c_str(), ".");
	
		std::string _sDecPart;
		_sDecPart.assign(_bitStr.data() );

		_bitStr = "\0";
		_bitStr = toBits((unsigned)_frac); // fraction bits stored to _bitStr

		 // integrate decimal & fraction bits length
		std::size_t s1 =0, s2 = 0;

		s1 = std::strlen(_sDecPart.data()); // the bits length of decimal part
		s2 = std::strlen(_bitStr.data()); // the bits length of fraction part


		// new spaces for storing the integrated fix-point bits
		char* _ps = new char[2*(s1 + s2)]; 

		for (size_t i = 0; i < s1; i++)
			_ps[i] = _sDecPart[i];

		_point = _sDecPart.find_first_of('.', 0);
		
		_sDecPart = "\0";

		// 10 being raised to the -N zeroes power equates to a value of ' 0.d1d2...d(n) '
		for (size_t k = 0; k < _fracZeros; k++)
			_dFrac /= 10;


		double _fpv = 0, _exp = -1, _2e = 0;
		const double _fv = _dFrac;
		int _pos = (int)(++_point);

		double i = 0;

			while( _fpv < _fv)
			{
				_dFrac *= 2;
				i = (int)_dFrac;

				// how to get its fraction part as double??
				_dFrac = std::abs(_dFrac - i);

				//RPRINT("i = "); RPRINT(i); RET2();

				_2e = std::pow(2, _exp--);
				//RPRINT("2e^(-n) = "); RPRINT(_2e); RET2();

				if (i > 0)
				{
					_fpv += i * _2e;
					_ps[_pos++] = '1';
				}
				else {
					_ps[_pos++] = '0';
				}

				// adjust number of bits behind the point with (_frac - _fpv) <= epsilon
				if ( (_fv - _fpv) <= _epsilon ) break;
			}

			_ps[_pos] = '\0';
			_bitStr = "\0";

			_bitLen = std::strlen(_ps);
			_bitStr.assign(_ps);

			delete[] _ps;
		return _bitStr;
	}

	void BitN::operator()() const
	{
		std::cout << _bitStr.data() << "\n\n";
	}



	// Implementations to _32Bit Data Structure...
	const int _32Bit::Byte() const
	{
		return this->EAX[3];
	}

	const int32_t _32Bit::HiWord() const
	{
		MAX_BIT = proper_bits(this->AX);

		if (MAX_BIT > WORD )
		{
			return MergeBits(this->EAX[0], this->EAX[1]);
		}
		else
			return 0;
	}

	const int32_t _32Bit::LoWord() const
	{
		MAX_BIT = proper_bits(this->AX);

		if (MAX_BIT > WORD )
		{
			return MergeBits(this->EAX[2], this->EAX[3]);
		}
		else
			return 0;
	}

	const int16_t _32Bit::HiByte() const
	{
		MAX_BIT = proper_bits(this->AX);

		if (MAX_BIT > BYTE && MAX_BIT <= WORD)
		{
			return this->EAX[2];
		}
		else
			return 0;
	}

	const int16_t _32Bit::LoByte() const
	{
		MAX_BIT = proper_bits(this->AX);

		if (MAX_BIT > BYTE && MAX_BIT <= WORD)
		{
			return this->EAX[3];
		}
		else
			return 0;
	}

	const int _32Bit::Value() const
	{
		return this->AX;
	}

	const int _32Bit::BitLength() const
	{
		return proper_bits(this->AX);
	}

	void _32Bit::setValue(const int _Dx)
	{
		this->AX = _Dx;
		MAX_BIT = proper_bits(this->AX);
		int _hiWord = 0, _loWord = 0;
		
		switch (MAX_BIT)
		{
			case 8:
				this->EAX[3] = this->AX;
				break;

			case 16:
				this->EAX[2] = HiPart(this->AX);
				this->EAX[3] = LoPart(this->AX);
				break;

			case 32:
				_hiWord = HiPart(this->AX); // high-order 16 bit part of AX
				_loWord = LoPart(this->AX); // low-order 16 bit part of AX

				this->EAX[0] = HiPart(_hiWord); // high-order 8 bit of _hiWord
				this->EAX[1] = LoPart(_hiWord);  // low-order 8 bit of _hiWord

				this->EAX[2] = HiPart(_loWord); // high-order 8 bit of _loWord
				this->EAX[3] = LoPart(_loWord); // low-order 8 bit of _loWord

			default:
				break;
		}
	}

