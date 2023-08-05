Using Licence GPL v. 3.0
#pragma once


static BNode* pRoot1 = nullptr;
static BNode* pRoot2 = nullptr;


struct AVL {

	static UINT const LT_Count() { return LT; }
	static UINT const RT_Count() { return RT; }

	static void set_LT_Count(const UINT nc) { LT = nc; }
	static void set_RT_Count(const UINT nc) { RT = nc; }

	static void reset_data() {
		LT = 0; RT = 0; BAL = 0;
	}

	static void compute_balance() {
		BAL = (LT > RT) ? LT - RT : RT - LT;
	}

	static void make_balance() {
		if (LT > RT) R_TURNS();
		else L_TURNS();

	}

	static UINT const balance_value() {
		return BAL;
	}

	
private:
	static UINT LT; // maximum in the Left Branches
	static UINT RT; // maximum in the Right Branches
	static UINT BAL; // difference between LT & RT

	// rotate left branches to the right of the root.
	static void R_TURNS();

	// rotate right branches to the left of the root.
	static void L_TURNS();

}; // END Of struct AVLX DEfs...


// Static AVL's members initialization
unsigned int AVL::LT = 0;
unsigned int AVL::RT = 0;
unsigned int AVL::BAL = 0;

	
