
#include <iostream>
#include <cstdio>
#include "mixhuff.h"

/*
	Parameters for main()
	[0]:	Program executable name.

	[1]:	Parameters for calling the program, this can
			be '-q' for compressing and '-d' for extracting.

	[2]:	Input file, the file to be compressed using huffman algorithm.

	[3]:	Output file, the file that contains the encoded format of the original
			to which the huffman encoding is being applied on.
*/


/* USAGES:
*   This program is compiled & built with Ms.Visual Studio 2022 targeted on x64 platforms.
* 
*	Type the following commands in the cmd shell:
* 
*   C:\> squzip -q [File.txt] 
	  the output compressed file will automatically be generated and named as 'File.szp'.

	C:\> squzip -q [File.txt] [<File.szp>]
		the output compressed file's name is determined by your choice.

	Examples:
	 C:\> squzip -q test_essay.txt
	 C:\> squzip -q test_essay.txt myEssay.txt

	 During the process, a deniable error messages may promps out, this maybe due to
	 the 32 bit environment of the cmd shell quite inresponsive to the bunches of data 
	 with extra bit being streamed in/out from/to stdin/stdout.

	The Decoding / Uncompress funtionality is still being working progress.
*/



// the number of bytes read
static LONGFLOAT CSIZE = 0.00;

// number of data items read
static std::size_t RECS = 0;

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

// input & output file pointers
static std::FILE* _INF = nullptr , *_OUT = nullptr;

// a Bit to Byte data pair of a vector
static BPAIR bitPair;


#define ZEROES(var1, var2) var1 = var2 = 0.00


// open a file with the specified file name.
inline const bool openF(const char* _file, const char* _sMode = "rb") {
	if (_INF) fclose(_INF);
	if (_OUT) fclose(_OUT);

	if (!std::strcmp(_sMode, "rb"))
		_INF = fopen(_file, _sMode);
	else 
		 _OUT = fopen(_file, _sMode);


	return ( _INF || _OUT );
}



// close the opened file
inline void closeF(std::FILE* _ionf) {
	if (_ionf)
		fclose(_ionf);
}




// Save huffman records 'HC' as header to the output compressed file.
inline const std::size_t save_encoded_data(const char* _file, const std::vector<BPAIR>& _src) {
	const std::size_t LENX = _src.size();
	
	if (!LENX) {
		PRINT("Error: not a valid number of data size");
		return 0;
	}


	if (!openF(_file, "wb")) {
		PRINT("Error: Can't write data to disk.");
		return 0;
	}

	RECS = vbp.size(); 
	fwrite(&RECS, sizeof(std::size_t), 1, _OUT); // write header's size

	for (const BPAIR& _bp : vbp) // write each huffman record to _OUT
		fwrite(&_bp, sizeof(BPAIR), 1, _OUT);

	

	fclose(_OUT);
	return LENX;
}




// print the contents of the file specified by _input
inline void print_file(std::FILE* _input) {
	int col = 0;

	while ( !feof(_input) ) {
		Byte ch = fgetc(_input);
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

		bp._bit = xc;
		bp._cdata = hf._data;
		vbp.push_back(bp);
		hf.reset();
	}

	
	//int col = 0;

	/*
	for (const BPAIR& _cp : vbp) {
		++col;
		RPRINT((char)_cp._cdata); 
		if (col > 79) {
			col = 0;
			RET;
		}
	} */

}


// print the encoded bit patterns and its associated data value
inline void print_encoded_bits(const std::vector<HF_REC>& _hcf) {
	const std::size_t LENC = _hcf.size();
	for (std::size_t i = 0; i < LENC; i++)
		RPRINT((char)_hcf[i]._data);
}



// get the total sum of every count bytes in the file
inline const std::size_t f_sizes(std::FILE* _ionf) {
	std::size_t _Count = 0;

	if (!_ionf) return 0;
	
	while ( !feof(_ionf)  ) {
		if ( fgetc(_ionf) ) ++_Count;
	}
	

	fclose(_ionf);
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

	fclose(_inp);
	return _Count;
}


// adding nodes to the vector
inline void add_V(std::vector<node>& _nodes, const NODE_T _nod) {
	bool _bFound = 0;
	const LongRange _vSize = (LongRange)_nodes.size();
	node _tmp;

	if (_nodes.empty()) {
		if (_nod._v != 0) _nodes.push_back(_nod);
		return;
	}

	if (_vSize < 20)
		_bFound = search_Node(_nodes, _nod._v);

	if (_vSize > 20) {
		sort_Nodes<Byte>(_nodes, _vSize - 1);
		_bFound = vector_search(_nodes, _nod._v, _tmp);
	}

	if (_bFound) return;

	if (_nod._v != 0) _nodes.push_back(_nod);
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



// sort the nodes based on the data or frequency
template < typename T >
inline void sort_V(std::vector<node>& _vn, const std::size_t _vSize) {
	PRINT("Sorting.. "); RET;
	
	if (_vSize > 20)
		merge_sort<T>(_vn, _vSize);
	else
		range_sort<T>(_vn, 0, (LongRange)(_vSize - 1));


	sort_Nodes<T>(_vn, (_vSize-1));
}


// check the frequency of each node in the vector
inline void check_nodes_frequency(const std::vector<node>& _fNods) {
	for (const node& e : _fNods) {
		RPRINT(e.dataValue()); RPRINT("["); RPRINT(e.FrequencyData()); RPRINT(" %fqr]");
		RET;
	}
}


// free the memory from a number of occupied nodes generated from the huffman tree.
inline void release_Vnp(const std::vector<node*>& vNodPtrs) {
	for (std::vector<node*>::iterator _vit = VNT.begin();
		_vit != VNT.end(); _vit++)
		(*_vit)->~node();

}



// using huffman algorithm to encode data read from external media
inline void Encoding() {
	CSIZE = (LONGFLOAT)read_v(_INF, RAWC);
	closeF(_INF);

	transForm(NODS, RAWC);

	RAWC.clear();

	sort_V<Byte>(NODS, NODS.size());

	filter_V(READYNODS, NODS);

	NODS.clear();

	sort_V<double>(READYNODS, (READYNODS.size()));

	huff_tree_create(VNT, READYNODS, READYNODS.size());

	huffman_encode(HC, VNT[vNodeLnk._Last]);

	//print_encoded_bits(HC); // use for debugging
	print_hf_rec(HC);

	RET2();

	PRINT("Completed.");

	READYNODS.clear();
	release_Vnp(VNT);
	VNT.clear();
}



// decode the compressed format of a data read from external media
inline void Decoding(std::string& _fOriginal) {
	std::size_t LENF = std::strlen(_fOriginal.data());
	_fOriginal = _fOriginal.substr(0, LENF - 3);
	_fOriginal = strcat((char*)_fOriginal.data(), "dat");

	if (!openF(_fOriginal.data())) {
		PRINT("ERROR: Can't open decoded file.");
		return;
	}

	print_file(_INF);
	closeF(_INF);
}



// encode an original source file to the targeted output compressed format
inline const std::size_t encode_file(std::string& _sInput, std::string& _sOutput) {
	Byte ch = 0;
	std::size_t LENF = 0;

	if (_sInput.empty() && _sOutput.empty()) {
		PRINT("Uncomplete path or file name !");
		PRINT("Could not proceed.. ");
		return 0;
	}

	const std::size_t LENZ = std::strlen(_sInput.data());

	if (_sOutput.empty()) {
		_sOutput = _sInput.substr(0, LENZ - 3);
		_sOutput = strcat((char*)_sOutput.data(), "szp");
	}
	
	// save huffman records 'HC' as header to the ouput file.
	LENF = save_encoded_data(_sOutput.data(), vbp);

	vbp.clear();


	if (!openF(_sInput.data() ) ) {  // read to _INF
		PRINT("ERROR: Can't open original source file.");
		return 0;
	}

	if (!openF(_sOutput.data(), "wa")) {  // write append to _OUT
		PRINT("ERROR: Can't write data to disk.");
		return 0;
	}

	while (!feof(_INF)) { //_INF refers to original source file
		ch = fgetc(_INF);
		if (vector_search(vbp, ch,bitPair)) { // Searches a Byte data value
			// Write the encoded bit to a '*.szp' file
			LENF += fwrite(&bitPair._bit, sizeof(Bit), 1, _OUT);
		}
		
	}

	fclose(_INF);
	fclose(_OUT);
	return LENF;
}



// open a compressed file and decodes it to the original .dat format
inline const std::size_t decode_file(std::string& _fEnc) {
	Bit uc = 0;
	std::string _fOrig;
	std::size_t LEN_ORIG = 0,_FSIZE = 0;

	// _fEnc refers to any *.szp files
	if (_fEnc.empty()) {
		PRINT("ERROR : Encoded source file not found !");
		PRINT("Could not proceed..");
		return 0;
	}

	LEN_ORIG = std::strlen(_fEnc.data());
	_fOrig = _fEnc.substr(0, LEN_ORIG - 3);
	_fOrig = strcat((char*)_fOrig.data(), "dat");


	// _INF is a pointer to a *.szp opened file
	if (!openF(_fEnc.data() ) ) {
		PRINT("ERROR: Can't open file !");
		PRINT("Make sure the source paths & file's name are correct.");
		return 0;
	}

	// read huffman records into vbp
	if (_INF) { // _INF refers to a *.szp file
		if (!vbp.empty()) vbp.clear();

		_FSIZE += fread(&RECS,sizeof(std::size_t),1, _INF); // Read header's size
		
		for (std::size_t i = 0; i < RECS; i++)
		{  // read each huffman record into bitPair and saved it to a vector
			_FSIZE += fread(&bitPair, sizeof(BPAIR), 1, _INF);
			vbp.push_back(bitPair);
		}
	}
	else
	{
		PRINT("ERROR: invalid or unrecognized data format.");
		PRINT("Could not read to end of file !");
		fclose(_INF);
		return 0;
	}


	// Now, we've got the header's data, next we would
	// create a new file that's used to be written with the original data.
	if (!openF(_fOrig.data(), "wb")) { // opened as _OUT
		PRINT("ERROR: Can't create original output file.");
		closeF(_INF);
		return 0;
	}

	while (!feof(_INF)) { 
		// _INF is now pointing to any location in the *.szp file
		uc = fgetc(_INF); // read each huffman bit into ch
		if (vector_search(vbp, uc, bitPair)) { // searches a Bit value
			// Write the corresponding Byte data to _OUT
			_FSIZE += fwrite(&bitPair._cdata, sizeof(Byte), 1, _OUT);
		}
		
	}

	closeF(_INF);
	closeF(_OUT);

	return _FSIZE;
}



/*  SQUZIP's MAIN FUNCTION */

int main(int argc, const char* argv[4]) {
	int LENC = 0;

	for (int i = 0; i < 4; i++)
	{
		if ( !argv[i] ) argv[i] = "\0";
	}

	progName = argv[0]; // program's name
	param1 = argv[1];  // parameter for main()
	param2 = argv[2];  // input file
	param3 = argv[3];  // output file


		if (!openF(param2.data())) {
			PRINT("ERROR: Can't open file !");
			return 0;
		}
		else {
			PRINT("Processing .. ");
			PRINT("this could take a few minutes.. "); RET;
		}
	

	if (!std::strcmp("-q", argv[1])) {
		Encoding();
		LENC = (int)encode_file(param2, param3);
	}
	else if (!std::strcmp("-d", argv[1])) {
		decode_file(param2);
		RPRINT("Number of records: "); RPRINT(RECS); RET;
		Decoding(param2);
	}
	
	else
		PRINT("ERROR: Unrecognized parameters or command !");

	
	
	closeF(_INF);
	closeF(_OUT);

	RET2();
	return LENC;
};
