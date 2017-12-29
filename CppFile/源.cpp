#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include<iostream>
#include<vector>
#include<iterator>
using namespace std;
int main()
{
	JStl::vector<int> a;
	for (auto it = a.begin(); it != a.end(); ++it)
		cout << *it;
	system("pause");
}