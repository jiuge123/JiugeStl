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

//判断是否为迭代器
//通过模板偏特化来进行判断，如果表达式为真，则继承下面第二个函数，
//那个函数继承的constant_integral的value是false；
using std::true_type;
using std::false_type;

template <typename Iter, bool b>
struct cat_Iter:false_type
{};

template <typename Iter>
struct cat_Iter<Iter,true>:true_type
{};

//如果能够转化为inputIter或者是个指针
template <typename Iter>
struct is_input_iterator:cat_Iter<Iter,std::is_convertible<Iter,input_iterator_tag>
							  ::value||std::is_pointer<Iter>::value>{};

template <typename Iter>
struct is_forward_iterator:cat_Iter<Iter, std::is_convertible<Iter, forward_iterator_tag>
							   ::value || std::is_pointer<Iter>::value>{};

template <typename Iter>
struct is_bidrection_iterator:cat_Iter<Iter, std::is_convertible<Iter, bidrection_iterator_tag>
								  ::value || std::is_pointer<Iter>::value>{};

template <typename Iter>
struct is_random_access_iterator:cat_Iter<Iter, std::is_convertible<Iter, random_access_iterator_tag>
									 ::value || std::is_pointer<Iter>::value>{};

template <typename Iter>
struct is_iterator :cat_Iter<Iter, std::is_convertible<Iter, input_iterator_tag>::value 
								|| std::is_convertible<Iter, output_iterator_tag>::value 
								|| std::is_pointer<Iter>::value>{};
/**************************************************************************************/
//计算两个iterator的距离
/**************************************************************************************/
template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_iterator_tag)
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
__distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{
	return last - first;
}

template <typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last)
{
	return __distance(first, last, typename iterator_traits<InputIterator>::iterator_category());
}

/**************************************************************************************/
//用于迭代器前进或后退n个位置
/**************************************************************************************/

template <typename InputIterator,class Distance>
inline void __advance(InputIterator &i, Distance n, input_iterator_tag)
{
	while (n--){
		++i;
	}		
}

template <typename BidirectionalIterator,class Distance>
inline void __advance(BidirectionalIterator &i,Distance n,bidrection_iterator_tag)
{
	if (n > 0){
		while (n--){
			++i;
		}
	}
	else {
		while (n--){
			--i;
		}
	}
}

template <typename RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator &i, Distance n, random_access_iterator_tag)
{
	i += n;
}

template <typename InputIterator, class Distance>
inline void advance(InputIterator &i, Distance n)
{
	__advance(i, n, typename iterator_traits<InputIterator>::iterator_category());
}



template <typename Iterator>
class reverse_iterator{
private:
	Iterator cur;  //正向迭代器
public:
	typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
	typedef typename iterator_traits<Iterator>::value_type        value_type;
	typedef typename iterator_traits<Iterator>::difference_type   difference_type;
	typedef typename iterator_traits<Iterator>::pointer           pointer;
	typedef typename iterator_traits<Iterator>::reference         reference;

	typedef Iterator                                              iterator_type;
	typedef reverse_iterator<Iterator>                            self;

public:
	//构造函数
	reverse_iterator() = default;
	explicit reverse_iterator(iterator_type i) :cur(i){}
	reverse_iterator(const self &rhs) :cur(rhs.cur){}

public:
	iterator_type base() const
	{
		return cur;
	}

	reference operator*() const
	{
		return *(cur - 1);
	}

};

}//namespace JStl
#endif