#ifndef  JIUGESTL_HEADFILE_ITERATOR
#define  JIUGESTL_HEADFILE_ITERATOR

//������

#include<type_traits>
#include<cstddef>

namespace JStl{

//���ֵ�����
struct input_iterator_tag{};//����
struct output_iterator_tag{};//���
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
	typedef Category			iterator_category;  //���������ͣ�����֮һ
	typedef T						value_type;//ָ���Ԫ��
	typedef Pointer			pointer;//ԭʼָ��
	typedef Reference       reference;//��ָԪ�ص�����
	typedef Distance         difference_type;//������֮��
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
	//�Ƿ���Ըı�
	std::is_convertible<typename Iterator::iterator_category, input_iterator_tag>::value ||
	std::is_convertible<typename Iterator::iterator_category, output_iterator_tag>::value>
{};

//��ȡ������������
template <class Iterator>
struct iterator_traits
	:public iterator_traits_helper<Iterator, has_iterator_cat<Iterator>::value>{};


}//namespace JStl
#endif