/* Using Licence: GPL v 3.0 */
#pragma once


template <typename P>
struct AVL {

	using DType = typename NodeTraits<P>::memberType;

	static typename NodeTraits<P>::p_type pRoot1;
	static typename NodeTraits<P>::p_type pRoot2;

	static typename NodeTraits<P>::p_type newRoot1;
	static typename NodeTraits<P>::p_type newRoot2;

	constexpr UINT const LT_Count() { return LT; }
	constexpr UINT const RT_Count() { return RT; }

	constexpr void set_LT_Count(const UINT nc) { LT = nc; }
	constexpr void set_RT_Count(const UINT nc) { RT = nc; }

	constexpr void reset_data() {
		LT = 0; RT = 0; BAL = 0;
	}

	constexpr void compute_balance() {
		BAL = (LT > RT) ? LT - RT : RT - LT;
	}

	constexpr void make_balance() {
		if (LT > RT) R_TURNS();
			else L_TURNS();
	}

	constexpr UINT const balance_value() {
		return BAL;
	}

	
private:
	UINT LT; // maximum in the Left Branches
	UINT RT; // maximum in the Right Branches
	UINT BAL; // difference between LT & RT

	// rotate left branches to the right of the root.
	constexpr void R_TURNS();

	// rotate right branches to the left of the root.
	constexpr void L_TURNS();

}; // END Of struct AVLX DEfs...


// AVL<P> Members' Initialization..
template <typename P>
typename NodeTraits<P>::p_type AVL<P>::pRoot1 = nullptr;

template <typename P>
typename NodeTraits<P>::p_type AVL<P>::pRoot2 = nullptr;

template <typename P>
typename NodeTraits<P>::p_type AVL<P>::newRoot1 = nullptr;

template <typename P>
typename NodeTraits<P>::p_type AVL<P>::newRoot2 = nullptr;






