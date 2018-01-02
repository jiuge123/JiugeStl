#ifndef  JIUGESTL_HEADFILE_ITERATOR
#define  JIUGESTL_HEADFILE_ITERATOR

//������

#include<type_traits>
#include<cstddef>

namespace JStl{

//���ֵ�����
struct input_iterator_tag{};//ֻ��
struct output_iterator_tag{};//ֻд
struct forward_iterator_tag :public input_iterator_tag{};//ǰ��
struct bidrection_iterator_tag :public forward_iterator_tag {};//˫��
struct random_access_iterator_tag :public bidrection_iterator_tag{};//���

//iterator ģ��
template <typename Category,
		  typename T,
	      typename Distance = ptrdiff_t,
		  typename Pointer = T*,
		  typename Reference = T&>
struct iterator
{
	typedef Category		iterator_category;  //���������ͣ�����֮һ
	typedef T				value_type;//ָ���Ԫ��
	typedef Pointer			pointer;//ԭʼָ��
	typedef Reference       reference;//��ָԪ�ص�����
	typedef Distance        difference_type;//������֮��
};

//iterator traits
//��ȡ������������
template<class Iterator>
struct iterator_traits
{
	typedef typename Iterator::iterator_category		 iterator_category;
	typedef typename Iterator::value_type				 value_type;
	typedef typename Iterator::pointer					 pointer;
	typedef typename Iterator::reference				 reference;
	typedef typename Iterator::difference_type			 difference_type;
};

//���ԭ��ָ���ģ�岿��������
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
	
//��ȡiterator��category
template <typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
	typedef typename iterator_traits<Iterator>::iterator_category category;
	return category();
}

//��ȡiterator��distance_type
template <typename Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

//��ȡiterator��value_type
template <typename Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&)
{
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

//�ж��Ƿ�Ϊ������
//ͨ��ģ��ƫ�ػ��������жϣ�������ʽΪ�棬��̳�����ڶ���������
//�Ǹ������̳е�constant_integral��value��false��
using std::true_type;
using std::false_type;

template <typename Iter, bool b>
struct cat_Iter:false_type
{};

template <typename Iter>
struct cat_Iter<Iter,true>:true_type
{};

//����ܹ�ת��ΪinputIter�����Ǹ�ָ��
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
//��������iterator�ľ���
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
//���ڵ�����ǰ�������n��λ��
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
	Iterator cur;  //���������
public:
	typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
	typedef typename iterator_traits<Iterator>::value_type        value_type;
	typedef typename iterator_traits<Iterator>::difference_type   difference_type;
	typedef typename iterator_traits<Iterator>::pointer           pointer;
	typedef typename iterator_traits<Iterator>::reference         reference;

	typedef Iterator                                              iterator_type;
	typedef reverse_iterator<Iterator>                            self;

public:
	//���캯��
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