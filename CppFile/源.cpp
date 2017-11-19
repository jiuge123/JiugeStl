#include"../HeadFile/alloc.h"
#include"../HeadFile/iterator.h"
#include<iostream>
#include<vector>
using namespace std;
int main()
{
	int *p = (int*)JStl::Alloc_Poor::allocate(8);
	system("pause");
}