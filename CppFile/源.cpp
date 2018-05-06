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
#include<forward_list>
using std::cout;
using std::endl;
using namespace JStl;
int main()
{
	deque<int> a{ 1, 23, 56 };
	list<int> b{ 1, 23, 56 };
	a.insert(a.begin(),{ 2, 4, 5 });
	for (auto c : a){
		cout << c<<endl;
	}
	system("pause");
}
