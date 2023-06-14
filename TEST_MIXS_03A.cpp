#include <iostream>
#include <string>
#include "mixtree.h"

using P_NODE = BNode*;


int main()
{

	BNode n1 = 120; // call BNode(int );
	BNode n0 = std::move(n1); // call BNode(BNode&& );



	n0.Print();
	n1.Print();


	
}