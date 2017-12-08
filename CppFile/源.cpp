#include"../HeadFile/iterator.h"
#include"../HeadFile/alloactor.h"
#include"../HeadFile/construct.h"
#include"../HeadFile/vector.h"
#include<iostream>
#include<vector>
#include<iterator>
#include<cassert>
using namespace std;
struct a{
	a(int s) :as(s){}	
	int as;
};

int main()
{
	JStl::vector<a> c{ 1, 2, 3 };
	JStl::vector<a> b(2, 3);
	cout << b.begin()->as;
	system("pause");
}