#include"../HeadFile/alloactor.h"
#include"../HeadFile/iterator.h"
#include<iostream>
#include<vector>
using namespace std;
struct  aa
{
	aa() = default;
	aa(int a, int b){}
};
int main()
{
	JStl::allocator<aa> a;
	const aa aaa, bbb;
	auto p = a.allocate();
	a.construct(p,aaa);
	system("pause"); 
}