#pragma once

/* Using GPL v 3.0 License */

namespace CMERGES {
	static class RANGE {
	public:
		constexpr RANGE();
		constexpr RANGE(double, double);

		static int Mid();
		static int low();
		static int high();

		static void setLow(double);
		static void setUpper(double);

	private:
		static double _lower, _mid, _upper;
	};
}

double CMERGES::RANGE::_lower = -1.00;
double CMERGES::RANGE::_mid = -1.00;
double CMERGES::RANGE::_upper = -1.00;
