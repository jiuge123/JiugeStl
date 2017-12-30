#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include<iostream>
#include<vector>
#include<iterator>
using namespace std;
int main()
{
	JStl::vector<int> a{ 1, 2, 3, 4, 5, 6 }, b{ 9, 9 };
	a.push_back(7);
	//cout<<*JStl::copy_backward(b.begin(), b.end(), a.begin());
	//a.insert(a.begin(), 3,3);
	for (auto it = a.begin(); it != a.end(); ++it)
		cout << *it;
	system("pause");
}