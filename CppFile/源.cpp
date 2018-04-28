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
	list<int> a;
	for (int i = 0; i < 255; i++){
		a.push_back(i);
	}
	deque<int> b(a.begin(), a.end());
	for (int i = 0; i < 2550; i++){
		if (i == 7 * 255-1)
			;
		b.emplace_back(3);
	}
	system("pause");
}
