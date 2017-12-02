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
	int aa[] = { 3, 4, 5, 6 };
	const JStl::vector<a> b(aa,aa+4),c(b);
	c.empty();
	system("pause");
}