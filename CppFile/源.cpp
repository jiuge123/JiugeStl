#include"../HeadFile/alloc.h"
#include"../HeadFile/alloactor.h"
#include"../HeadFile/construct.h"
#include<iostream>
#include<vector>
using namespace std;
int main()
{	
	int *p = (int*)JStl::Alloc_Poor::allocate(18);
	JStl::construct(p,3);
	JStl::construct(p+4, 6);
	cout << *p<<*(p+4);
	system("pause");
}