#ifndef  JIUGESTL_HEADFILE_UTIL
#define  JIUGESTL_HEADFILE_UTIL

//������

#include<cstddef>
#include<type_traits>


namespace JStl
{

//move
template <typename T>	inline
typename std::remove_reference<T>::type&& move(T a) _NOEXCEPT
{
	//ȥ�����ú�ת��Ϊ��ֵ
	return static_cast<typename std::remove_reference<T>::type&&>(a);
}

//forward
//ֻ����ʾ����ģ�壬�����޷��ж����ͣ�int��int&, int &&��ʶ��Ϊint
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
	auto temp(Jl::move(lhs));
	lhs = JStl::move(rhs);
	rhs = JStl::move(temp);
}

}//namespace JiugeStl
#endif