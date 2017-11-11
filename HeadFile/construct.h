#ifndef  JIUGESTL_HEADFILE_CONSTRUCT
#define  JIUGESTL_HEADFILE_CONSTRUCT
//���캯��
#include <new>
#include "util.h"

namespace JStl{
template<typename T>
void construct(T *p){
	//��λnew,������p��ֱ�ӹ���
	::new ((void*)p) T();
}

template <class T, class Ty2>
void construct(T* ptr, const Ty2& value)
{
	::new ((void*)ptr) T(value);
}

template <class T, class... Args>
void construct(T* ptr, Args&&... args)
{
	::new ((void*)ptr) T(JStl::forward<Args>(args)...);
	std::cout << "a";
}

}//namespace JStl
#endif