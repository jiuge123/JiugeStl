#ifndef  JIUGESTL_HEADFILE_UNINITIALIZED
#define  JIUGESTL_HEADFILE_UNINITIALIZED
//����������Χ���캯��

#include "iterator.h"
namespace JStl{
//�ӵ�����result��ʼ��������last-first������
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
	
}

//��[first,last)��copy���졣
template<typename ForwardIterator, typename T>
ForwardIterator uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{

}

//��first��ʼcopy����n��
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{

}
}//namespace JStl

#endif