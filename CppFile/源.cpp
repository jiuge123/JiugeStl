#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include"../HeadFile/list.h"
#include"../HeadFile/deque.h"
#include"../HeadFile/functional.h"
#include"../HeadFile/util.h"
#include"../HeadFile/array.h"
#include"../HeadFile/forward_list.h"
#include<iostream>
#include<deque>
#include<string>
#include<vector>
#include<list>
#include<array>
#include<algorithm>
#include<forward_list>
using std::cout;
using std::endl;
using namespace JStl;
int main()
{
	std::forward_list<int> d{ 1, 2, 3,6 };
	std::forward_list<int> a(d.begin(),d.end());
	forward_list<int> g{ 1, 2 };
	forward_list<int> b = g;
	cout<<*g.insert_after(g.before_begin(), 9);
	for (auto c : g)
		cout << c;
	
	system("pause");
}