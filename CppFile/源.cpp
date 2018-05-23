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
	std::forward_list<int> d{ 1, 2 ,4,5};
	std::forward_list<int> a(d.begin(),d.end());
	forward_list<int> g{ 1, 2, 5, 7 };
	forward_list<int> b{ 0, 4, 5, 8, 9 };
	b.reverse();
	for (auto c : b)
		cout << c;
	
	system("pause");
}