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
using std::cout;
using std::endl;
using namespace JStl;
int main()
{
	int a1[256];
	int a2[256];
	int a3[256];
	for (int i = 0; i < 256;i++){
		a1[i] = i;
		a2[i] = i+256;
		a3[i] = i+512;
	}
	int *b2[] = { a1, a2, a3 };
	deque_iterator<int, int&, int*> a(a1, b2), b(a2, b2 + 1);
	system("pause");
}
