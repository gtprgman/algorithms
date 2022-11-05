#pragma once
#include "mixutil.h"


using namespace mix;
using namespace mix::ptr_type;
using namespace mix::smart_ptr;
using namespace mix::data;



// custom deleter for single unique pointer
template < class TP >
struct del_unique
{
	bool operator()(TP* _tp) {
		std::cout << "user custom deleter called .. " << std::endl;
		static_assert(sizeof(_tp) > 0, "invalid pointer argument for '_tp' ");
		delete _tp;
		return (nullptr == _tp);
	}
};



int main(void)
{
	
	Bucket b1("b1"), b2("b2");
	Bucket cpyB2 = b2;
	
	// with custom deleter
	uniqueP<Bucket, del_unique<Bucket>>&& upb = up_create<Bucket,del_unique<Bucket>>();

	upb.reset(new Bucket()); // allocate space for the Bucket

	std::cout << "a new bucket just created on the heap, \n";
	std::cout << "and be uniquely owned by 'upb'. \n";
	std::cout << "data for 'upb': " << upb->data() << std::endl;

	std::cout << "\n\n";

	std::cout << "Exclusively owns the bucket object 'b1' " << std::endl;
	std::cout << " ' *upb = b1; ' \n";
	*upb = b1;

	std::cout << "data for the acquired Bucket: " << upb->data() << std::endl;

	std::cout << "\n\n";

	// uses the default deleter
	uniqueP<Bucket>&& ub2 = up_create<Bucket>();

	ub2.reset(new Bucket());

	std::cout << "a new Bucket just created on the heap, \n";
	std::cout << "and be uniquely owned by 'ub2' . \n";
	std::cout << "data for 'ub2': " << ub2->data() << std::endl;
	
	std::cout << "\n Exclusively owns the 'b2' object. \n";
	std::cout << " ' *ub2 = b2; ' \n";
	*ub2 = b2;

	std::cout << "data for 'ub2:' " << ub2->data() << std::endl;


	std::cout << "\n\n";
	system("PAUSE");
	return 0;
	
	
}





