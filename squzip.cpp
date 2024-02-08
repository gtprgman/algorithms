#include <iostream>
#include <map>
#include <fstream>
#include "mixhuff.h"


using BPAIR = std::pair<Bit, Byte>;


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
static LONGFLOAT CSIZE = 0.000;

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
static std::map<Bit, Byte> mbp;
static node* d_Tree = nullptr;




void inline save_data(const char* _file,std::vector<node>& _src) {
	std::ofstream _fo(_file, std::ios::out | std::ios::binary);

	for (const node& e : _src) {
		_fo.put(e.dataValue());
	}
	
	_fo.close();

}



// open a file with a specified file name.
inline const bool openF(std::ifstream& _inF,const char* _file) {
	if (_inF.is_open()) _inF.close();
	_inF.open(_file, std::ios::in | std::ios::binary);
	return _inF.is_open();
}



// close an opened file
inline void closeF(std::ifstream& _inpF) {
	if (_inpF.is_open()) _inpF.close();
}



// print the contents of the file specified by _input
inline void print_file(std::ifstream& _input) {
	int col = 0;

	while (_input) {
		char ch = _input.get();
		col++;
		if (ch) RPRINT(ch);
		if (col > 79) {
			col = 0;
			RET;
		}
	}

	_input.close();
}



// print the contents of the vector data
inline void print_vector(std::vector<char>& _vc, const std::size_t _vSize) {
	int col = 0;

	for (std::size_t i = 0; i < _vSize; i++) {
		col++;
		RPRINT(_vc[i]);
		if (col > 79) {
			col = 0;
			RET;
		}
	}
}


inline void print_hf_rec(std::vector<HF_REC>& _hfc) {
	
	for (const HF_REC& hf : _hfc) {
		Bit x = 0b1, xc = 0b0;

		for (std::size_t j = 0; j < hf._bits.size(); j++) {
			x &= (Bit)hf._bits[j];
			x <<= (7 - j);
			xc |= x;
			x = 0b1;
		}
		mbp.emplace(std::pair<Bit, Byte>(xc, hf._data));
	}

	for (const BPAIR& cp : mbp) {
		printf("% d ,", cp.first);
		printf("% c ,", cp.second);
		RET;
	}

}



// get the total sum of every count bytes in the file
inline const std::size_t tot_f_values(std::ifstream& _input, const char* _file = " ") {
	int cb = 0,_Count = 0;

	while (_input) {
		cb = _input.get();
		if (cb) _Count += cb;
		cb = 0;
	}

	openF(_input, _file); 
	return _Count;
}


// get the total sum of count elements' value in the vector
inline const std::size_t vector_values(std::vector<char>& _vc, const std::size_t _vSize) {
	int _Count = 0;

	for (std::size_t i = 0; i < _vSize; i++)
		_Count += _vc[i];

	return _Count;
}


// read the content of the file into the vector
inline const std::size_t read_v(std::ifstream& _inp, std::vector<NODE_T>& _vc) {
	int ch = 0, _Count = 0;

	while (_inp) {
		ch = _inp.get();
		if (ch) {
			_vc.push_back(ch);
			_Count++;
		}
		ch = 0;
	}

	return _Count;
}



/* Filter the nodes to a separate vector, to obtain a single unique node in each element of
   vector */
inline void filter_V(std::vector<node>& _dest, const std::vector<node>& _src) {
	PRINT("Filtering.. ");

	double _count = 0.000,fc = 0.000;
	const LongRange LENZ = (LongRange)_src.size();
	NODE_T nc;

	if (_src.empty()) return;

	nc = _src[0];

	for (LongRange i = 0; i < LENZ; i++) {
		// comparing value of both side
		if (VALT<Byte>(nc) == (VALT<Byte>(_src[i]))) {
			++_count;
			fc = (_count / CSIZE) * 100.00;
		}
		else {
			nc._freq = fc;
			_count = 0;
			fc = 0.00;
			_dest.emplace_back(nc);
			nc = _src[i];
		}
	}
}



// transform into vector node
inline void transForm(std::vector<node>& _target, const std::vector<NODE_T>& _source) {
	PRINT("Adding.. ");
	for (const NODE_T& e : _source)
		_target.emplace_back(e);
}



// sort the nodes based on data or frequency
template < typename T >
inline void sort_V(std::vector<node>& _vn, const std::size_t _vSize) {
	PRINT("Sorting.. "); RET;
	
	if (_vSize > 20)
		merge_sort<T>(_vn, (LongRange)_vSize);
	else
		range_sort<T>(_vn, 0, (LongRange)(_vSize - 1));


	sort_Nodes<T>(_vn, (LongRange)_vSize);
}


// check the frequqncy of each node in the vector
inline void check_nodes_frequency(const std::vector<node>& _fNods) {
	for (const node& e : _fNods) {
		RPRINT(e.dataValue()); RPRINT("("); RPRINT(e.FrequencyData()); RPRINT(")");
		RET;
	}
}




inline void encode_file(std::ifstream& _input) {
	

}



int main(int argc, const char* argv[4]) {

	for (std::size_t i = 0; i < 4; i++)
	{
		if ( !argv[i] ) argv[i] = " ";
	}

	progName = argv[0];
	param1 = argv[1];
	param2 = argv[2];
	param3 = argv[3];

	std::ifstream fi(param2, std::ios::in | std::ios::binary);

	if (!fi.is_open()) {
		PRINT("ERROR: Can't open file !");
		return 0;
	}

	PRINT("processing .. "); RET;
	PRINT("This could take a few minutes.. "); RET;
	
	CSIZE = (LONGFLOAT)read_v(fi, RAWC);
	closeF(fi);

	transForm(NODS, RAWC);
	
	RAWC.clear();

	sort_V<Byte>(NODS, NODS.size());

	filter_V(READYNODS, NODS);

	NODS.clear();
	
	sort_V<double>(READYNODS, READYNODS.size());
	
	PRINT("Generating huffman tree structure..");

	std::unique_ptr<node> ht = nullptr;
	ht.reset((CONST_PTR)huff_tree_create(VNT, READYNODS, READYNODS.size()));

	node* fh = ht.get();

	huffman_encode(HC, fh);

	print_hf_rec(HC);


	RET;
	RET;

	return -1;
};
