#ifndef  JIUGESTL_HEADFILE_ALLOCATOR
#define  JIUGESTL_HEADFILE_ALLOCATOR

//�ڴ����

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
	//�����ڴ�
	static T* allocate();
	static T* allocate(size_t n);

	//���ù��캯��
	static void construct(T *p);
	static void construct(T *p, const T& rhs);
	static void construct(T *p, T&& rhs);

	template <typename... Args>
	static void construct(T *p, Args&&... args);

	//������������
	static void destroy(T *p);
	static void destroy(T *first, T *last);

	//�ͷ��ڴ�
	static void deallocate(T *p);
	static void deallocate(T *p, size_t n);

	template<typename U>
	struct rebind{
		typedef allocator<U> other;
	};

	//���ص�ַ
	T* address(T& x)
	{
		return (T*)&x;
	}
	const T* const_address(const T& x)
	{
		return (const T*)&x;
	}
};

/**************************************************************************************/
//new�����ڴ�
/**************************************************************************************/
template <typename T>
T* allocator<T>::allocate()
{
	//�Զ���new����������Ƿ����ڴ�,������
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

/**************************************************************************************/
//����construct����
/**************************************************************************************/
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

/**************************************************************************************/
//����destroy����
/**************************************************************************************/
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

/**************************************************************************************/
//delete�ͷ��ڴ�
/**************************************************************************************/
template <typename T>
void allocator<T>::deallocate(T *p)
{
	if (p){
		::operator delete(p);
	}
}

template <typename T>
void allocator<T>::deallocate(T *p,size_t n)
{
	if (p){
		::operator delete(p);
	}
}

}
#endif