#pragma once
/* Using License: GPL .v .3.0 */

#ifndef HUFF_TREE
	#include <vector>
	#include <deque>

#endif

constexpr unsigned BYTE = 8;
constexpr unsigned WORD = 16;
constexpr unsigned DWORD = 32;
constexpr unsigned DDWORD = 64;


// the max. number of bits evaluated by 'BIT_TOKEN()'
unsigned MAX_BIT = 0;


// returns a specific named token which evaluates to a max. number of bits.
constexpr unsigned BIT_TOKEN(const unsigned nBits)
{
	if (nBits <= BYTE) MAX_BIT = BYTE;
	else if (nBits > BYTE && nBits <= WORD) MAX_BIT = WORD;
	else if (nBits > WORD && nBits <= DWORD) MAX_BIT = DWORD;
	else MAX_BIT = DDWORD;

	return MAX_BIT;
}


// invert every bit in the bit array.
void invert_bits(bool _pb[], const unsigned nBits)
{
	for (unsigned j = 0; j < nBits; j++)
	{
		_pb[j] = !_pb[j];
	}
}



// get a bit character from a position in the bit array as specified by '_pb'.
const char* char_from_bit(const bool _pb[], const int _index)
{
	return (_pb[_index])? (char*)"1" : (char*)"0";
}



// get a value from a position in the bit array 
const unsigned n_of_bit(const bool _pb[], const int _index)
{
	return _pb[_index];
}



// obtain a full bits string from the bit array specified by '_pb'.
const std::string str_from_bits(const bool _pb[], const unsigned nBits)
{
	char* _ss = new char[nBits];

	for (unsigned i = 0; i < nBits; i++)
		_ss[i] =  (_pb[i])? 49 : 48;
	
	_ss[nBits] = '\0';
	return _ss;
}


/* obtain a bits from string data, this includes the most significant bits padded
   to the left of primary bits */
const bool* bits_from_str(const std::string& _cBits)
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
	BitN(const std::initializer_list<bool>& _bitL) : _bitLen(0)
	{
		_vBits.erase(_vBits.begin(), _vBits.end());

		for (const auto& _b : _bitL)
			_vBits.push_front(_b);

		_bitLen = _vBits.size();
	}

	void setBits(const std::initializer_list<bool>& _bitL)
	{
		_vBits.erase(_vBits.begin(), _vBits.end());

		for (iList<bool>::iterator _pb = _bitL.begin(); _pb != _bitL.end(); _pb++)
		{
			_vBits.push_front(*_pb);
		}

		_bitLen = _vBits.size();
	}


	const int value_from_bitstr(const std::string& _bits)
	{
		int _v = 0, j = 0, _exp = 0;
		const int _nBits = (int)strlen(_bits.c_str());

		bool* _pb = new bool[_nBits];

		for (j = (_nBits - 1); j >= 0; j--)
		{
			 _pb[j] = (_bits.c_str()[j] == 49)? true : false;
			 _v += _pb[j] * (0b1 << _exp++);
		}

		delete[] _pb;

		return _v;
	}


	const std::string& Bits()
	{
		_bitStr = "\0";
	
		for (std::deque<bool>::reverse_iterator _pb = _vBits.rbegin();
			_pb != _vBits.rend(); _pb++)
		{
			strcat((char*)_bitStr.c_str(), (*_pb)? (char*)"1" : (char*)"0");
		}
		
		return _bitStr;
	}


	const std::string& toBits(const unsigned _val)
	{
		unsigned _dec = _val;
		bool _bv = 0;

		_bitStr = "\0";
		_vBits.erase(_vBits.begin(), _vBits.end());

		while (_dec > 1)
		{
			_bv = _dec % 2;
			_vBits.push_front(_bv);
			_dec >>= 1;
		};

		_vBits.push_front(1);
		_bitLen = _vBits.size();

		for (std::deque<bool>::iterator _pb = _vBits.begin(); _pb != _vBits.end(); _pb++)
			strcat((char*)_bitStr.c_str(), (*_pb)? (char*)"1" : (char*)"0");
		
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


	// obtains a 2-complement bits string from a specific signed value ( + / - )
	const std::string& to_signed_bits(const int _signed_v)
	{
		_bitStr = "\0";
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
			_bitLen = std::strlen(_bitStr.c_str());
			
		}

		delete[] _bits;
		return _bitStr;
	}

	const size_t bitSize() const { return _bitLen; }


	// returns a string representation of a fixed point binary bits.
	const std::string& to_fixed_point_bits(const std::string& _cf)
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
		char* _ps = new char[s1 + s2]; 

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
				if ( (_fv - _fpv) <= 0.005 ) break;
			}

			_ps[_pos] = '\0';
			_bitStr = "\0";

			_bitLen = std::strlen(_ps);
			_bitStr.assign(_ps);

		return _bitStr;
	}

	void operator()() const
	{
		std::cout << _bitStr << "\n\n";
	}

private:
	std::deque<bool> _vBits;
	std::string _bitStr;
	std::size_t _bitLen;
};

