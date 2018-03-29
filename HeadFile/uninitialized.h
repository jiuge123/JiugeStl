#ifndef  JIUGESTL_HEADFILE_UNINITIALIZED
#define  JIUGESTL_HEADFILE_UNINITIALIZED

//范围构造函数

#include "iterator.h"
#include "construct.h"
#include "algobase.h"

namespace JStl{
/**************************************************************************************/
////从迭代器result开始连续move构造last-first个对象，返回last的iter
/**************************************************************************************/
template<typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_move(InputIterator first, InputIterator last,
	ForwardIterator result, std::true_type)
{
	return move(first, last, result);
}

template<typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_move(InputIterator first, InputIterator last,
	ForwardIterator result, std::false_type)
{
	auto p = result;
	try{
		while (first != last){
			construct(&*p, JStl::move(*first));
			++first;
			++p;
		}
	}
	catch (...){
		while (result != p)
			destroy(result);
		++result;
	}
	return p;
}
	
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_move(InputIterator first, InputIterator last,
	ForwardIterator result)
{
	return __uninitialized_move(first, last, result, std::is_move_assignable	<
		typename iterator_traits<InputIterator>::value_type>{});
}

/**************************************************************************************/
////从迭代器result开始连续构造last-first个对象，返回last的iter
/**************************************************************************************/
template<typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, 
									 ForwardIterator result, std::true_type)
{
	return copy(first, last, result);
}

template<typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, 
									 ForwardIterator result, std::false_type)
{
	auto p = result;
	try{
		while (first != last){
			construct(&*p, *first);
			++first;
			++p;
		}
	}
	catch (...){
		while (result != p)
			destroy(result);
			++result;
	}
	return p;
}

//不判断last是否大于first
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, 
								   ForwardIterator result)
{
	return __uninitialized_copy(first, last, result, std::is_pod<
		typename iterator_traits<InputIterator>::value_type>{});
}

/**************************************************************************************/
//在[first,last)上fill,无返回值
/**************************************************************************************/
template<typename ForwardIterator, typename T>
void __uninitialized_fill(ForwardIterator first, ForwardIterator last, 
						  const T& x, std::true_type)
{
	fill(first, last, x);
}

template<typename ForwardIterator, typename T>
void __uninitialized_fill(ForwardIterator first, ForwardIterator last, 
						  const T& x, std::false_type)
{
	auto p = first;
	//要么全部成功，要么全部失败
	try{
		while (p != last){
			construct(&*p, x);
			++p;		
		}
	}
	catch(...){
		while (first != p){
			destroy(&first);
			++first;
		}
	}
}

template<typename ForwardIterator, typename T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{
	__uninitialized_fill(first, last, x, std::is_pod<
							  typename iterator_traits<ForwardIterator>::value_type>{});
}

/**************************************************************************************/
//从first开始fill构造n个,返回最后的iter
/**************************************************************************************/
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n,
									 const T& x, std::true_type)
{
	return fill_n(first, n, x);
}

template<typename ForwardIterator, typename Size, typename T>
ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n,
								     const T& x, std::false_type)
{
	auto p = first;
	try{
		while (n--){
			construct(&*p, x);
			++p;
		}
	}
	catch (...){
		while (first != p){
			destroy(&*first);
		}
	}
	return p;
}

template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
	return __uninitialized_fill_n(first, n, x, std::is_pod<
						   typename iterator_traits<ForwardIterator>::value_type>{});
}
}//namespace JStl
#endif