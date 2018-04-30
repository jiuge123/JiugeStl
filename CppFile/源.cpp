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
	deque<int> a{ 1, 2, 3,4 };
	for (auto c : a)
		cout << c;
	a.clear();
	for (auto c : a)
		cout << c;
	system("pause");
}
