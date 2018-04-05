#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include"../HeadFile/list.h"
#include<iostream>
#include<list>
using namespace std;
int main()
{
	JStl::list<int> a{ 1, 2, 3 }, b{ 4, 5, 5 };
	a = { 2, 3, 4 };
	for (auto c : a)
	{
		cout << c;
	}
	
	system("pause");
}
