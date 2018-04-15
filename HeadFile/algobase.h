#ifndef  JIUGESTL_HEADFILE_ALGOBASE
#define  JIUGESTL_HEADFILE_ALGOBASE

#include <cassert>

namespace JStl{
/**************************************************************************************/
//copy 把[first，last)拷贝到result开始的last和first中
/**************************************************************************************/
template<typename InputIterator, typename OutputIterator>
OutputIterator __unchecked_copy_cat(InputIterator first, InputIterator last,
	OutputIterator result, JStl::input_iterator_tag)
{
	while (first != last){
		*result = *first;
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
		*result = *first;
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

//如果Tp Up相等，且有默认=operator
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
	assert(last >= first);
	return __unchecked_copy(first, last, result);
	
}

/**************************************************************************************/
//copy_backward 将 [first, last)区间内的元素拷贝到 [result - (last - first), result)内
/**************************************************************************************/
template <class BidrectionalIterator1, class BidrectionalIterator2>
BidrectionalIterator2
__unchecked_copy_backward_cat(BidrectionalIterator1 first, BidrectionalIterator1 last,
BidrectionalIterator2 result, JStl::bidrection_iterator_tag)
{
	while (first != last){
		--result;
		--last;
		*result = *last;
	}
	*result = *last;
	return result;
}

template <class BidrectionalIterator1, class BidrectionalIterator2>
BidrectionalIterator2
__unchecked_copy_backward_cat(BidrectionalIterator1 first, BidrectionalIterator1 last,
BidrectionalIterator2 result, JStl::random_access_iterator_tag)
{
	auto n = last - first;
	while (n-- > 0){		
		--result;
		--last;
		*result = *last;
	}	
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

template <class Tp, class Up>
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

/**************************************************************************************/
//fill 从first到last，拷贝x
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
//fill_n 拷贝n个x
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
//move 把[first，last)move到result开始的last和first中
/**************************************************************************************/
template<typename InputIterator, typename OutputIterator>
OutputIterator __unchecked_move_cat(InputIterator first, InputIterator last,
	OutputIterator result, JStl::input_iterator_tag)
{
	while (first != last){
		*result = JStl::move(*first);
		++first;
		++result;
	}
	return result;
}

template<typename RandomIterator, typename OutputIterator>
RandomIterator __unchecked_move_cat(RandomIterator first, RandomIterator last,
	OutputIterator result, JStl::random_access_iterator_tag)
{
	auto n = last - first;
	while (n-- > 0){
		*result = JStl::move(*first);
		++first;
		++last;
	}
	return result;
}

template<typename InputIterator, typename ForwardIterator>
ForwardIterator __unchecked_move(InputIterator first, InputIterator last,
	ForwardIterator result)
{
	return __unchecked_move_cat(first, last, result, JStl::iterator_category(first));
}

//如果Tp Up相等，且有默认=operator
template<typename Tp, typename Up>
typename std::enable_if<
	std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
	std::is_trivially_move_assignable<Up>::value,
	Up*>::type
	__unchecked_move(Tp* first, Tp* last, Up* result)
{
	const auto n = static_cast<size_t>(last - first);
	if (n != 0)
		std::memmove(result, first, n * sizeof(Up));
	return result + n;
}

template<typename InputIterator, typename ForwardIterator>
ForwardIterator move(InputIterator first, InputIterator last,
	ForwardIterator result)
{
	assert(last >= first);
	return __unchecked_move(first, last, result);
	
}

/**************************************************************************************/
//move_backward 将 [first, last)区间内的元素拷贝到 [result - (last - first), result)内
/**************************************************************************************/
template <class BidrectionalIterator1, class BidrectionalIterator2>
BidrectionalIterator2
__unchecked_move_backward_cat(BidrectionalIterator1 first, BidrectionalIterator1 last,
BidrectionalIterator2 result, JStl::bidrection_iterator_tag)
{
	while (first != last){
		--result;
		--last;
		*result = JStl::move(*last);
	}
	*result = *last;
	return result;
}

template <class BidrectionalIterator1, class BidrectionalIterator2>
BidrectionalIterator2
__unchecked_move_backward_cat(BidrectionalIterator1 first, BidrectionalIterator1 last,
BidrectionalIterator2 result, JStl::random_access_iterator_tag)
{
	auto n = last - first;
	while (n-- > 0){
		--result;
		--last;
		*result = JStl::move(*last);
	}
	return result;
}

template <class BidrectionalIterator1, class BidrectionalIterator2>
BidrectionalIterator2
__unchecked_move_backward(BidrectionalIterator1 first, BidrectionalIterator1 last,
BidrectionalIterator2 result)
{
	return __unchecked_move_backward_cat(first, last, result,
		iterator_category(first));
}

template<class Tp, class Up>
typename std::enable_if<
	std::is_same<typename std::remove_const<Tp>::type, Up>::value &&
	std::is_trivially_move_assignable<Up>::value,
	Up*>::type
__unchecked_move_backward(Tp* first, Tp* last, Up* result)
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
move_backward(BidrectionalIterator1 first, BidrectionalIterator1 last, BidrectionalIterator2 result)
{
	return __unchecked_move_backward(first, last, result);
}

/**************************************************************************************/
//equal
// 比较 [first, last)区间上的元素值是否和第二序列相等
/**************************************************************************************/
template<typename InputIterator1,typename InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
{
	for (; first1 != last1; ++first1, ++first2){
		if (*first1 != *first2)
			return false;
	}
	return true;
}

template<typename InputIterator1, typename InputIterator2,typename Compared>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,Compared comp)
{
	for (; first1 != last1; ++first1, ++first2){
		if (!comp(*first1, *first2))
			return false;
	}
	return true;
}
/**************************************************************************************/
// lexicographical_compare
// 以字典序排列对两个序列进行比较，当在某个位置发现第一组不相等元素时，有下列几种情况：
// (1)如果第一序列的元素较小，返回 true ，否则返回 false
// (2)如果到达 last1 而尚未到达 last2 返回 true
// (3)如果到达 last2 而尚未到达 last1 返回 false
// (4)如果同时到达 last1 和 last2 返回 false
/**************************************************************************************/
template<typename InputIterator1, typename InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
							 InputIterator2 first2, InputIterator2 last2)
{
	for (; first1 != last1&&first2 != last2; ++first1, ++first2){
		if (*first1 < *first2)
			return true;
		else if (*first1 > *first2)
			return false;
	}
	return (first1 == last1) && (first2 != last2);
}

template<typename InputIterator1, typename InputIterator2, typename Compared>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, Compared comp)
{
	for (; first1 != last1&&first2 != last2; ++first1, ++first2){
		if (comp(*first1,*first2))
			return true;
		else if (comp(*first2, *first1))
			return false;
	}
	return (first1 == last1) && (first2 != last2);
}

}//namespace JStl
#endif