#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include"../HeadFile/list.h"
#include"../HeadFile/deque.h"
#include"../HeadFile/functional.h"
#include"../HeadFile/util.h"
#include<iostream>
#include<deque>
using std::cout;
using std::endl;
using namespace JStl;
int main()
{
	vector<int> a;
	cout<<is_bidrection_iterator<deque_iterator<int,int*,int&>>::value;
	system("pause");
}
