#ifndef  JIUGESTL_HEADFILE_HEAPALGO
#define  JIUGESTL_HEADFILE_HEAPALGO

#include<iostream>

#include "algobase.h"
#include "util.h"

// 包含 heap 的四个算法 : push_heap, pop_heap, sort_heap, make_heap

namespace JStl{

//将it上浮到合适的位置
template <class RandomIter>
void swim_aux(RandomIter first,RandomIter it)
{
	size_t i = it - first + 1;
	for (; i > 1 && *it > *(first + i / 2 - 1); i = it - first + 1){
		swap(*it, *(first + i / 2 - 1));
		it = first + i / 2 - 1;
	}
}

template <typename RandomIter, typename Compared>
void swim_aux(RandomIter first, RandomIter it, Compared comp)
{
	size_t i = it - first + 1;
	for (; i > 1 && comp(*(first + i / 2 - 1),*it); i = it - first + 1){
		swap(*it, *(first + i / 2 - 1));
		it = first + i / 2 - 1;
	}
}

//将first下沉到合适位置
template <typename RandomIter>
void sink_aux(RandomIter first, RandomIter last)
{
	auto it = first;
	size_t size = last - first;
	for (size_t i = 1; i * 2 <= size; i *= 2){//存在子节点
		int j = 2 * i - 1;
		if (j < size &&  *(first + j) < *(first + j + 1))//如果j<size,说明有两个子节点
			++j;
		if (*(first + j) < *it)
			return;
		swap(*it, *(first + j));
		it = first + j;
	}
}

template <typename RandomIter, typename Compared>
void sink_aux(RandomIter first, RandomIter last, Compared comp)
{
	auto it = first;
	size_t size = last - first;
	for (size_t i = 1; i * 2 <= size; i *= 2){//存在子节点
		int j = 2 * i - 1;
		if (j < size &&  comp(*(first + j),*(first + j + 1)))//如果j<size,说明有两个子节点
			++j;
		if (comp(*(first + j),*it))
			return;
		swap(*it, *(first + j));
		it = first + j;
	}
}

/*****************************************************************************************/
// make_heap
// 该函数接受两个迭代器，表示 heap 容器的首尾，把容器内的数据变为一个 heap
/*****************************************************************************************/
//默认函数堆顶为最大值
template <typename RandomIter>
void make_heap(RandomIter first, RandomIter last)
{
	for (auto it = first; it != last; ++it){
		swim_aux(first, it);
	}
}

template <typename RandomIter,typename Compared>
void make_heap(RandomIter first, RandomIter last,Compared comp)
{
	for (auto it = first; it != last; ++it){
		swim_aux(first, it, comp);
	}
}

}//namesapce JStl
#endif