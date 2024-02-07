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

static std::size_t CSIZE = 0;
static std::string progName, param1, param2, param3;
static std::vector<char> RAWC;
static std::vector<HF_REC> HC;
static std::vector<node*> VNT;
static std::vector<node> NODS,FNODS, READYNODS;
static std::map<Bit, Byte> mbp;
static node* d_Tree = nullptr;




void save_data(const char* _file,std::vector<node>& _src) {
	std::ofstream _fo(_file, std::ios::out | std::ios::binary);

	for (const node& e : _src) {
		_fo.put(e.dataValue());
	}
	
	_fo.close();

}



// open a file with a specified file name.
const bool openF(std::ifstream& _inF,const char* _file) {
	if (_inF.is_open()) _inF.close();
	_inF.open(_file, std::ios::in | std::ios::binary);
	return _inF.is_open();
}



// close an opened file
void closeF(std::ifstream& _inpF) {
	if (_inpF.is_open()) _inpF.close();
}



// print the contents of the file specified by _input
void print_file(std::ifstream& _input) {
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
void print_vector(std::vector<char>& _vc, const std::size_t _vSize) {
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


void print_hf_rec(std::vector<HF_REC>& _hfc) {
	
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
const std::size_t tot_f_values(std::ifstream& _input, const char* _file = " ") {
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
const std::size_t vector_values(std::vector<char>& _vc, const std::size_t _vSize) {
	int _Count = 0;

	for (std::size_t i = 0; i < _vSize; i++)
		_Count += _vc[i];

	return _Count;
}


// read the content of the file into the vector
const std::size_t read_v(std::ifstream& _inp, std::vector<char>& _vc) {
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



// Plot the read data on the tree
void plot_on_tree(const std::size_t _Size) {
	d_Tree = (CONST_PTR)ALLOC_N<Byte>((Byte)CSIZE);
	node::setRoot(ROOT2P(&d_Tree));
	node::setSize(RAWC.size());

	for (const auto& c : RAWC)
		d_Tree->Add(c);

}



// create a vector nodes from data collected from the tree
void create_frequency_vector_nodes() {
	for (const auto& c : RAWC)
		FNODS.emplace_back(ANODE(c));


	RAWC.clear();
}



void sort_V(std::vector<node>& _vn, const std::size_t _vSize) {
	printf("\n sorting.. \n");
	
	if (_vSize > 20)
		merge_sort<double>(_vn, (LongRange)_vSize);
	else
		range_sort<double>(_vn, 0, (LongRange)(_vSize - 1));


	sort_Nodes<double>(_vn, (LongRange)_vSize);
}



void encode_file(std::ifstream& _input) {
	

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

	std::cout << "processing .. " << "\n\n";
	std::cout << "This could take a few minutes.. " << "\n\n";
	
	CSIZE = read_v(fi, RAWC);

	plot_on_tree((Byte)CSIZE);

	create_frequency_vector_nodes();

	sort_V(FNODS, FNODS.size());

	filter_Nodes(READYNODS, FNODS);

	FNODS.clear();

	sort_Nodes<double>(READYNODS, READYNODS.size());


	PRINT("completed..");
	PRINT("generating huffman tree structure.. ");
	RET;

	std::unique_ptr<node> ht = nullptr;
	ht.reset((CONST_PTR)huff_tree_create(VNT, READYNODS, READYNODS.size()));
	
	node* fh = ht.get();

	huffman_encode(HC, fh);

	print_hf_rec(HC);

	
	closeF(fi);

	
	RET;
	RET;
	RET;
	node::Dispose();

	return -1;
};
