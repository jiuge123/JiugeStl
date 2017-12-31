#include"../HeadFile/iterator.h"
#include"../HeadFile/algobase.h"
#include"../HeadFile/vector.h"
#include<iostream>
#include<vector>
#include<iterator>
using namespace std;
int main()
{
	vector<int> a{ 1, 2, 3, 4, }, b{ 9, 9 };
	for (auto it = a.begin(); it != a.end(); ++it)
		cout << *it<<endl;
	system("pause");
}
