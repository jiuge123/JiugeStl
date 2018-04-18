#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include"../HeadFile/list.h"
#include"../HeadFile/functional.h"
#include"../HeadFile/util.h"
#include<iostream>
#include<list>
#include<vector>
using std::cout;
using std::endl;
using namespace JStl;
int main()
{
	int b = 6;
	int *a = &b;
	cout<<reinterpret_cast<int>(&a);
	system("pause");
}
