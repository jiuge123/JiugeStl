#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include"../HeadFile/list.h"
#include"../HeadFile/functional.h"
#include<iostream>
#include<list>
#include<vector>
using namespace std;
template<class T,class T2>
struct fpair{
	typedef T first_type ;
	typedef T2 second_type ;
	int first =4;
	double second=9.3;
};
int main()
{
	JStl::list<int> a{ 1, 2 };
	JStl::vector<int> b{ 1, 2 ,3};
	list<int> c{ 1, 2 };
	JStl::list<int> d{ 1,2 ,3};
	JStl::selectsecond <  fpair < int, float > > f;
	fpair<int, float> g;
	cout << f(g);
	system("pause");
}
