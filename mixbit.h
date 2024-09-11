#pragma once
/* Using License: GPL .v .3.0 */

#ifndef HUFF_TREE

	#include <vector>
	#include <deque>

#endif



#ifndef MX_BIT
	#define MX_BIT
#endif


#define halfSz(_Tot_) (_Tot_ / 2) - 1
#define oneAdder(_v_) _v_ + 1

constexpr unsigned BYTE = 8;
constexpr unsigned WORD = 16;
constexpr unsigned DWORD = 32;
constexpr unsigned QWORD = 64;




// uppercase the specified char
inline static const char upCase(const int _c)
{
	if (_c <= 0) return _c;

	// is _c already in upper case??
	if (_c >= 65 && _c <= 90)
		return _c;

	
	int _ch = 0b00100000 ^ _c;

	return _ch;
}




// downcase the specified char
inline static const char downCase(const int _cAlpha)
{
	if (_cAlpha <= 0) return _cAlpha;

	// is _cAlpha already in lower case??
	if (_cAlpha >= 97 && _cAlpha <= 122)
		return _cAlpha;

	int _cLow = 0b01100000 | _cAlpha;

	return _cLow;
}




inline static const char* concat_str(char* _target, const char* _str)
{
	const std::size_t lenz = std::strlen(_target),
		lenS = std::strlen(_str);

	char* _pStr = new char[lenz + lenS];
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




/* get a number of characters from a string starting from a position specified
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
	bitInfo(): X(0), numBits(0) {};

	bitInfo(const int _x, BitSZ _bitSz)
	{
		X = _x;
		numBits = _bitSz;
	}

	~bitInfo()
	{
		X = 0;
		numBits = 0;
	}

	int X : 32;  
	BitSZ numBits : 32;
};



// pack the entire bits of the vector into a UINT vector
template < typename T >
inline static void bitsPack(std::vector<UINT>& _packed, const std::vector<bitInfo<T>>& _vb)
{
	int _bx = 0b0, _Ax = 0b0;
	const std::size_t _vcSz = _vb.size(), _nIter = halfSz(_vcSz);
	std::size_t	_loopn = 0;

	T _n = 0;


	for (const auto& ub : _vb)
	{
		_bx = ub.X;
		_n = ub.numBits;

		_Ax <<= _n;
		_Ax |= _bx;

		++_loopn;

		if (_loopn > _nIter)
		{
			_packed.push_back(_Ax);

			_Ax = 0b0;
			_loopn = 0;
		}
	}
}



inline static const unsigned int unpack_bit(const unsigned _nonPacked, const unsigned _packed)
{
	return _packed & _nonPacked;
}



// fixed point numeric type.
template < const unsigned BITS >
struct fixN
{
	fixN(): _nVal(0){}

	fixN(const double x)
	{
		*this = x;
	}

	fixN(const int x)
	{
		*this = x;
	}


	void operator= (const double x)
	{
		_nVal = toFix(x);
	}

	void operator= (int x)
	{
		_nVal = toFixInt(x);
	}
	
	const double operator()()
	{
		return toDouble(_nVal);
	}

	const int operator()(int c)
	{
		return fixToInt(_nVal);
	}

	const double decimal()
	{
		return toDouble((_nVal & iXMask ));
	}

	const double rational()
	{
		return  toDouble((_nVal & fXMask) );
	}

	
private:
	int _nVal;

	const int scale = BITS / 2;

	const int fXMask = ((int)std::pow(2,BITS) - 1) & ((int)std::pow(2,scale) - 1);
	const int iXMask = 0xFFFF << scale;

	const double toFix(const double x)
	{
		return x *(double)(1 << scale);
	}

	const double toDouble(const int x)
	{
		return (double)x / (double)(1 << scale);
	}

	const int toFixInt(int x)
	{
		return x << scale;
	}

	const int fixToInt(int x)
	{
		return x >> scale;
	}

};

// the max. number of bits evaluated by 'BIT_TOKEN()'
unsigned MAX_BIT = 0;


// returns a specific named token which evaluates to a max. number of bits.
constexpr unsigned BIT_TOKEN(const unsigned nBits)
{
	if (nBits <= BYTE) MAX_BIT = BYTE;
	else if (nBits > BYTE && nBits <= WORD) MAX_BIT = WORD;
	else if (nBits > WORD && nBits <= DWORD) MAX_BIT = DWORD;
	else MAX_BIT = 128 - QWORD;

	return MAX_BIT;
}


// Convert alphanumeric string '0,1,2..9' to integer
inline static const int strtoint(const char* _sNum)
{
	int _result = 0;
	const int _len = (int)std::strlen(_sNum);

	for (int _c = 0, _Exp = 0, i = (_len - 1); i >= 0; i--)
	{
		_c = _sNum[i] ^ 0b00110000; // xor with the alphanumeric ascii bit mask
		_result += _c * (int)std::pow(10, _Exp++);

	}

	return _result;
}




inline static const char* inttostr(const int nVal)
{
	char* _ss = new char[20];
	std::deque<char> _cTube;
	int nDiv = nVal, _mod = 0, cnt = 0;

	*_ss = '\0';
	char _ch = '0';

	if (!nDiv) {
		_ss[0] = '0';
		_ss[1] = 0;
		return _ss;
	}

	while (nDiv > 0)
	{
		_mod = nDiv % 10;
		_ch = '0' + (char)_mod;
		_cTube.push_front(_ch);
		nDiv /= 10; ++cnt;
	}

	for (int i = 0; i < cnt; i++)
	{
		_ch = _cTube.at(i);
		_ss[i] = _ch;
	}

	_ss[cnt] = '\0';
	_cTube.clear();

	return _ss;
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
		_bs = nullptr;
		unsigned int _bsz = oneAdder(num_of_bits<unsigned int>::eval(_dec));
		_value = _dec;
		
		if (_bsz > 0)
			_bs = new char[_bsz];

		for (unsigned int i = 0; i < _bsz; i++)
		{
			_bs[i] = (_value % 2)? 49 : 48;
			_value >>= 1;
		}

		_value = 0;
		_bs[_bsz] = 0;
		_bs = (char*)reverse_str(_bs);
		return _bs;
	}

private:
	static value_type _value;
	static char* _bs;
};

// static members initializer
template <class T>
T to_binary<T>::_value = 0;

template <class T>
char* to_binary<T>::_bs = nullptr;






template <class T >
struct bin_to_dec
{
	using value_type = typename T;

	// the bit string is assumed to be in little-endian order.
	static inline const value_type eval(const std::string&& _strBits)
	{
		std::size_t lenMax = std::strlen(_strBits.data());
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
		_ss[i] =  (_pb[i])? '1' : '0';
	
	_ss[nBits] = '\0';
	return _ss;
}


/* obtain a bits from string data, this includes the most significant bits padded
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



struct BitN
{
	BitN() :_bitLen(0) {}

	BitN(const int _val)  
	{
		_bitStr = (_val > 0)? toBits(_val) : to_signed_bits(_val);
		
	}

	BitN(const char* _strFixBits)
	{
		to_fixed_point_bits(_strFixBits, 0.1);
	}

	
	
	BitN(const std::initializer_list<bool>& _bitL) : _bitLen(0)
	{

		toBits(value_from_bitlist(_bitL) );
	}


	void setBits(const std::string&& _bitL)
	{
		_bitStr = _bitL;
		_bitLen = _bitStr.size();
	}



	const int value_from_bitstr(const std::string& _bits)
	{
		int _v = bin_to_dec<int>::eval(_bits.data());

		return _v;
	}


	const std::string& Bits()
	{
		return _bitStr;
	}


	const std::string& toBits(const unsigned _val)
	{
		unsigned _dec = _val;

		_bitStr.clear(); 

		_bitStr = to_binary<unsigned int>::eval(_val);
		
		_bitLen = _bitStr.size();
		return _bitStr;
	}


	const int value_from_bitlist(const iList2<bool>& _bits)
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
	const std::string& to_signed_bits(const int _signed_v)
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

	const size_t bitSize() const { return _bitStr.size(); }


	// returns a string representation of a fixed point binary bits.
	const std::string& to_fixed_point_bits(const std::string& _cf, const double _epsilon)
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

	void operator()() const
	{
		std::cout << _bitStr.data() << "\n\n";
	}


private:
	std::string _bitStr;
	std::size_t _bitLen;
};

