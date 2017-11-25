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

//����iterator�ľ���
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