#ifndef  JIUGESTL_HEADFILE_ITERATOR
#define  JIUGESTL_HEADFILE_ITERATOR

//迭代器

#include<type_traits>
#include<cstddef>

namespace JStl{

//五种迭代器
struct input_iterator_tag{};//输入
struct output_iterator_tag{};//输出
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
	typedef Category			iterator_category;  //迭代器类型，五种之一
	typedef T						value_type;//指向的元素
	typedef Pointer			pointer;//原始指针
	typedef Reference       reference;//所指元素的引用
	typedef Distance         difference_type;//迭代器之差
};

//iterator traits

template <typename T>
struct has_iterator_cat
{
private:
	struct two{ char a; char b; };
	template <class U> static two test(...);
	template <class U>static char test(typename U::iterator_catelory* = 0);
public:
	static const bool value = sizeof(test<T>(0)) == sizeof(char);
};

template <class Iterator,bool>
struct iterator_traits_impl{};

template<class Iterator>
struct iterator_traits_impl<Iterator, true>
{
	typedef typename Iterator::iterator_category		 iterator_category;
	typedef typename Iterator::value_type				 value_type;
	typedef typename Iterator::pointer						 pointer;
	typedef typename Iterator::reference					 reference;
	typedef typename Iterator::difference_type		 difference_type;
};

template <class Iterator,bool>
struct iterator_traits_helper{};

template<class Iterator>
struct iterator_traits_helper<Iterator, true>
	:public iterator_traits_impl<Iterator,
	//是否可以改变
	std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
	std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
{};

//萃取迭代器的特性
template <class Iterator>
struct iterator_traits
	:public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value>{};


}//namespace JStl
#endif