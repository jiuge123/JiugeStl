#ifndef  JIUGESTL_HEADFILE_ALGOBASE
#define  JIUGESTL_HEADFILE_ALGOBASE
namespace JStl{
/**************************************************************************************/
//copy ��[first��last)������result��ʼ��last��first��
/**************************************************************************************/

template<typename InputIterator, typename ForwardIterator>
ForwardIterator copy(InputIterator first, InputIterator last,
					 ForwardIterator result)
{
	return copy_cat(first, last, result);
}
/**************************************************************************************/
//fill
/**************************************************************************************/
template<typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, const T& x)
{

}
/**************************************************************************************/
//fill_n
/**************************************************************************************/
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator fill_n(ForwardIterator first, Size n, const T& x)
{

}
}//namespace JStl

#endif