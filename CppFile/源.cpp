#include"../HeadFile/util.h"
#include"../HeadFile/alloactor.h"
#include<iostream>
using namespace std;
struct MyStruct
{
	~MyStruct(){}
};
int main()
{
	JStl::Allocator<MyStruct> a;
	auto p  = a.allocate(3);
	auto q = p;
	a.construct(q++);
	a.construct(q++);
	a.construct(q++);
	a.destroy(p,q);
	system("pause");
}