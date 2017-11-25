#include"../HeadFile/iterator.h"
#include"../HeadFile/alloactor.h"
#include"../HeadFile/construct.h"
#include<iostream>
#include<vector>
#include<iterator>
using namespace std;
template <typename T,typename U>
struct iter :public JStl::iterator<T, U>
{

};
int main()
{	
	iter<JStl::bidrection_iterator_tag, int> i;
	JStl::distance(i, i);
	system("pause");
}