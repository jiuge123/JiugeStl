#include"../HeadFile/alloactor.h"
#include"../HeadFile/iterator.h"
#include<iostream>
#include<vector>
using namespace std;
int main()
{
	JStl::allocator<int> a;
	auto p = a.allocate(3);
	int aa = 3;
	a.construct(p,aa);
	system("pause"); 
}