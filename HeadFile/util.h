#ifndef  JIUGESTL_HEADFILE_UTIL
#define  JIUGESTL_HEADFILE_UTIL
#include<type_traits>
#include<cstddef>
//π§æﬂ¿‡
namespace JiugeStl
{

//move
template <typename T>	inline
typename std::remove_reference<T>::type&& move(T a) _NOEXCEPT
{
	return static_cast<typename std::remove_reference<T>::type&&>(a);
}

//forward
template <typename T>	inline
T&& forward(typename std::remove_reference<T>::type& arg) _NOEXCEPT
{
	return static_cast<T&&>(arg);
}

template <typename T> inline
T&& forward(typename std::remove_reference<T>::type&& arg) _NOEXCEPT	
{
	static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
	return static_cast<T&&>(arg);
}

// swap
template <typename T>
void swap(T& lhs, T& rhs)
{
	auto temp(JiugeStl::move(lhs));
	lhs = JiugeStl::move(rhs);
	rhs = JiugeStl::move(temp);
}

}//namespace JiugeStl
#endif