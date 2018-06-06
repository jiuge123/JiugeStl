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
#include"../HeadFile/heap_algo.h"
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
using std::ends;
using std::endl;
using namespace JStl;
int main()
{
	vector <int > a{ 1,2,3,4,5,6,7,8,9};
	make_heap(a.begin(), a.end(),greater<int>());
	a[0] = 10;
	sink_aux(a.begin(), a.end(), greater<int>());
	for (auto c : a)
		cout << c << ends;
	cout << endl;
	system("pause");
}