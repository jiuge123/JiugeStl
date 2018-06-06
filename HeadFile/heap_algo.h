#ifndef  JIUGESTL_HEADFILE_HEAPALGO
#define  JIUGESTL_HEADFILE_HEAPALGO

#include<iostream>

#include "algobase.h"
#include "util.h"

// ���� heap ���ĸ��㷨 : push_heap, pop_heap, sort_heap, make_heap

namespace JStl{

//��it�ϸ������ʵ�λ��
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

//��first�³�������λ��
template <typename RandomIter>
void sink_aux(RandomIter first, RandomIter last)
{
	auto it = first;
	size_t size = last - first;
	for (size_t i = 1; i * 2 <= size; i *= 2){//�����ӽڵ�
		int j = 2 * i - 1;
		if (j < size &&  *(first + j) < *(first + j + 1))//���j<size,˵���������ӽڵ�
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
	for (size_t i = 1; i * 2 <= size; i *= 2){//�����ӽڵ�
		int j = 2 * i - 1;
		if (j < size &&  comp(*(first + j),*(first + j + 1)))//���j<size,˵���������ӽڵ�
			++j;
		if (comp(*(first + j),*it))
			return;
		swap(*it, *(first + j));
		it = first + j;
	}
}

/*****************************************************************************************/
// make_heap
// �ú���������������������ʾ heap ��������β���������ڵ����ݱ�Ϊһ�� heap
/*****************************************************************************************/
//Ĭ�Ϻ����Ѷ�Ϊ���ֵ
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