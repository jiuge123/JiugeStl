#ifndef  JIUGESTL_HEADFILE_VECTOR
#define  JIUGESTL_HEADFILE_VECTOR

#include <initializer_list>

#include "iterator.h"
#include "uninitialized.h"
#include "allocator.h"

namespace JStl{
template<typename T,typename Alloc = JStl::allocator<T>>
class vector{
public:
	Alloc data_allocator;
	typedef		T					value_type;
	typedef		T*					pointer;
	typedef		const T*			const_pointer;
	typedef		T&					reference;
	typedef		const T&			const_reference;
	typedef		size_t			    size_type;
	typedef		ptrdiff_t			difference_type;

	typedef		T*					iterator;
	typedef		const T*			const_iterator;
	typedef		JStl::reverse_iterator<iterator> 
									reverse_iterator;
	typedef		JStl::reverse_iterator<const_iterator>
									const_reverse_iterator;
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

	//�����д�����Ǹ���ֻҪ�����������ͻ�������ƥ�䣬��������int
	//enable��ʹ��typeʱ�����ǰ��ı��ʽΪ�棬�����ã�ǰ��δ�ٲ���ƥ��
	template<typename Iter, typename std::enable_if<is_input_iterator<Iter>::value, int>::type = 0>
	vector(Iter first, Iter last);

	vector(const vector&);

	vector(vector&&) _NOEXCEPT;

	~vector();

	vector& operator=(const vector&) ;

	vector& operator=(vector&&) _NOEXCEPT;

	vector& operator=(std::initializer_list<value_type>);
public:	
	//��������ز���
	iterator begin()
	{
		return begin_;
	}

	const_iterator begin() const
	{
		return begin_;
	}

	iterator end()
	{
		return end_;
	}

	const_iterator end() const
	{
		return end_;
	}


public:
	reference operator[](size_t i)
	{
		if (i < size())
			return *(begin_ + i);
		throw std::out_of_range("vector_index_out_of_range");
	}

	const_reference operator[](size_t i) const
	{
		if (i < size())
			return *(begin_ + i);
		throw std::out_of_range("vector_index_out_of_range");
	}

	reference front()
	{
		return *begin_;
	}

	reference back()
	{
		return *(end_ - 1);
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

template<typename T, typename Alloc = allocator<T>>
template<typename Iter, typename std::enable_if<is_input_iterator<Iter>::value, int>::type = 0>
vector<T, Alloc>::vector(Iter first, Iter last)
{
	if (first < last){
		init_Space(last - first);
		JStl::uninitialized_copy(first, last, begin_);
	}	
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>::vector(const vector &rhs)
{
	init_Space(rhs.size());
	JStl::uninitialized_copy(rhs.begin(), rhs.end(), begin_);
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>::vector(vector &&rhs) :begin_(rhs.begin_), end_(rhs.end_), cap_(rhs.cap_) _NOEXCEPT
{
	rhs.begin_ = nullptr;
	rhs.end_ = nullptr;
	rhs.cap_ = nullptr;
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
	if (begin_ == rhs.begin_){
		return *this;
	}
	else{
		this->~vector();
		init_Space(rhs.size());
		JStl::uninitialized_copy(rhs.begin_, rhs.end_, begin_);
		return *this;
	}	
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>& vector<T, Alloc>::operator=(vector &&rhs) _NOEXCEPT
{
	if (begin_ == rhs.begin_){
		return *this;
	}
	else{
		begin_ = rhs.begin_;
		end_ = rhs.end_;
		cap_ = rhs.cap_;
		rhs.begin_ = nullptr;
		rhs.end_ = nullptr;
		rhs.cap_ = nullptr;
		return *this;
	}
}

template<typename T, typename Alloc = allocator<T>>
vector<T, Alloc>& vector<T, Alloc>::operator=(std::initializer_list<value_type> l)
{
	init_Space(l.size());
	JStl::uninitialized_copy(l.begin(), l.end(), begin);
}









}//namespaec JStl;
#endif