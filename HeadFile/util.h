#ifndef  JIUGESTL_HEADFILE_UTIL
#define  JIUGESTL_HEADFILE_UTIL

//工具类

#include<cstddef>
#include<type_traits>


namespace JStl
{

//move
template <typename T>	inline
typename std::remove_reference<T>::type&& move(T&& a) _NOEXCEPT
{
	//去除引用后转化为右值
	return static_cast<typename std::remove_reference<T>::type&&>(a);
}

//forward
//只能显示调用模板，否则无法判断类型，int，int&, int &&都识别为int
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
	auto temp(JStl::move(lhs));
	lhs = JStl::move(rhs);
	rhs = JStl::move(temp);
}

//min
template<typename T>
T min(const T &a, const T &b)
{
	return (a < b ? a : b);
}

//max
template<typename T>
T max(const T &a, const T &b)
{
	return (a > b ? a : b);
}

//address_of
template<typename T>
T* address_of(T& value)
{
	return &value;
}

//pair
template<typename Arg1,typename Arg2>
struct pair{
	typedef		Arg1	first_type;
	typedef		Arg2	second_type;
	first_type first;
	second_type second;

//构造函数及运算符

	pair() :first(), second()
	{}

	pair(const Arg1 &a, const Arg2 &b) :first(a), second(b)
	{}

	pair(Arg1 &&a, Arg2 &&b) :first(forward<first_type>(a)),
		second(forward<second_type>(b))
	{}

	pair(const pair& rhs) :first(rhs.first), second(rhs.second)
	{}

	//两个类型不同的pair转化
	//如果rhs的类型可以转化为当前
	template <class Other1, class Other2,
		typename std::enable_if<
		std::is_constructible<Arg1, const Other1&>::value &&
		std::is_constructible<Arg2, const Other2&>::value &&
		std::is_convertible<const Other1&, Arg1>::value &&
		std::is_convertible<const Other2&, Arg2>::value, int>::type = 0>
		pair(const pair<Other1, Other2>& other)
		: first(other.first),second(other.second)
	{}
	
	pair(pair&& rhs) :first(forward<first_type>(rhs.first)),
		second(forward<second_type>(rhs.second))
	{}

	template <class Other1, class Other2,
		typename std::enable_if<
		std::is_constructible<Arg1, Other1>::value &&
		std::is_constructible<Arg2, Other2>::value &&
		std::is_convertible<Other1, Arg1>::value &&
		std::is_convertible<Other2, Arg2>::value, int>::type = 0>
		pair(pair<Other1, Other2>&& other)
		: first(forward<Other1>(other.first)),
		second(forward<Other2>(other.second))
	{}

};



}//namespace JiugeStl
#endif