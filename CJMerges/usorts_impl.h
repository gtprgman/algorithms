#pragma once
/* Using GPL v 3.0 License */

#ifndef UNIQUE_MEM
#define UNIQUE_MEM
#include <memory>
#endif

#ifndef USORTS_MERGES
#define USORTS_MERGES
#include "usorts.h"
#endif


using namespace CMERGES;



constexpr RANGE::RANGE() {
	_lower = -1;
	_mid = -1;
	_upper = -1;
}


constexpr RANGE::RANGE(double lv, double hv) {
	_lower = lv;
	_upper = hv;
	_mid = (int)((_lower + _upper) > 0) ? 0.5 * (_lower + _upper) : -1;
}



int RANGE::Mid() {

	if (_mid< 0)
		_mid = (int)((_lower + _upper) > 0) ? 0.5 * (_lower + _upper) : -1;

	return (int)_mid;
}


int RANGE::low() {
	return (int)_lower;
}



int RANGE::high() {
	return (int)_upper;
}




void RANGE::setLow(double lv) { _lower = lv; }

void RANGE::setUpper(double hv) { _upper = hv; }


static std::unique_ptr<int[]>&& mergesSort(std::unique_ptr<int[]>&& a, int aLength)
{
	int i, c, j, k;
	int mid0 = -1, len = aLength - 1;

	i = 0; k = 0; j = 0; c = -1; 

	RANGE::setLow(0);
	RANGE::setUpper(len);
	mid0 = RANGE::Mid();

	if (mid0 > 0) mid0 = (int)ceil(0.25 * mid0);

	k = 0; j = 0;

	for (; k < mid0; k++) {
		if (a[k] > a[k + 1])
		{
			c = a[k + 1];
			a[k + 1] = a[k];
			a[k] = c;
		}
	}

	i = mid0; j = i;

	RANGE::setLow(mid0);
	RANGE::setUpper(len);
	mid0 = RANGE::Mid();

	for (; j < mid0; j++) {
		if (a[j] > a[j + 1])
		{
			c = a[j + 1];
			a[j + 1] = a[j];
			a[j] = c;
		}
	}


	i = j; k = i;

	RANGE::setLow(i);
	RANGE::setUpper(len);
	mid0 = RANGE::Mid();


	for (; k < mid0; k++) {
		if (a[k] > a[k + 1])
		{
			c = a[k + 1];
			a[k + 1] = a[k];
			a[k] = c;
		}
	}

	i = k; j = i;

	for (; j < len; j++) {
		if (a[j] > a[j + 1])
		{
			c = a[j + 1];
			a[j + 1] = a[j];
			a[j] = c;
		}
	}


	// applying insertion sort on each half of the set
	RANGE::setLow(0);
	RANGE::setUpper(len);
	mid0 = RANGE::Mid();

	i = 0;

	for (; i <= len; ) {
		for (; i < mid0; i++) {
			j = i; k = j + 1;

			do {
				if (a[j] > a[k])
				{
					c = a[k];	// smaller value
					a[k] = a[j];
					a[j] = c;	// put smaller value on to the top
				}
				--j; --k;
			} while (j >= 0);
		}
	
		RANGE::setLow(mid0);
		RANGE::setUpper(len);
		mid0 = RANGE::Mid();

		if (i == len) break;
		else
		{
			i = mid0 - 1;
			mid0 = len;
		}
	}


	return std::move(a);
}
