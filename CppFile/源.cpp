#include"../HeadFile/iterator.h"
#include"../HeadFile/alloactor.h"
#include"../HeadFile/construct.h"
#include"../HeadFile/vector.h"
#include<iostream>
#include<vector>
#include<iterator>
using namespace std;
struct a{
	a(int s) :as(s){}
	int as;
};
int main()
{	
	JStl::vector<a> b(5, a(5));
	system("pause");
}