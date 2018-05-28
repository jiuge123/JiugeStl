#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include"../HeadFile/list.h"
#include"../HeadFile/deque.h"
#include"../HeadFile/functional.h"
#include"../HeadFile/util.h"
#include"../HeadFile/array.h"
#include"../HeadFile/forward_list.h"
#include"../HeadFile/stack.h"
#include<iostream>
#include<deque>
#include<string>
#include<vector>
#include<list>
#include<array>
#include<stack>
#include<algorithm>
#include<forward_list>
using std::cout;
using std::endl;
using namespace JStl;
int main()
{
	stack<int, list<int>> b{ 1, 2, 3 };
	cout << b.size();
	cout << b.top();
	system("pause");
}