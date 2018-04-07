#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include"../HeadFile/list.h"
#include<iostream>
#include<list>
#include<vector>
using namespace std;
int main()
{
	JStl::list<int> a{ 1, 2, 4 };
	JStl::vector<int> b{ 1, 2, 4 };
	a.emplace_front(5);
	for (auto c : a)
		cout << c;
	system("pause");
}
