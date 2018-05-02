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
	std::deque<int> f;
	vector<int> ff{ 6, 7, 8 };
	int k = 54;
	deque<int> a{ 1, 2, 3, 4 };
	cout<<*a.insert(a.begin()+1, 33);
	for (auto c : a)
		cout << c<<endl;

	system("pause");
}
