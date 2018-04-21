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
using std::cout;
using std::endl;
using namespace JStl;
int main()
{
	int a1[] = { 1, 2, 3, 4 };
	int a2[] = { 5, 6, 7, 8 };
	int a3[] = { 9, 10, 11, 12 };
	int *b2[] = { a1, a2, a3 };
	deque_iterator<int, int&, int*> a(a1+2,b2),b(a2,b2+1);
	cout << *a;

	system("pause");
}
