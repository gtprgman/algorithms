/* Using Licence: GPL v 3.0 */
#pragma once



template <typename P>
struct AVL {

	using DType = typename NodeTraits<std::remove_pointer_t<P>>::memberType;

	static typename NodeTraits<P>::p_type pRoot1;
	static typename NodeTraits<P>::p_type pRoot2;

	static typename NodeTraits<P>::p_type newRoot1;
	static typename NodeTraits<P>::p_type newRoot2;

	static constexpr UINT const LT_Count() { return LT; }
	static constexpr UINT const RT_Count() { return RT; }

	static constexpr void set_LT_Count(const UINT nc) { LT = nc; }
	static constexpr void set_RT_Count(const UINT nc) { RT = nc; }

	static constexpr void reset_data() {
		LT = 0; RT = 0; BAL = 0;
	}

	static constexpr void compute_balance() {
		BAL = (LT > RT)? LT - RT : RT - LT;
	}

	static constexpr void make_balance() {
		if (LT > RT) R_TURNS();
			else L_TURNS();
	}

	static constexpr UINT const balance_value() {
		return BAL;
	}

	
private:
	static UINT LT; // maximum in the Left Branches
	static UINT RT; // maximum in the Right Branches
	static UINT BAL; // difference between LT & RT

	// rotate left branches to the right of the root.
	static constexpr void R_TURNS();

	// rotate right branches to the left of the root.
	static constexpr void L_TURNS();

}; // END Of struct AVLX DEfs...


// AVL<P> Members' Initialization..
template <typename P>
UINT AVL<P>::LT = 0;

template <typename P>
UINT AVL<P>::RT = 0;

template <typename P>
UINT AVL<P>::BAL = 0;

template<typename P>
typename NodeTraits<P>::p_type AVL<P>::pRoot1 = nullptr;

template <typename P>
typename NodeTraits<P>::p_type AVL<P>::pRoot2 = nullptr;

template <typename P>
typename NodeTraits<P>::p_type AVL<P>::newRoot1 = nullptr;

template <typename P>
typename NodeTraits<P>::p_type AVL<P>::newRoot2 = nullptr;




