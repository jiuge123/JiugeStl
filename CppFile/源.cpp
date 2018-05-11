#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include"../HeadFile/list.h"
#include"../HeadFile/deque.h"
#include"../HeadFile/functional.h"
#include"../HeadFile/util.h"
#include"../HeadFile/array.h"
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

	array<int, 3> d,f;
	d.assign(3);
	f.assign(5);
	cout << (d >= f);
	system("pause");
}