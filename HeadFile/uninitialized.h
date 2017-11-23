#ifndef  JIUGESTL_HEADFILE_UNINITIALIZED
#define  JIUGESTL_HEADFILE_UNINITIALIZED
//包括三个范围构造函数

#include "iterator.h"
namespace JStl{
//从迭代器result开始连续构造last-first个对象
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
	
}

//在[first,last)上copy构造。
template<typename ForwardIterator, typename T>
ForwardIterator uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{

}

//从first开始copy构造n个
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{

}
}//namespace JStl

#endif