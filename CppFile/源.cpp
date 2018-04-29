#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include"../HeadFile/list.h"
#include"../HeadFile/deque.h"
#include"../HeadFile/functional.h"
#include"../HeadFile/util.h"
#include<iostream>
#include<deque>
#include<vector>
#include<list>
#include<algorithm>
using std::cout;
using std::endl;
using namespace JStl;
int main()
{
	deque<int> a{ 1, 2, 34 };
	deque<int> b(a);
	b.emplace_back(3);
	cout << b.size();
	system("pause");
}
