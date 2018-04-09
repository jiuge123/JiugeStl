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
	list<int> c{ 1, 2 };
	cout<<*a.insert(a.begin(), {4,9,8});
	for (auto cd : a)
		cout << cd;
	system("pause");
}
