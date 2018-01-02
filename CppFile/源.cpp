#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include<iostream>
#include<vector>
#include<iterator>
using namespace std;
int main()
{
	JStl::vector<int> a{ 1, 2, 3, 4, }, b{ 9, 9 };
	auto it = a.rbegin();
	*it = 6;
	//for ( it != a.rend(); ++it)
		cout << *it<<endl;
	system("pause");
}
