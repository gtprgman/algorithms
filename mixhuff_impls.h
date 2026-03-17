#pragma once
/* Using License GPL v 3.0. */

#ifndef MX_HUFF_IMPLS
	#define MX_HUFF_IMPLS

#endif


const size_t _ROWSZ = 80;
static std::string _SystemFile = "\0";


constexpr double COMP_RATE = 0.52; /* 0.52 is the default value, the users are allowed to tweak it
									 in the command line */

static const intmax_t writePack(std::FILE*&, const std::string&);

static const size_t readPack(std::FILE*&, std::vector<intmax_t>&);

static const size_t readPackInfo(const std::string&, 
								 std::vector<intmax_t>&, std::vector<intmax_t>&, 
								 std::vector<intmax_t>&, std::vector<intmax_t>&,
								 std::vector<intmax_t>&, std::vector<intmax_t>&
								);

// Impl of struct node : 'mixhuff.h'
node::node() :_data(0), _fdata(0)
{

}


node::node(unsigned char&& _Val) : _data(_Val), _fdata(0)
{

}


node::node(unsigned char&& _c, int64_t&& _fv) : _data(_c), _fdata(_fv)
{

}


node::node(const node& rNod) {
	if (this == &rNod) return;
	this->_data = rNod._data;
	this->_fdata = rNod._fdata;
}


node::node(node&& rvNod)
{
	if (this == &rvNod) return;
	this->_data = rvNod._data;
	this->_fdata = rvNod._fdata;
	rvNod.~node();
};



const node& node::operator= (const node& rNod) {
	if (this == &rNod) return (*this);

	this->_data = rNod._data;
	this->_fdata = rNod._fdata;

	return (*this);
}


node&& node::operator= (node&& rvNod) noexcept {
	if (this == &rvNod) return std::move(*this);

	this->_data = rvNod._data;
	this->_fdata = rvNod._fdata;

	rvNod._data = 0;
	rvNod._fdata = 0;

	rvNod.~node();

	return std::move(*this);
}


node::~node() {
	this->_data = 0;
	this->_fdata = 0;
}


void node::setData(unsigned char&& uc) {
	this->_data = uc;
}


void node::setFrequencyData(int64_t&& fc) {
	this->_fdata = fc;
}


// Get Accessor Methods..
const int& node::Value() const {
	return this->_data;
}


const unsigned char& node::dataValue() const {
	return this->_data;
}


const int64_t& node::FrequencyData() const {
	return this->_fdata;
}

// implicit conversion
node::operator int() const {
	return (int)this->_data;
}

node&& node::Release() const {
	return node(*this);
}


const int& node::operator()()  const{
	return (int)this->_data;
}


const bool node::operator < (const node& _other)
{
	return (this->_data < _other._data);
}


const bool node::operator > (const node& _other)
{
	return (this->_data > _other._data);
}

const bool node::operator <= (const node& _other)
{
	return (this->_data <= _other._data);
}

const bool node::operator >= (const node& _other)
{
	return (this->_data >= _other._data);
}

const bool node::operator == (const node& _other)
{
	return (this->_data == _other._data);
}

void node::Print() const {
	RPRINT(this->_data); RPRINT("->"); RPRINT(this->_fdata);
	RET;
}



// Impl of _TREE::plot_tree() : 'mixhuff.h'
inline void _TREE::plot_tree(const std::vector<node>& _fpNods, const double& _compRate)
{
	schema_Iter(_fpNods, _compRate);
}


// ReSync the integers of *.sqz
static inline const std::size_t ReSync_Int(const std::string& _SqzHex, 
										   const std::vector<UC>& Bit_Len,
										   const std::vector<Can_Bit>& _CniHeader, 
										   std::vector<UC>& _Original)
{
	intmax_t w = 0, bx = 0;
	std::size_t synced_size = 0;
	std::vector<Can_Bit>& HeaderInfo = (std::vector<Can_Bit>&)_CniHeader;
	std::vector<Can_Bit>::iterator _HiT;

	char _x = 0;
	std::string bitX = "\0", sqz_hex = _SqzHex;
	
	const std::string::iterator _bitter = sqz_hex.begin();
	const std::string::iterator _bitEnd = sqz_hex.end();

	char *_pb = (char*)_bitter._Ptr; // points to the beginning of sqz_hex
	std::vector<intmax_t> _SqzCodes = {};
	const size_t _BitL_Size = Bit_Len.size();
	
	bitX = "\0";

	// acquiring codes symbols from the read *.sqz data
	for (size_t g = 0; g < _BitL_Size; g++) 
	{
		w = (int)Bit_Len[g];
	
		if (w > 1 && *_pb == '0') /* the codeword '0' is only 1 bit wide, if it appeared in other bits pattern then it
									must be cut off. */
		{
			for (; *_pb == '0'; _pb++) continue;
			
		}

		bitX = lstr(_pb, w);
		bx = int_bit(bitX.c_str());
		_SqzCodes.push_back(bx);
		
		bx = 0; bitX.clear(); bitX = "\0";

		if ((_bitEnd._Ptr - _pb) > 1) _pb += w; else _pb += 0;
		w = 0;
	}

/*
	mix::generic::STL_Print(_SqzCodes.begin(), _SqzCodes.end(), RPRINTC<intmax_t>); RET;
	return 0;
   // '_SqzCodes' vector is successfully filled with correct codes symbols
*/

/*
	for (const auto& _cne : HeaderInfo)
	{
		RPRINTC(_cne._xData); RPRINTC(_cne._codeWord); RET;
	}

*/

	const intmax_t pacSize = _SqzCodes.size();

	// resolving codes symbols into original data ..
	for (intmax_t t = 0; t < pacSize; t++)
	{
		if ( mix::generic::vector_search(HeaderInfo.begin(), HeaderInfo.end(), _SqzCodes.at(t),
										mix::generic::NLess<intmax_t>(), _HiT) )
		{
			_x = _HiT->_xData;
			_Original.push_back((int)_x);
			++synced_size;
			_x = 0;
		}
	}

	//mix::generic::STL_Print(_Original.begin(), _Original.end(), RPRINTC<char>); RET;

	return synced_size;
}


// Impl of 'filter_pq_nodes' prototype : 'mixhuff.h'
static inline void filter_pq_nodes(std::vector<node>& _target, std::vector<node>& _Pqueue)
{
	std::size_t _Cnt = 0;
	node _nod = 0;
	intmax_t _fqr = 0;


	for (std::vector<node>::iterator _nt = _Pqueue.begin(); _nt < _Pqueue.end(); _nt++)
	{
		_nod = *_nt;

		if (_target.empty())
		{
			_fqr = _nod.FrequencyData();
			if (!_fqr) ++_fqr;
			_nod.setFrequencyData(intmax_t(_fqr) );
			_target.push_back(_nod);
			continue;
		}

		if (_target[_Cnt] == _nod)
		{
			_fqr = _target[_Cnt].FrequencyData();
			_target[_Cnt].setFrequencyData(intmax_t(++_fqr) );
			continue;
		}
		else
		{
			_fqr = 1;
			_nod.setFrequencyData(int64_t(_fqr) );
			_target.push_back(_nod);
			_Cnt = _target.size() - 1;
		}
	}
}


// impl of _TREE::create_encoding() : 'mixhuff.h'
inline const bool _TREE::create_encoding(const size_t& _From, 
										 const size_t& _To,
										 int64_t& _bt,
										 const std::vector<node>& _Vn)
{
	node _e = '0'; bool _bEncodeable = false;
	intmax_t _Dir = 0, _recurr = 0, _sameVal = 0, _prevX = 0;
	static intmax_t _fq = 0;
	std::vector<BPAIR<unsigned char>>::iterator _iGet;
	const size_t _LowerBound = _From, _UpperBound = _To;

	if (_LowerBound > _Vn.size() || _UpperBound > _Vn.size()) return false;

	_fq = 50; // the initial root frequency is 50%

	// Processing the Encoding from vector data
	for (size_t i = _From; i < _To; i++)
	{
		 _e = _Vn.at(i);

		if (_fq > _e.FrequencyData())
			_Dir = L;

		else if (_fq < _e.FrequencyData())
			_Dir = R;

		else if (_fq == _e.FrequencyData())
		{
			_Dir = R;
			_recurr++;
		}
	  
		_fq = _e.FrequencyData();

		if (_recurr > 1)
		{
			_sameVal = ++_prevX;
			_bt = _sameVal;
			_recurr = 0;
			goto filterPhase;
		}
		
		_bt |=_Dir;

	filterPhase: // "enforce unique mechanism" layer 1

		_sameVal = _bt;
		_prevX = _sameVal;

		if (mix::generic::
			vector_search(_vPair.begin(), _vPair.end(), _sameVal, mix::generic::NLess<intmax_t>(), _iGet))
			//if (std::binary_search(_vPair.begin(), _vPair.end(),_bpr))
		{
			_sameVal = _iGet->_val;
			_sameVal += ((_iGet - _vPair.begin()) / 2);
			_bt = _sameVal;
			_prevX = _sameVal;
		}

		_vPair.push_back(BPAIR{ UC(_e.dataValue()), intmax_t(_prevX) });
		mix::generic::fast_sort(_vPair.begin(), _vPair.end(), mix::generic::NLess<intmax_t>());
		//std::stable_sort(_vPair.begin(), _vPair.end());
		_bEncodeable = true;
	}
	_fq = 0;

	
	return _bEncodeable;
}



// Impl of _TREE::schema_Iter() : 'mixhuff.h'
inline void _TREE::schema_Iter(const std::vector<node>& _fpNods, const double _cmpRate = 0)
{
	const size_t _TreeSizes = _fpNods.size();

	const double _CompRate = (_TreeSizes > 5 && _cmpRate)? std::floor(_cmpRate * _TreeSizes) : _TreeSizes;

	const double _fCompRate = (_CompRate)? std::floor((double)_TreeSizes / _CompRate) : 0;
	const size_t _DivSize = (_fCompRate)? (size_t)_fCompRate : 1;
	size_t _divSize = _DivSize;
	int64_t _msk = 0, _BT = 2, _Dir = L;
	bool _bSucceed = false;

	//Clean();

	for (size_t t = 0; t < _TreeSizes; t += _DivSize)
	{
		if ( (_TreeSizes - t) <= _DivSize ) _divSize = 1;
		_bSucceed = create_encoding(t, (( t + _divSize) >= _TreeSizes)? (t + _TreeSizes) : (t + _divSize), _msk, _fpNods);

		if (!_bSucceed) break;
	
		_msk ^= _BT--;

		if (_BT < 1) 
			_BT = 2; 
			

		_Dir = _Dir ^ 1;
		_msk |= _Dir;
	}

	// to encode the last left item in '_fpNods' vector
	create_encoding(_TreeSizes - 1, _TreeSizes, _msk, _fpNods);

	mix::generic::t_sort(_vPair.begin(), _vPair.end(), 0.25, mix::generic::numLess());
	_TREE::enforce_unique(_vPair); 
}



// Impl of _TREE::enforce_unique() : 'mixhuff.h'
inline void _TREE::enforce_unique(std::vector<BPAIR<unsigned char>>& _bPairs)
{
	int64_t _Addend = 0;
	int64_t _MaxSz = (int64_t)_bPairs.size();

	for (int64_t n = 0, m = 0; m < _MaxSz; m++)
	{
		_Addend = m;
		_bPairs[m]._val += (_Addend + 1);
		_Addend = 0;
	}
}



static inline const intmax_t writePackInfo(std::FILE*& _fHandle, const std::vector<unsigned char>& _hDatInfo)
{
	intmax_t f_size = 0;
	int _dat = 0, _coeffcnt = 0;
	std::vector<UC>& header_info = (std::vector<UC>&)_hDatInfo;
	std::vector<intmax_t>header_info_saved = {}, header_bit_info = {}, header_packed = {}, header_bit_packed = {};
	std::string header_packed_hex = "\0", header_hex_bit = "\0";

	if (!_fHandle) {
		std::cerr << "\n Error saving header information to a File. \n";
		return 0;
	}

	for (std::vector<UC>::iterator _ht = header_info.begin();
				_ht < header_info.end(); _ht++)
	{
		_dat = *_ht;
		_coeffcnt = _dat ^ 127;
		header_info_saved.push_back(int(_coeffcnt));
		header_bit_info.push_back(len_bit(int(_coeffcnt)));
	}
	
	cni_bits_pack(header_packed, header_info_saved);
	cni_bits_pack(header_bit_packed, header_bit_info);

	for (const auto& _hp : header_packed) 
		header_packed_hex = concat_str((char*)header_packed_hex.c_str(), To_HexF<int>::eval(_hp).c_str());

	for (const auto& _hx : header_bit_packed)
		header_hex_bit = concat_str((char*)header_hex_bit.c_str(), To_HexF<int>::eval(_hx).c_str());


	f_size += writePack(_fHandle, header_packed_hex);
	f_size += writePack(_fHandle, header_hex_bit);

	return f_size;
}




// Read the encoded information data table from a file.
static inline const std::size_t readPackInfo(const std::string& _inFile, 
											std::vector<intmax_t>& _EncodedChars ,std::vector<intmax_t>& _EncodedCharsBits,
											std::vector<intmax_t>& _CodeWords,  std::vector<intmax_t>& _CodeBits,
											std::vector<intmax_t>& _ActualSymbols , std::vector<intmax_t>& _ActualCodeBits )
{
	size_t tot_size = 0, _readSize = 0,
			_CharsSize = 0, _CodeSize = 0, _SymbolSize = 0,
			_CharBitSize = 0, _CodeBitSize = 0, _SymbolBitSize = 0;


	std::FILE* _Fi = std::fopen(_inFile.c_str(), "rb");

	if (!_Fi)
	{
		std::cerr << "\n Error read header information from the file.";
		std::cerr << "\n Could not proceed ..";
		return 0;
	}

	if (!(_readSize = std::fread(&_CharsSize, sizeof(size_t), 1, _Fi)))
		std::cerr << "\n Error read encoded data lengths information .. ";
	else readPack(_Fi, _EncodedChars); // reads up encoded chars from the file.

	tot_size += _readSize; _readSize = 0;

	if (!(_readSize = std::fread(&_CharBitSize, sizeof(size_t), 1, _Fi)))
		std::cerr << "\n Error read encoded data bit-lengths information .. ";
	else readPack(_Fi, _EncodedCharsBits);  // reads up encoded chars bit-lengths info  ..

	tot_size += _readSize; _readSize = 0;

	if (!(_readSize = std::fread(&_CodeSize, sizeof(size_t), 1, _Fi)))
		std::cerr << "\n Error read codewords lengths information .. ";
	else readPack(_Fi, _CodeWords); // reads up code words info ..
	
	tot_size += _readSize; _readSize = 0;

	if (!(_readSize = std::fread(&_CodeBitSize, sizeof(size_t), 1, _Fi)))
		std::cerr << "\n Error read codewords bit-lengths information .. ";
	else readPack(_Fi, _CodeBits); // reads up codewords bit-lengths info ..

	tot_size += _readSize; _readSize = 0;

	if (!(_readSize = std::fread(&_SymbolSize, sizeof(size_t), 1, _Fi)))
		std::cerr << "\n Error read actual code symbol lengths information .. ";
	else readPack(_Fi, _ActualSymbols); // reads up actual code symbols data ..

	tot_size += _readSize; _readSize = 0;

	if (!(_readSize = std::fread(&_SymbolBitSize, sizeof(size_t), 1, _Fi)))
		std::cerr << "\n Error read actual code symbol bit-lengths information .. ";
	else readPack(_Fi, _ActualCodeBits); // reads up actual code symbol bit-lengths information ..

	tot_size += _readSize; _readSize = 0;

	if (_Fi) std::fclose(_Fi);
	return tot_size;
}



// Packed the raw data source and saves it to a file.
static inline const intmax_t writePack(std::FILE*& _fSqz, const std::string& sqz_hex)
{
	if (!_fSqz) {
		std::cerr << "\n\n Error Open Write-Accesses to File !\n";
		return 0;
	}

	const size_t hex_size = sqz_hex.size();
	const size_t byte_size = (HEX_SIZE * hex_size) / BYTE;

	// saving actual byte size of a packed hex bit
	std::fwrite(&byte_size, sizeof(byte_size), 1, _fSqz);
		
	const size_t w_size = save_cni_bit(_fSqz, sqz_hex);

	if (_fSqz) std::fclose(_fSqz);

	return w_size;
}



// Read the packed data source into an int Vector.
static inline const std::size_t readPack(std::FILE*& _fHandle, std::vector<intmax_t>& vInts)
{
	int _x = 0;
	size_t _totBytesRead = 0;

	if (!_fHandle) {
		std::cerr << "\n Error open Read-Accesses to File !\n\n";
		return 0;
	}

	while ((_x = std::fgetc(_fHandle)) > EOF)
	{
		vInts.push_back(int(_x));
		++_totBytesRead;
	}

return _totBytesRead;
}



// write the original uncompressed form of the data into a file.
static inline const std::size_t writeOriginal(const std::string& _OriginFile, const std::vector<UC>& raw_dat)
{
	size_t rawSize = 0;
	std::FILE* FRaw = std::fopen(_OriginFile.data(), "wb");

	if (!FRaw)
	{
		std::cerr << "\n Error Write-Accessed to File.";
		return 0;
	}

	const size_t nSize = raw_dat.size();;

	for (size_t s = 0; s < nSize; s++)
	{
		std::fputc(raw_dat[s], FRaw);
		++rawSize;
	}

	if (FRaw) std::fclose(FRaw);
	return rawSize;
}



// read the original data source from a file and populate the vector with the read data.
static inline const std::size_t readOriginal(std::FILE*& _fHandle , std::vector<unsigned char>& _vecData, unsigned char*& _DataSrc)
{
	std::size_t _wordsRead = 0;
	
	if (!_fHandle)
	{
		std::cerr << "\n Error read source file.. !! " << "\n\n";
		goto EndRead;
	}

	if (!_DataSrc) _DataSrc = new unsigned char[_ROWSZ];
	
	while (std::fgets((char*)_DataSrc, _ROWSZ, _fHandle))
	{
		for (size_t c = 0; c < _ROWSZ; c++) {
			if (_DataSrc[c] > 0) {
				_vecData.push_back(_DataSrc[c]);
				++_wordsRead;
			}
		}

		std::memset(_DataSrc, 0, _ROWSZ);
	}
	

EndRead:
	return _wordsRead;
}


// generates a huffman encoding information ..
static inline const int64_t Gen_Encoding_Info(std::vector<unsigned char>& _Src, 
											  std::vector<BPAIR<unsigned char>>& CodInfo, 
										      std::vector<_Canonical>& Cni_Info0, 
											  std::vector<_Canonical>& Cni_Info1,
											  std::vector<intmax_t>& PacResults,
											  std::vector<intmax_t>& PacInts,
										      const double& cmp_rate, 
											  char&& _cCode = 'u')
{
	intmax_t SqzInt = 0;
	/*
		std::priority_queue<node, std::vector<node>, bitLess> _pq = {};
		std::priority_queue<node, std::vector<node>, fqLess> _fpq = {};
	*/
	std::vector<node> _pq = {};

	std::vector<node> PNodes = {};

	Can_Bit cbi = {};

	std::vector<_Canonical> CniBits = {};
	std::vector<_Canonical>::iterator CBiT = {};

	std::string xs_bit = "\0";

	const double comp_ratio = (cmp_rate)? cmp_rate : COMP_RATE;

	const size_t T_SIZE = _Src.size();

	for (size_t t = 0; t < T_SIZE; t++)
	{
		PNodes.push_back(node(UC(_Src[t])));
	}

	if (_cCode == 'D') {
		PRINT("\nNodes data..");
		for (const auto& _e : PNodes) RPRINTC(_e.dataValue());
	}
	RET;

	PRINT("\n Re-Ordering nodes elements ... "); 
	mix::generic::STL_Priority_Queue(_pq, PNodes.begin(), PNodes.end(), mix::generic::NLess<node>());

	_pq.clear();

	RPRINT("finished done.\n");

	PRINT("\n Filtering nodes elements...");
	filter_pq_nodes(_pq, PNodes);

	PNodes.clear();

	RPRINT("finished done. \n");

	if (_cCode == 'D')
	{
		PRINT("\nFiltered Nodes Data..");
		for (const auto& _e : _pq) RPRINTC(_e.dataValue());
		RET;
	}

	if (_cCode == 'D')
	{
		PRINT("\nFrequency nodes data..");
		for (const auto& _e : _pq)
		{
			RPRINT(_e.dataValue()); RPRINT("=>"); RPRINT(_e.FrequencyData()); RET;
		}
		RET;
	}

	PRINT("\n generating tree .. ");
	_TREE::plot_tree(_pq, comp_ratio);
	RPRINT(" finished done. \n");

	CodInfo = _TREE::CodeMap(); // huffman encoding info generated

	vectorClean(Cni_Info0);

	if (_cCode == 'D')
	{
		PRINT("\nGenerated Huffman Encoding Information. (BPAIR)");
		PRINT("_xData <=> _val");
		for (const auto& bp : CodInfo)
		{
			RPRINTC(bp._data); RPRINTC(bp._val); RET;
		}
	}

	for (const BPAIR<unsigned char>& bp : CodInfo)
	{
		cbi._xData = bp._data;
		cbi._bitLen = bp.bit_len;
		Cni_Info0.push_back(cbi);
		cbi = {};
	}

	if (_cCode == 'D') {
		PRINT("\nRaw Canonical Data Obtained from the Generated BPAIR. (Cni_Info0) ");
		PRINT("_xData <=> _bitLen");
		for (const auto& ce : Cni_Info0)
		{
			RPRINTC(ce._xData); RPRINTC(ce._bitLen); RET;
		}
	}
	
	PRINT("\n Sorting encoding data elements .. ");
		mix::generic::fast_sort(Cni_Info0.begin(), Cni_Info0.end(), mix::generic::NLess<char>());
		mix::generic::fast_sort(Cni_Info0.begin(), Cni_Info0.end(), mix::generic::NLess<intmax_t>());
	RPRINT(" finished done. \n");

	if (_cCode == 'D')
	{
		PRINT("\nSorted Canonical Encoding Information Data.. (sorted Cni_Info0) " );
		PRINT("_xData <=> _bitLen");
		for (const auto& _cb : Cni_Info0)
		{
			RPRINTC(_cb._xData); RPRINTC(_cb._bitLen); RET;
		}

		RET;
	}

	vectorClean(Cni_Info1);

	PRINT("\n Obtaining Code Words .. ");
	_Gen_Canonical_Info(Cni_Info1, Cni_Info0); // obtaining codewords based on bit length info ..
	RPRINT(" finished done. \n ");

	PRINT("\n Enforcing the uniqueness among code words .. ");
	cni_enforce_unique(Cni_Info1); // codewords data updated
	RPRINT(" finished done. \n");

	if (_cCode == 'D') {
		PRINT("\nCanonical Huffman Encoding Information Generated ! [ Cni_Info1 ] " );
		PRINT("_xData <=> _codeWord <=> _bitLen");

		for (const auto& ci : Cni_Info1)
		{
			RPRINTC(ci._xData); RPRINTC(ci._codeWord); RPRINTC(ci._bitLen);
			RET;
		}
	}

	cbi = {}; 

	for (const _Canonical& cni : Cni_Info1)
	{
		cbi._xData = cni._xData;
		cbi._codeWord = cni._codeWord;
		cbi._bitLen = len_bit(intmax_t(cni._codeWord));
		CniBits.push_back(cbi);
		cbi = {};
	}

	cbi = {}; vectorClean(Cni_Info1);

	for (const auto& _cnb : CniBits) {
		cbi._xData = _cnb._xData;
		cbi._codeWord = _cnb._codeWord;
		cbi._bitLen = _cnb._bitLen;
		Cni_Info1.push_back(cbi);
		cbi = {};
	}
	
	PRINT("\n Sorting nodes elements ..");
	mix::generic::t_sort(CniBits.begin(), CniBits.end(), 0.25, mix::generic::NLess<char>());
	RPRINT(" finished done. \n");

	if (_cCode == 'D') {
		PRINT(" \n Finalized Sorted(char) Canonical Encoding Information Generated .. [ Cni_Info1 ] ");
		PRINT(" _xData <=> _codeWord <=> _bitLen ");
		for (const auto& _cnb : CniBits) {
			RPRINTC(_cnb._xData); RPRINTC(_cnb._codeWord); RPRINTC(_cnb._bitLen); RET;
		}
	}

	RET;

	// gathering source data..
	PRINT("\n gathering source data ..");

	for (size_t fz = 0; fz < T_SIZE; fz++)
	{
		char _ci = _Src[fz];

		if (_ci > 0)
		{
			if (mix::generic::vector_search(CniBits.begin(), CniBits.end(), _ci, mix::generic::NLess<char>(), CBiT))
			{
				PacInts.push_back(CBiT->_codeWord);
			}
		}
	}

	RPRINT(" finished done. \n");

	if (_cCode == 'D') {
		PRINT("\n Generated Code Symbols ..");
		mix::generic::STL_Print(PacInts.begin(), PacInts.end(), RPRINTC<intmax_t>); RET;
	}

	PRINT("\n packing bits .. ");
	SqzInt = cni_bits_pack(PacResults,PacInts);
	RPRINT(" finished done. \n");

		if (_cCode == 'D') { RPRINTC("\n Packed Integer Symbols .. "); RPRINTC(SqzInt); } RET;
	
	
	return SqzInt;
}



static inline const bool Compress(const std::string& _destF, const std::string& _srcF, const double& compRate, 
									unsigned char* _cBuff)
{
	bool _bDone = 0;
	UC _xt = 0;

	std::string _sqz_hex = "\0";

	std::vector<UC> _srcData = {}, xChars = {}, xCode = {},
					_bit_len = {}, rle_bit = {};

	std::vector<intmax_t> _pacInts = {}, _pacRes = {};

	std::vector<BPAIR<unsigned char>> _CodeMap = {};
	std::vector<_Canonical> _CanSrc = {}, _CanInfo = {};
	
	std::FILE* _FO = std::fopen(_srcF.c_str(), "rb") , 
			  *_FT = std::fopen(_destF.c_str(), "wb");

	std::size_t F_SIZE = 0, HCN_SIZE = 0, PACKED_HEADER_SIZE = 0;
	intmax_t _sqzNum = 0;

	if (!_FO)
	{
		std::cerr << "\n Error Read Source File. " << "\n\n";
		goto finishedDone;
	}

	if (!(F_SIZE = readOriginal(_FO, _srcData, _cBuff)) )
	{
		RET;
		std::cerr << "\n Error read source file." << "\n\n";
		goto finishedDone;
	}
	
	
	_sqzNum = Gen_Encoding_Info(_srcData, _CodeMap, _CanSrc, _CanInfo, _pacRes,_pacInts, compRate);
	// _srcData is fully filled with correct data
	// _CodeMap is generated successfully
	// _CanSrc is fetch back with the correct data values	
	// _CanInfo is fully filled with correct data
	// _pacInts is fully filled with correct values
	/*
		mix::generic::STL_Print(_pacInts.begin(), _pacInts.end(), RPRINTC<intmax_t>); RET;
		goto finishedDone;
	*/
	
	/*_CanSrc  => Cni_Info0 in Gen_Encoding_Info() and
	  _CanInfo => Cni_Info1 in Gen_Encoding_Info()
	*/

/*
	mix::generic::STL_Print(_pacRes.begin(), _pacRes.end(), RPRINTC<intmax_t>); RET;
	_pacRes is fetched with correct packed values. 
	goto finishedDone;  
*/

	for (const auto& _ei : _pacRes) _sqz_hex = concat_str((char*)_sqz_hex.c_str(), To_HexF<int>::eval(_ei).c_str());
	
	// _sqz_hex is assigned with the correct hex string pattern
	//PRINT(_sqz_hex); RET; goto finishedDone;
	
	/*
		// Debugging Codes.. 

		for (const auto& _cni : _CanInfo)
		{
			RPRINTC(_cni._xData); RPRINTC(_cni._codeWord); RET;
		}

		goto finishedDone;

	*/


	// Saving encoding information headers data ..
	for (const auto& cn : _CanInfo)
	{
		_xt = cn._xData;
		xChars.push_back(_xt);
		_xt = 0;

		_xt = (int)cn._codeWord;
		xCode.push_back(UC(_xt));

		_xt = 0;
	}

	// saving encoded chars ..
	if (!(F_SIZE = writePackInfo(_FT, xChars)))
	{
		std::cerr << "\n Error saving encoded characters ..";
		std::cerr << "\n Could not proceed ..";
		goto finishedDone;
	}

	// saving code symbols ..
	if (!(F_SIZE = writePackInfo(_FT, xCode)))
	{
		std::cerr << "\n Error saving code symbols.. ";
		std::cerr << "\n Could not proceed .. ";
		goto finishedDone;
	}

	// writing packed data source into a file ( *.sqz ).
	if ( !(_bDone = writePack(_FT, _sqz_hex)))
	{
		RET;
		std::cerr << "\n Error writing compressed file !  \n\n";
		goto finishedDone;
	}

	//PRINT(_sqz_hex); RET;

finishedDone:
	vectorClean(_srcData);
	vectorClean(_pacInts);
	vectorClean(_CodeMap);
	vectorClean(_CanSrc);
	vectorClean(xChars);
	
	if (!_SystemFile.empty()) _SystemFile.clear();
	if (_FO) std::fclose(_FO);
	if (_FT) std::fclose(_FT);

	_TREE::Clean();
	F_SIZE = 0;
	return _bDone;
}



static inline const std::size_t UnCompress(const std::string& _packedFile, const std::string& _unPackedFile, 
											const double& cmp_rate)
{
	UC _Bit = 0; Can_Bit _cbt;
	std::vector<UC> _rawData, _BitL;
	std::vector<Can_Bit> cnbt;
	std::vector<_Canonical> Cni_Info, Cni_Head0, Cni_Head1;
	std::vector<intmax_t> _Codes, _Chars, _Actual, _CodeBits, _CharBits, _ActualBits;
	intmax_t _SqzInt = 0, _bix = 0;
	size_t _rawSize = 0, header_size = 0, bits_sizes = 0;
	std::string _read_hex = "\0", _bitX = "\0";
	char* _hex_Bits = nullptr; 

	header_size = readPackInfo(_packedFile, _Chars, _CharBits, _Codes, _CodeBits, _Actual, _ActualBits);


	if (!header_size)
	{
		std::cerr << "\n Header Info Data Read Error !";
		std::cerr << "\n Could not proceed ..";
		goto EndPhase;
	}
	
	   // Debugging Codes..

	mix::generic::STL_Print(_Chars.begin(), _Chars.end(), RPRINTC<intmax_t>); RET;
	mix::generic::STL_Print(_Codes.begin(), _Codes.end(), RPRINTC<intmax_t>); RET;
	mix::generic::STL_Print(_Actual.begin(), _Actual.end(), RPRINTC<intmax_t>); RET;

	mix::generic::STL_Print(_CharBits.begin(), _CharBits.end(), RPRINTC<intmax_t>); RET;
	mix::generic::STL_Print(_CodeBits.begin(), _CodeBits.end(), RPRINTC<intmax_t>); RET;
	mix::generic::STL_Print(_ActualBits.begin(), _ActualBits.end(), RPRINTC<intmax_t>); RET;

	goto EndPhase;

	

	

	for (const auto& _e : _Codes) _read_hex = concat_str((char*)_read_hex.c_str(), To_HexF<int>::eval(_e).c_str());
	// _read_hex is assigned with the correct hex digits pattern

	//PRINT(_read_hex); RET; goto EndPhase;
	
	bits_sizes = (_read_hex.size() * 4) * sizeof(char);

	_hex_Bits = new char[bits_sizes]; 

	_hex_Bits = (char*)HxFs_To_Bin(_read_hex.c_str()).c_str(); // _hex_Bits is pointed to the corret bits pattern.

	//PRINT(_hex_Bits); RET; goto EndPhase;
	
	_hex_Bits = (char*)trunc_left_zeroes(_hex_Bits); // _hex_Bits is pointed to the correct truncated bits pattern.

	//PRINT(_hex_Bits); goto EndPhase;

	header_size = Cni_Head1.size();
	// expands out RLE information into '_BitL' vector
	for (size_t x = 0; x < header_size; x++)
	{
		_Bit = (int)Cni_Head1[x]._bitLen;
		_bix = Cni_Head1[x]._rle_bit_len;

		// t = 0 -> number of repeated bit-lengths..
		for (intmax_t t = 0; t < _bix; t++)
		{
			_BitL.push_back(UC(_Bit));
		}
	} // _BitL is successfully fetched with correct data

	//mix::generic::STL_Print(_BitL.begin(), _BitL.end(), RPRINTC<intmax_t>); RET; goto EndPhase;

/*
	// Debugging codes.. 
	RPRINTC("Data: "); RPRINTC("Code: "); RET;
	for (const auto& _cni : Cni_Info)
	{
		RPRINTC(_cni._xData); RPRINTC(_cni._codeWord); RET;
	}
	goto EndPhase;
*/
	// Finalized 'Encoding Information Header' (Cni_Info) is successfully fetched with correct data


	for (const auto& _ci : Cni_Info)
	{
		_cbt._xData = _ci._xData;
		_cbt._codeWord = _ci._codeWord;

		cnbt.push_back(_cbt);
	}

	PRINT("\n rematching code symbols with data ..");

	if (_BitL[0] == 1) _hex_Bits = (char*)concat_str((char*)"0", _hex_Bits);

	//PRINT(_hex_Bits); RET;	goto EndPhase;

	if (!(_rawSize = ReSync_Int(_hex_Bits, _BitL, cnbt, _rawData)))
		std::cerr << "\n code symbols mismatched ..";

	if (!(_rawSize = writeOriginal(_unPackedFile.c_str(), _rawData)))
		std::cerr << "\n Error writing original data to a file.";

EndPhase:
	std::memset(_hex_Bits, '\0', std::strlen(_hex_Bits));

	_hex_Bits = nullptr;

	vectorClean(_rawData);
	vectorClean(Cni_Info);
	vectorClean(_BitL);
	vectorClean(_Codes);
	vectorClean(Cni_Head0);
	vectorClean(Cni_Head1);
	vectorClean(cnbt);
	
	vectorClean(_Chars);
	vectorClean(_Codes);
	vectorClean(_Actual);

	vectorClean(_CharBits);
	vectorClean(_CodeBits);
	vectorClean(_ActualBits);

	return _rawSize;
}



