#include "mixutil.h"
#include "mixer.h"





int main()
{
	
	int n = 0;
	Bucket b = std::string("b0");
	Bucket* pb = &b;

	int arr[3] = { 250, 122, 690 };
	int (*pArr)[3] = &arr; // pointer to array of [n] elems
 	int (&rArr)[3] = arr;	// reference to array of [n] elems
	
	int* pN = arr;
	int** pNN = &pN;

	ptrTraits<_TYPE(nullptr)>::type nan = 0;
	ptrTraits<_TYPE(&n)>::type pNum = pN;
	ptrTraits<_TYPE(arr)>::type nums = {1895,3259,6175};

	ptrTraits<_TYPE(pArr)>::type nums1 = pArr;
	ptrTraits<_TYPE(&rArr)>::type refPtr = &rArr;
	ptrTraits<_TYPE(pNN)>::type ptr2ptr = pNN;

	ptrTraits<_TYPE(pb)>::type ptrBucket = new Bucket();



	system("PAUSE");
	return 0;
}