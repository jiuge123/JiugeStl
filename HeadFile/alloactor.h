#ifndef  JIUGESTL_HEADFILE_ALLOCATOR
#define  JIUGESTL_HEADFILE_ALLOCATOR
#include "util.h"
namespace JStl
{
template <typename T>
class Allocator
{
public:
	typedef	T		value_type;
	typedef	T*		pointer;
	typedef	const T*     const_pointer;
	typedef    T&           reference;
	typedef    const T&     const_reference;
	typedef    size_t       size_type;
	typedef    ptrdiff_t    difference_type;
	Allocator() = default;

	~Allocator() = default;

	Allocator(const Allocator& lhs) = delete;

	Allocator &operator=(const Allocator &lhs) = delete;
	//分配内存
	T* allocate();
	T* allocate(size_t n);

	//调用构造函数
	void construct(T *p);
	void construct(T *p,const T& rhs);
	void construct(T *p,T&& rhs);

	template <typename... Args>
	void construct(T *p, const Args&... args);

	//调用析构函数
	void destroy(T *p);
	void destroy(T *first, T *last);

	//释放内存
	void deAllocate(T *p);
	void deAllocate(T *p, size_t n);

};

template <typename T>
T* Allocator<T>::allocate(){
	//自定义new运算符功能是分配内存
	return static<T*>(::operator new(sizeof(T)));
}

template <typename T>
T* Allocator<T>::allocate(size_t n){
	if (n == 0){
		return nullptr;
	}	
	else{
		return static_cast<T*>(::operator new(n * sizeof(T)));
	}
}

template <typename T>
void Allocator<T>::construct(T *p){
	//定位new,可以在p上直接构造
	::new ((void*)p) T();
}

template <typename T>
void Allocator<T>::construct(T *p,const T &rhs){
	::new ((void*)p) T(rhs);
}

template <typename T>
void Allocator<T>::construct(T *p,T &&rhs){
	::new ((void*)p) T(JStl::move(rhs));
}

template <typename T>
template <typename... Args>
void Allocator<T>::construct(T *p, const Args&... args){
	::new ((void*)p) T(JStl::forward<Args>(args)...);
}

template <typename T>
void Allocator<T>::destroy(T *p){
	//判断是否有默认构造函数或是基本类型，如果不是就显式调用析构函数
	if (!std::is_trivially_destructible<T>{} && p != nullptr){
		p->~T();
	}
}

template <typename T>
void Allocator<T>::destroy(T *first, T *last){
	if (!std::is_trivially_destructible<typename iterator_traits<T*>::value_type>{}){
		while (first != last){
			destroy(&*first);
			++first;
		}
	}
}

template <typename T>
void Allocator<T>::deAllocate(T *p){
	if (p != nullptr){
		::operator delete(p);
	}
}

template <typename T>
void Allocator<T>::deAllocate(T *p,size_t n){
	if (p != nullptr){
		::operator delete(p);
	}
}

}
#endif