#pragma once
#include "mix_util.h"


using namespace mix;
using namespace mix::ptr_type;
using namespace mix::smart_ptr;
using namespace mix::data;



int main(void)
{
	Bucket b1("b1"), b2("b2");
	Bucket cpyB2 = b2;

	uniqueP<Bucket>&& upb = up_create(Bucket);

	upb->set("b1");

	std::cout << upb->data() << ", " << b2.data() << ", " << cpyB2.data() << std::endl;
	system("PAUSE");
	return 0;
}
