#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include"../HeadFile/list.h"
#include"../HeadFile/functional.h"
#include"../HeadFile/util.h"
#include<iostream>
#include<list>
#include<vector>
using std::cout;
using std::endl;
struct MyStruct
{
	MyStruct(int a){}
};
int main()
{
	JStl::list<int> a{ 1, 2 };
	JStl::vector<int> b{ 1, 2 ,3};
	std::list<int> c{ 1, 2 };
	JStl::list<int> d{ 1,2 ,3};
	JStl::pair<int, double> f(2, 3.7);
	system("pause");
}
