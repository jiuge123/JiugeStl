#ifndef  JIUGESTL_HEADFILE_ALLOCATOR
#define  JIUGESTL_HEADFILE_ALLOCATOR

//内存分配

#include "construct.h"

namespace JStl
{
template <typename T>
class allocator
{
public:
    typedef		T					value_type;
	typedef		T*					pointer;
	typedef		const T*			const_pointer;
	typedef		T&					reference;
	typedef		const T&			const_reference;
	typedef		size_t			    size_type;
	typedef		ptrdiff_t			difference_type;
	allocator() = default;

	~allocator() = default;

	allocator(const allocator& lhs) = delete;

	allocator &operator=(const allocator &lhs) = delete;
	//分配内存
	T* allocate();
	T* allocate(size_t n);

	//调用构造函数
	void construct(T *p);
	void construct(T *p,const T& rhs);
	void construct(T *p,T&& rhs);

	template <typename... Args>
	void construct(T *p,Args&&... args);

	//调用析构函数
	void destroy(T *p);
	void destroy(T *first, T *last);

	//释放内存
	void deallocate(T *p);
	void deallocate(T *p, size_t n);

	template<typename U>
	struct rebind{
		typedef allocator<U> other;
	};

	//返回地址
	T* address(T& x)
	{
		return (T*)&x;
	}
	const T* const_address(const T& x)
	{
		return (const T*)&x;
	}
};

template <typename T>
T* allocator<T>::allocate()
{
	//自定义new运算符功能是分配内存,不构造
	return static_cast<T*>(::operator new(sizeof(T)));
}

template <typename T>
T* allocator<T>::allocate(size_t n)
{
	if (n == 0){
		return nullptr;
	}	
	else{
		return static_cast<T*>(::operator new(n * sizeof(T)));
	}
}

template <typename T>
void allocator<T>::construct(T *p)
{
	JStl::construct(p);
}

template <class T>
void allocator<T>::construct(T* ptr, const T& value)
{
	JStl::construct(ptr, value);
}

template <class T>
void allocator<T>::construct(T* ptr, T&& value)
{
	JStl::construct(ptr,JStl::move(value));
}

template <typename T>
template <typename... Args>
void allocator<T>::construct(T *p, Args&&... args)
{
	JStl::construct(p, JStl::forward<Args>(args)...);
}

template <class T>
void allocator<T>::destroy(T* ptr)
{
	JStl::destroy(ptr);
}

template <class T>
void allocator<T>::destroy(T* first, T* last)
{
	JStl::destroy(first, last);
}

template <typename T>
void allocator<T>::deallocate(T *p)
{
	if (p != nullptr){
		::operator delete(p);
	}
}

template <typename T>
void allocator<T>::deallocate(T *p,size_t n)
{
	if (p != nullptr){
		::operator delete(p);
	}
}

}
#endif