#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include"../HeadFile/list.h"
#include<iostream>
#include<list>
using namespace std;
int main()
{
	JStl::list<int> a{ 1, 2, 3 };
	JStl::list<int> b(std::move(a));
	cout << b.size();
	system("pause");
}
