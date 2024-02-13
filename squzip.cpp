#include <iostream>
#include "mixhuff.h"


struct BPAIR {
	Bit bits;
	Byte value;
};


/*
	Parameters for main()
	[0]:	Program executable name.

	[1]:	Parameters for calling the program, this can
			be '-q' for compressing and '-d' for extracting.

	[2]:	Input file, the file to be compressed using huffman algorithm.

	[3]:	Output file, the file that contains the encoded format of the original
			to which the huffman encoding is being applied on.
*/

// the number of bytes read
static LONGFLOAT CSIZE = 0.00;

// parameters for main()
static std::string progName, param1, param2, param3;

// accommodate the data read from external media
static std::vector<NODE_T> RAWC;

// vector data of huffman record
static std::vector<HF_REC> HC;

// vector data of a number of pointers to node
static std::vector<node*> VNT;

// vector of nodes
static std::vector<node> NODS,READYNODS;

// a list of mapping between the encoded bit patterns to its correspondence raw byte
static std::vector<BPAIR> vbp;

static std::FILE* _INF = nullptr, *_OUT = nullptr;


#define ZEROES(var1, var2) var1 = var2 = 0.00

// open a file with a specified file name.
inline const bool openF(const char* _file, const char* _sMode = "rb") {
	if (!std::strcmp("rb", _sMode))
		_INF = fopen(_file, _sMode);
	else _OUT = fopen(_file, _sMode);

	return (_INF || _OUT);
}



// close an opened file
inline void closeF(std::FILE* _inf) {
	if (_inf) fclose(_inf);
}



inline const std::size_t save_encoded_data(const char* _file,std::vector<BPAIR>& _src) {
	const std::size_t LENX = _src.size();
	
	if (!LENX) {
		PRINT("Error: not a valid number of data size");
		return 0;
	}


	if (!openF(_file, "wb")) {
		PRINT("Error: Can't write encoded data to disk.");
		return 0;
	}

	fwrite(&_src, sizeof(_src), LENX, _OUT);
	
	for (const BPAIR& _bp : vbp)
		fwrite(&_bp.bits, sizeof(Bit), 1, _OUT);
	

	fclose(_OUT);
	return LENX;
}




// print the contents of the file specified by _input
inline void print_file(std::FILE* _input) {
	int col = 0;

	while ( !feof(_input) ) {
		char ch = fgetc(_input);
		col++;
		if (ch) RPRINT(ch);
		if (col > 79) {
			col = 0;
			RET;
		}
	}

	fclose(_input);
}



// print the contents of the vector data
inline void print_vector(std::vector<node>& _vc, const std::size_t _vSize) {
	int col = 0;

	for (std::size_t i = 0; i < _vSize; i++) {
		col++;
		RPRINT(_vc[i].dataValue());
		if (col > 79) {
			col = 0;
			RET;
		}
	}
}


/* translate the encoded bit patterns into binary bits form and
	print out the translated bits and its mapped data value*/
inline void print_hf_rec(std::vector<HF_REC>& _hfc) {
	const std::size_t LENZ = _hfc.size();
	HF_REC hf;
	BPAIR bp;

	for (std::size_t i = 0; i < LENZ; i++) {
		Bit x = 0b1, xc = 0b0;
		hf = _hfc[i];

		for (std::size_t j = 0; j < hf._bits.size(); j++) {
			x &= (Bit)hf._bits[j];
			x <<= (7 - j);
			xc |= x;
			x = 0b1;
		}

		bp.bits = xc;
		bp.value = hf._data;
		vbp.push_back(bp); 
		hf.reset();
	}

	for (const BPAIR& _cp : vbp) {
		RPRINT(_cp.bits); RPRINT("|"); RPRINT((char)_cp.value); RET;
	}

}


// print the encoded bit patterns and its associated data value
inline void print_encoded_bits(const std::vector<HF_REC>& _hcf) {
	for (const HF_REC& _hc: _hcf) {
		RPRINT(_hc.Bits()); RPRINT("|");  RPRINT((char)_hc._data); 
		RET;
	}
}



// get the total sum of every count bytes in the file
inline const std::size_t f_sizes(std::FILE* _inf) {
	std::size_t _Count = 0;

	if (!_inf) return 0;
	
	while (!feof(_inf) ) {
		if ( fgetc(_inf) ) ++_Count;
	}
	
	return _Count;
}




// read the content of the file into the vector
inline const std::size_t read_v(std::FILE* _inp, std::vector<NODE_T>& _vc) {
	char ch = 0; 
	std::size_t _Count = 0;

	while ( !feof(_inp) ) {
		ch = fgetc(_inp);
		if (ch != 0) {
			_vc.push_back(ch);
			_Count++;
		}
		ch = 0;
	}

	return _Count;
}


// adding nodes to the vector
inline void add_V(std::vector<node>& _nodes, const NODE_T _nod) {
	bool _bFound = 0;
	const LongRange _vSize = (LongRange)_nodes.size();

	if (_nodes.empty()) {
		if (_nod._v != 0) _nodes.emplace_back(_nod);
		return;
	}

	if (_vSize < 20)
		_bFound = search_Node(_nodes, _nod._v);

	if (_vSize > 20) {
		sort_Nodes<Byte>(_nodes, _vSize - 1);
		_bFound = vector_search(_nodes, _nod._v);
	}

	if (_bFound) return;

	if (_nod._v != 0) _nodes.emplace_back(_nod);
}




/* Filter the nodes to a separate vector, to obtain a single unique node in each element of
   the vector */
inline void filter_V(std::vector<node>& _dest, const std::vector<node>& _src) {
	PRINT("Filtering.. ");

	double _count = 0.00,fc = 0.00;
	std::size_t i = 0;
	const std::size_t LENZ = (_src.size() + 1);
	NODE_T nc;

	if (_src.empty()) return;

	for (i = 0; i < LENZ; i++) {
		if (_src[i].Value() == 0) continue;
		else {
			nc = _src[i];
			break;
		}
	}

	for (; i < LENZ; i++) {
		// comparing value of both side
		if ( nc._v == _src[i].Value() ) {
			++_count;
			fc = (_count / CSIZE) * 100.00;
		}
		else {
			nc._freq = fc;
			ZEROES(_count, fc);
			add_V(_dest, nc);

			if (_src[i].Value() != _src[i + 1].Value()) {
				nc = _src[i];
				nc._freq = (1.00 / CSIZE) * 100.00;
				add_V(_dest, nc);
				nc._v = _src[i + 1].Value();
				continue;
			}
			else {
				nc = _src[i];
				++_count;
			}
		}
	}
}



// transform into vector node
inline void transForm(std::vector<node>& _target, const std::vector<NODE_T>& _source) {
	PRINT("Adding.. ");
	for (const NODE_T& e : _source)
		if (e._v != 0) _target.push_back(e);
}



// sort the nodes based on data or frequency
template < typename T >
inline void sort_V(std::vector<node>& _vn, const std::size_t _vSize) {
	PRINT("Sorting.. "); RET;
	
	if (_vSize > 20)
		merge_sort<T>(_vn, _vSize);
	else
		range_sort<T>(_vn, 0, (LongRange)(_vSize - 1));


	sort_Nodes<T>(_vn, (_vSize-1));
}


// check the frequqncy of each node in the vector
inline void check_nodes_frequency(const std::vector<node>& _fNods) {
	for (const node& e : _fNods) {
		RPRINT(e.dataValue()); RPRINT("["); RPRINT(e.FrequencyData()); RPRINT(" %fqr]");
		RET;
	}
}




inline void decode_file(std::FILE* _input, std::FILE* _output) {
	

}



int main(int argc, const char* argv[4]) {

	for (std::size_t i = 0; i < 4; i++)
	{
		if ( !argv[i] ) argv[i] = "\0";
	}

	progName = argv[0];
	param1 = argv[1];
	param2 = argv[2];
	param3 = argv[3];


	if (!openF(param2.data()) ) {
		PRINT("ERROR: Can't open file !");
		return 0;
	}

	PRINT("processing .. "); RET;
	PRINT("This could take a few minutes.. "); RET;
	
	CSIZE = (LONGFLOAT)read_v(_INF, RAWC);
	closeF(_INF);

	transForm(NODS, RAWC);
	
	RAWC.clear();

	sort_V<Byte>(NODS, NODS.size());

	filter_V(READYNODS, NODS);

	NODS.clear();

	sort_V<double>(READYNODS, (READYNODS.size()) );
	
	NPRINT(READYNODS);
	RET2();

	check_nodes_frequency(READYNODS);

	RET;
	RET;

	PRINT("Encoding.."); RET;

	huff_tree_create(VNT, READYNODS, READYNODS.size());

	huffman_encode(HC, VNT[vNodeLnk._Last]);


	print_encoded_bits(HC);
	//print_hf_rec(HC);
	

	/*
	std::string _fName;

	if (param3.empty()) {
		param3 = param2;
		_fName = strcat((char*)param3.data(), ".szp");
	}
	else
		_fName = param3;

	const std::size_t LENW = save_encoded_data(_fName.data(), mbp);
	*/
	
	RET;
	RET;

   VNT.clear();

	return -1;
};
