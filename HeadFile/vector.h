#ifndef  JIUGESTL_HEADFILE_VECTOR
#define  JIUGESTL_HEADFILE_VECTOR

#include <initializer_list>

#include "iterator.h"
#include "uninitialized.h"

namespace JStl{
template<typename T,typename Alloc = allocator<T>>
class vector{ 
	Alloc data_allocator;
	typedef		T					value_type;
	typedef		T*					iterator;
	typedef		const T*			const_iterator;
	typedef		T&					reference;
	typedef		const T&			const_reference;
	typedef		size_t			    size_type;
	typedef		ptrdiff_t			difference_type;
private:
	T *begin_;  //���õ��׵�ַ
 	T *end_;	//���õ�β��ַ
	T *cap_;	//������ַ
	
	//��ʼ����ռ�
	void init_Space(size_t);
	
public:
	//���죬�������죬�ƶ����죬������������ֵ���ƶ���ֵ
	vector() :begin_(0), end_(0), cap(0){}

	vector(size_t, const T&);

	vector(std::initializer_list<value_type>);

	template<typename Iter>
	vector(Iter first, Iter last);

	vector(const vector&);

	vector(vector&&) _NOEXCEPT;

	~vector();

	vector& operator=(const vector&) ;

	vector& operator=(vector&&) _NOEXCEPT;


public:	
	T* begin() const
	{
		return begin_;
	}

	T* end() const
	{
		return end_;
	}

	//����vector����
	size_t size() const
	{
		return end_ - begin_;
	}

	//����vector����
	size_t capacity() const
	{
		return cap_ - begin_;
	}

	//vector�Ƿ�Ϊ��
	bool empty() const
	{
		return begin_ == end_;
	}

};

template<typename T, typename Alloc = allocator<T>>
void vector<T, Alloc>::init_Space(size_t n)
{
	try{
		begin_ = data_allocator.allocate(n);
		end_ = begin_ + n;
		cap_ = end_;
	}
	catch (...){
		begin_ = nullptr;
		end_ = nullptr;
		cap_ = nullptr;
	}
}

template<typename T, typename Alloc = allocator<T>>
vector<T,Alloc>::vector(size_t n, const T& value)
{
	init_Space(n);
	JStl::uninitialized_fill_n(begin_, n, value);
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>::vector(std::initializer_list<T> i)
{
	init_Space(i.size());
	JStl::uninitialized_copy(i.begin(), i.end(), begin_);
}

//error
template<typename T, typename Alloc = allocator<T>>
template<typename Iter>
vector<T, Alloc>::vector(Iter first, Iter last)
{
	if (first < last){
		init_Space(last - first);
		JStl::uninitialized_copy(first, last, begin_);
	}	
}

//error
template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>::vector(const vector &rhs)
{
	init_Space(rhs.size());
	JStl::uninitialized_copy(rhs.begin(), rhs.end(), begin_);
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>::vector(vector &&rhs)
{

}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>::~vector()
{
	destroy(begin_, end_);
	data_allocator.deallocate(begin_);
	begin_ = nullptr;
	end_ = nullptr;
	cap_ = nullptr;
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>& vector<T, Alloc>::operator=(const vector &rhs)
{

}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>& vector<T, Alloc>::operator=(vector &&rhs)
{

}









}//namespaec JStl;
#endif