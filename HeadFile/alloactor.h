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
	//�����ڴ�
	T* allocate();
	T* allocate(size_t n);

	//���ù��캯��
	void construct(T *p);
	void construct(T *p,const T& rhs);
	void construct(T *p,T&& rhs);

	template <typename... Args>
	void construct(T *p, const Args&... args);

	//������������
	void destroy(T *p);
	void destroy(T *first, T *last);

	//�ͷ��ڴ�
	void deAllocate(T *p);
	void deAllocate(T *p, size_t n);

};

template <typename T>
T* Allocator<T>::allocate(){
	//�Զ���new����������Ƿ����ڴ�
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
	//��λnew,������p��ֱ�ӹ���
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
	//�ж��Ƿ���Ĭ�Ϲ��캯�����ǻ������ͣ�������Ǿ���ʽ������������
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