#ifndef  JIUGESTL_HEADFILE_ALLOCATOR
#define  JIUGESTL_HEADFILE_ALLOCATOR
#include "util.h"
namespace JiugeStl
{
template <typename T>
class allocator
{
public:
	typedef	T		value_type;
	typedef	T*		pointer;
	typedef const T*     const_pointer;
	typedef T&           reference;
	typedef const T&     const_reference;
	typedef size_t       size_type;
	typedef ptrdiff_t    difference_type;
	allocator();
	~allocator();
	
private:

};

template <typename T>
allocator<T>::allocator()
{
}

template <typename T>
allocator<T>::~allocator()
{
}
}
#endif