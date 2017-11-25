#ifndef  JIUGESTL_HEADFILE_ITERATOR
#define  JIUGESTL_HEADFILE_ITERATOR

//迭代器

#include<type_traits>
#include<cstddef>

namespace JStl{

//五种迭代器
struct input_iterator_tag{};//只读
struct output_iterator_tag{};//只写
struct forward_iterator_tag :public input_iterator_tag{};//前向
struct bidrection_iterator_tag :public forward_iterator_tag {};//双向
struct random_access_iterator_tag :public bidrection_iterator_tag{};//随机

//iterator 模板
template <typename Category,
		  typename T,
	      typename Distance = ptrdiff_t,
		  typename Pointer = T*,
		  typename Reference = T&>
struct iterator
{
	typedef Category		iterator_category;  //迭代器类型，五种之一
	typedef T				value_type;//指向的元素
	typedef Pointer			pointer;//原始指针
	typedef Reference       reference;//所指元素的引用
	typedef Distance        difference_type;//迭代器之差
};

//iterator traits
//萃取迭代器的特性
template<class Iterator>
struct iterator_traits
{
	typedef typename Iterator::iterator_category		 iterator_category;
	typedef typename Iterator::value_type				 value_type;
	typedef typename Iterator::pointer					 pointer;
	typedef typename Iterator::reference				 reference;
	typedef typename Iterator::difference_type			 difference_type;
};

//针对原生指针的模板部分特例化
template <typename T>
struct iterator_traits<T*>
{
	typedef random_access_iterator_tag          iterator_category;
	typedef T									value_type;
	typedef T*									pointer;
	typedef T&									reference;
	typedef ptrdiff_t							difference_type;
};

template <typename T>
struct iterator_traits<const T*>
{
	typedef random_access_iterator_tag          iterator_category;
	typedef T									value_type;
	typedef const T*							pointer;
	typedef const T&							reference;
	typedef ptrdiff_t							difference_type;
};
	
//萃取iterator的category
template <typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
	typedef typename iterator_traits<Iterator>::iterator_category category;
	return category();
}

//萃取iterator的distance_type
template <typename Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

//萃取iterator的value_type
template <typename Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

//计算iterator的距离
template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
_distance(InputIterator first,InputIterator last,input_iterator_tag)
{
	typename iterator_traits<InputIterator>::difference_type n;
	while (first != last){
		++first;
		++n;
	}
	return n;
}

template <typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
_distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{
	return last - first;
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
	return _distance(first, last, typename iterator_traits<InputIterator>::iterator_category());
}


}//namespace JStl
#endif