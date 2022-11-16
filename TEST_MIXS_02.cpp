
#include "mixutil.h"


using namespace mix;
using namespace mix::ptr_type;
using namespace mix::smart_ptr;
using namespace mix::data;



// custom deleter for single shared pointer
template < class TP >
struct del_shared
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
	// uses default deleter
	shareP<Bucket> spb = sp_create<Bucket>();

	std::cout << "a new share bucket just created on the heap \n";
	std::cout << "data for the new shared bucket: " << spb->data() << "\n\n";

	*spb = Bucket("b1");

	std::cout << "a copy of Bucket object 'b1' is stored \n";
	std::cout << "on the created shared space owned by 'spb '.\n";
	std::cout << "data for the shared bucket: " << spb->data() << "\n\n";

	// added custom deleter
	shareP<Bucket> spb2 = sp_create<Bucket, del_shared<Bucket> >(2);

	std::cout << "a new share bucket is created on the heap \n";
	std::cout << "data for the new shared bucket: " << spb2->data() << "\n\n";

	*spb2 = Bucket("b2");

	std::cout << "a copy of a new created Bucket 'b2 ' \n";
	std::cout << "just stored on the shared space owned by 'spb2' \n";
	std::cout << "data for 'spb2' : " << spb2->data() << "\n\n";



	std::cout << "\n\n";
	
	system("PAUSE");
	return 0;
	
	
}





