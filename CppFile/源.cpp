#include"../HeadFile/iterator.h"
#include"../HeadFile/alloactor.h"
#include"../HeadFile/construct.h"
#include<iostream>
#include<vector>
using namespace std;
struct a{
	a(int c){

	}
};
int main()
{	
	JStl::iterator_traits<a*>::value_type c = 3;
	system("pause");
}