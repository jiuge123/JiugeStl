#ifndef  JIUGESTL_HEADFILE_ALGOBASE
#define  JIUGESTL_HEADFILE_ALGOBASE

namespace JStl{
/**************************************************************************************/
//copy ��[first��last)������result��ʼ��last��first��
/**************************************************************************************/
template<typename InputIterator, typename OutputIterator>
OutputIterator __unchecked_copy_cat(InputIterator first, InputIterator last,
	OutputIterator result, JStl::input_iterator_tag)
{
	while (first != last){
		construct(result, *first);
		++first;
		++result;
	}
	return result;
}

template<typename RandomIterator, typename OutputIterator>
RandomIterator __unchecked_copy_cat(RandomIterator first, RandomIterator last,
	OutputIterator result, JStl::random_access_iterator_tag)
{
	auto n = last - first;
	while (n-- > 0){
		construct(result, *first);
		++first;
		++last;
	}
	return result;
}

template<typename InputIterator, typename ForwardIterator>
ForwardIterator __unchecked_copy(InputIterator first, InputIterator last,
	ForwardIterator result)
{
	return __unchecked_copy_cat(first, last, result, JStl::iterator_category(first));
}

//���Tp Up��ȣ�����Ĭ��=operator
template<typename Tp, typename Up>
typename std::enable_if<
	std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
	std::is_trivially_copy_assignable<Up>::value,
	Up*>::type
__unchecked_copy(Tp* first, Tp* last, Up* result)
{
		const auto n = static_cast<size_t>(last - first);
		if (n != 0)
			std::memmove(result, first, n * sizeof(Up));
		return result + n;
	}

template<typename InputIterator, typename ForwardIterator>
ForwardIterator copy(InputIterator first, InputIterator last,
	ForwardIterator result)
{
	if (last > first)
		return __unchecked_copy(first, last, result);
	return 0;
}
/**************************************************************************************/
//fill ��first��last������x
/**************************************************************************************/
template<typename ForwardIterator, typename T>
void __fill(ForwardIterator first, ForwardIterator last, const T& x,
	JStl::forward_iterator_tag)
{
		while (first != last){
			*first = x;
			++first;
		}
	}

template<typename RandomIterator, typename T>
void __fill(RandomIterator first, RandomIterator last, const T& x,
	JStl::random_access_iterator_tag)
{
	fill_n(first, last - first, x);
}

template<typename ForwardIterator, typename T>
void fill(ForwardIterator first, ForwardIterator last, const T& x)
{
	__fill(first, last, x, iterator_category(first));
}
/**************************************************************************************/
//fill_n ����n��x
/**************************************************************************************/
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator fill_n(ForwardIterator first, Size n, const T& x)
{
	while (n--){
		*first = x;
		first++;
	}
	return first;
}
/**************************************************************************************/
//copy_backward �� [first, last)�����ڵ�Ԫ�ؿ����� [result - (last - first), result)��
/**************************************************************************************/
template <class BidrectionalIterator1, class BidrectionalIterator2>
BidrectionalIterator2
__unchecked_copy_backward_cat(BidrectionalIterator1 first, BidrectionalIterator1 last,
BidrectionalIterator2 result, JStl::bidrection_iterator_tag)
{
	while (first != last)
		*--result = *--last;
	return result;
}

template <class BidrectionalIterator1, class BidrectionalIterator2>
BidrectionalIterator2
__unchecked_copy_backward_cat(BidrectionalIterator1 first, BidrectionalIterator1 last,
BidrectionalIterator2 result, JStl::random_access_iterator_tag)
{
	for (auto n = last - first; n > 0; --n)
		*--result = *--last;
	return result;
}

template <class BidrectionalIterator1, class BidrectionalIterator2>
BidrectionalIterator2
__unchecked_copy_backward(BidrectionalIterator1 first, BidrectionalIterator1 last,
BidrectionalIterator2 result)
{
	return __unchecked_copy_backward_cat(first, last, result,
		iterator_category(first));
}

template<class Tp,class Up>
typename std::enable_if<
	std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
	std::is_trivially_copy_assignable<Up>::value,
	Up*>::type
__unchecked_copy_backward(Tp* first, Tp* last, Up* result)
{
	const auto n = static_cast<size_t>(last - first);
	if (n != 0)
	{
		result -= n;
		std::memmove(result, first, n * sizeof(Up));
	}
	return result;
}

template<typename BidrectionalIterator1, typename BidrectionalIterator2>
BidrectionalIterator2
copy_backward(BidrectionalIterator1 first, BidrectionalIterator1 last, BidrectionalIterator2 result)
{
	return __unchecked_copy_backward(first, last, result);
}


}//namespace JStl
#endif