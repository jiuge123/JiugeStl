#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include"../HeadFile/list.h"
#include"../HeadFile/functional.h"
#include<iostream>
#include<list>
#include<vector>
using namespace std;
int main()
{
	JStl::list<int> a{ 1, 2,2,2,2,3,4, 4 };
	JStl::vector<int> b{ 1, 2, 4,8,32,124 };
	list<int> c{ 1, 2 };
	JStl::list<int> d{ 3, 3, 3, 8, 32, 124 };
	a.merge(d);
	for (auto cd : a)
		cout << cd;
	cout <<"\n"<< a.size()<<endl; 
	for (auto cd : d)
		cout << cd;
	cout << "\n" << d.size();
	system("pause");
}
