#include "mixutil.h"
#include "mixer.h"




int main() {
	int n = 0;
	Bucket b = std::string("b0");
	Bucket* pb = &b;

	int arr[3] = { 250, 122, 690 };
	int(*pArr)[3] = &arr; // pointer to array of [n] elems
	int(&rArr)[3] = arr;	// reference to array of [n] elems

	int* pN = arr;
	int** pNN = &pN;

	struct Car {
		int initEngine() {
			std::cout << "Engine started.. \n";
			return -1;
		}
	};
	
	std::cout << "isPointer('int arr[3]' )		: " << ptrTraits<_TYPE(arr)>::isPointer << "\n";
	std::cout << "isPointer('int (*pArr)[3]' )	: " << ptrTraits<_TYPE(pArr)>::isPointer << "\n";

	std::cout << "isPointer(&rArr): " << ptrTraits<_TYPE(&rArr)>::isPointer << "\n";
	std::cout << "isPointer(pNN): " << ptrTraits<_TYPE(pNN)>::isPointer << "\n";

	std::cout << "---------------------------------------------------------- \n\n ";

	std::cout << "isArray(arr)	: " << ptrTraits<_TYPE(arr)>::isArray << "\n";
	std::cout << "isArray(pArr)	: " << ptrTraits<_TYPE(pArr)>::isArray << "\n";
	std::cout << "isArray(&rArr): " << ptrTraits<_TYPE(&rArr)>::isArray << "\n";
	std::cout << "isArray(pNN)	: " << ptrTraits<_TYPE(pNN)>::isArray << "\n";

	std::cout << "----------------------------------------------------------\n\n";

	std::cout << "arr elems count: " << ptrTraits<_TYPE(arr)>::count << "\n";
	std::cout << "arr array size : " << ptrTraits<_TYPE(arr)>::size << "\n";

	
	Car myCar;

	int (Car::* pToMbr)();
	pToMbr = &Car::initEngine;


	std::cout << "\n Is Pointer to Member ? 'Car::*pToMbr ' : " << 
		ptrTraits<_TYPE(pToMbr)>::isPointerToMember << "\n";

	ptrTraits<_TYPE(pToMbr)>::type mc = pToMbr;


	system("PAUSE");
}