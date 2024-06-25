#pragma once

#ifndef HUFF_TREE
	#include <vector>
	#include <deque>

#endif



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


	const size_t bitSize() const { return _bitLen; }

private:
	std::deque<bool> _vBits;
	std::string _bitStr;
	std::size_t _bitLen;
};
