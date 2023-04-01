#include "mixutil.h"
#include "mixer.h"




int main() {
	
	U_ARRAY<int> upn = MK_U_ARRAY<int>(3);
	
	upn[0] = 190; upn[1] = 209; upn[2] = 315;

	smart_print(upn.get(), upn.get() + 3);
	
	std::cout << "\n\n";

	S_ARRAY<Bucket> sbc = MK_S_ARRAY<Bucket>(2);

	smart_print(sbc.get(), sbc.get() + 2);

	printf("\n\n");

	U_ARRAY<Bucket> ubc = MK_U_ARRAY<Bucket>(3);

	smart_print(ubc.get(), ubc.get() + 3);

	std::cout << "\n\n";

	
	_init_p<int*>::initialize(upn.get(), {200,400,600});

	smart_print(upn.get(), upn.get() + 3);

	printf("\n\n");

	_init_p<Bucket*>::initialize(ubc.get(), { Bucket("One"),Bucket("2"),Bucket("Three") });

	smart_print(ubc.get(), ubc.get() + 3);

	printf("\n\n");

	_init_p<Bucket*>::initialize(sbc.get(), { Bucket("0"),Bucket("1") });

	smart_print(sbc.get(), sbc.get() + 2);

	printf("\n\n");
	
	

	return 0;
}


