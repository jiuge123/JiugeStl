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
#include"../HeadFile/queue.h"
#include<iostream>
#include<deque>
#include<string>
#include<vector>
#include<list>
#include<array>
#include<stack>
#include<queue>
#include<algorithm>
#include<forward_list>
using std::cout;
using std::endl;
using namespace JStl;
int main()
{
	queue<int, list<int>> b{ 1, 2, 3 };
	std::queue<int, std::list<int>> d;
	cout<<b.front();
	cout << b.back();
	
	system("pause");
}