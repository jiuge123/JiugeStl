#ifndef  JIUGESTL_HEADFILE_LIST
#define  JIUGESTL_HEADFILE_LIST

#include <cassert>

#include <initializer_list>

#include "iterator.h"
#include "uninitialized.h"
#include "allocator.h"
#include "util.h"

//包含list 双向环状链表

namespace JStl{

template <class T>
struct list_node 
{
	typedef list_node<T>* node_ptr;

	node_ptr prev;
	node_ptr next;
	T value;

	list_node() = default;

	list_node(const T &v) :value(v){}

	list_node(T &&v) :value(move(v)){}

	void unlink()
	{
		prev = next = self();
	}

	node_ptr self()
	{
		return static_cast<node_ptr>(&*this);
	}
};

//iterator

template <class T>
struct list_iterator :public random_access_iterator_tag
{
	typedef bidrection_iterator_tag			  iterator_category;
	typedef T                                 value_type;
	typedef T*                                pointer;
	typedef T&                                reference;
	typedef ptrdiff_t						  difference_type;
	
	typedef list_node<T>*					  node_ptr;
	typedef list_iterator<T>                  self;

	node_ptr node_; 

	list_iterator() = default;

	list_iterator(node_ptr x) :node_(x) {}

	list_iterator(const list_iterator& rhs) :node_(rhs.node_) {}

	reference operator*() const
	{
		return node_->value;
	}
	
	pointer operator->() const 
	{ 
		return &(operator*()); 
	}

	self& operator++()
	{
		assert(node_ != nullptr);
		node_ = node_->next;
		return *this;
	}

	self operator++(int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator--()
	{
		assert(node_ != nullptr);
		node_ = node_->prev;
		return *this;
	}

	self operator--(int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}

	bool operator==(const self& rhs) const 
	{ 
		return node_ == rhs.node_; 
	}
	
	bool operator!=(const self& rhs) const 
	{ 
		return node_ != rhs.node_; 
	}

};

template <class T>
struct list_const_iterator :public random_access_iterator_tag
{
	typedef bidrection_iterator_tag			  iterator_category;
	typedef T                                 value_type;
	typedef const T*                          pointer;
	typedef const T&                          reference;
	typedef ptrdiff_t						  difference_type;	
	
	typedef list_node<T>*					  node_ptr;
	typedef list_const_iterator<T>            self;

	node_ptr node_;

	list_const_iterator() = default;

	list_const_iterator(node_ptr x) :node_(x) {}

	list_const_iterator(const list_iterator<T>& rhs) :node_(rhs.node_) {}

	list_const_iterator(const list_const_iterator<T>& rhs) :node_(rhs.node_) {}

	reference operator*() const
	{
		return node_->value;
	}

	pointer operator->() const
	{
		return &(operator*());
	}

	self& operator++()
	{
		assert(node_ != nullptr);
		node_ = node_->next;
		return *this;
	}

	self operator++(int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator--()
	{
		assert(node_ != nullptr);
		node_ = node_->prev;
		return *this;
	}

	self operator--(int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}

	bool operator==(const self& rhs) const
	{
		return node_ == rhs.node_;
	}

	bool operator!=(const self& rhs) const
	{
		return node_ != rhs.node_;
	}
};

//list

template<typename T, typename Alloc = allocator<T>>
class list{
public:
	typedef JStl::allocator<T>							data_allocator;
	typedef JStl::allocator<list_node<T>>				node_allocator;

	typedef typename allocator<T>::value_type			value_type;
	typedef typename allocator<T>::pointer				pointer;
	typedef typename allocator<T>::const_pointer		const_pointer;
	typedef typename allocator<T>::reference			reference;
	typedef typename allocator<T>::const_reference		const_reference; 
	typedef typename allocator<T>::size_type			size_type;
	typedef typename allocator<T>::difference_type		difference_type;

	typedef JStl::list_iterator<T>						iterator;
	typedef JStl::list_const_iterator<T>                const_iterator;
	typedef JStl::reverse_iterator<iterator>			reverse_iterator;
	typedef JStl::reverse_iterator<const_iterator>		const_reverse_iterator;
	
	typedef list_node<T>*								node_ptr;

private:
	node_ptr node_;   //指向尾部
	size_type size_;

private:
	template<class... Args>
	node_ptr create_node(Args&& ...args);
	void destroy_node(node_ptr);
	void unlink_nodes(node_ptr first, node_ptr last);

	//初始构造
	void fill_init(size_type n, const value_type& value);  
	template<typename Iter>
	void copy_init(Iter first, Iter last);

	//删除原来，构造新的
	void fill_assign(size_type n, const value_type& value);
	template<typename Iter>
	void copy_assign(Iter first, Iter last);

	//插入
	iterator fill_insert(const_iterator pos, size_type n, const value_type& value);
	template<typename Iter>
	iterator copy_insert(const_iterator pos, size_type n, Iter begin);

	//插入尾部节点
	void link_node_at_back(node_ptr first, node_ptr last);
	//插入头部节点
	void link_node_at_front(node_ptr first, node_ptr last);
	void link_nodes(node_ptr pos, node_ptr first, node_ptr last);
	iterator link_iter_node(node_ptr pos, node_ptr link);
	
public:
	//构造，拷贝构造，移动构造，析构，拷贝赋值，移动赋值
	list();

	list(size_type n); 

	list(size_type n,const value_type& v);

	template<typename Iter, typename std::enable_if<
		JStl::is_input_iterator<Iter>::value, int>::type = 0>
	list(Iter first,Iter last);

	list(const std::initializer_list<T>& l);

	list(const list& rhs);

	list(list &&rhs);

	list& operator=(const list& rhs);

	list& operator=(list &&rhs);

	list& operator=(std::initializer_list<T> l);

	~list();
public:
	//迭代器相关操作
	iterator begin()
	{
		return node_->next;
	}

	const_iterator begin() const
	{
		return node_->next;
	}
	
	iterator end()
	{
		return node_;
	}
	
	const_iterator end() const
	{
		return node_;
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}
	
	reverse_iterator rbegin() const
	{
		return reverse_iterator(end());
	}

	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}

	reverse_iterator rend() const
	{
		return reverse_iterator(begin());
	}

	const_iterator cbegin() const
	{
		return begin();
	}

	const_iterator cend() const
	{
		return end();
	}

	reverse_iterator rcbegin() const
	{
		return rbegin();
	}

	reverse_iterator rcend() const
	{
		return rend();
	}
public:
	//简单函数
	void swap(list &rhs)
	{
		JStl::swap(node_, rhs.node_);
		JStl::swap(size_, rhs.size_);
	}

	bool empty()
	{
		return node_->next == node_;
	}

	size_type size()
	{
		return size_;
	}

	size_type max_size()
	{
		return size_type(-1);
	}

	reference front()
	{
		assert(!empty());
		return node_->next->value;
	}

	const_reference front() const
	{
		assert(!empty());
		return node_->next->value;
	}

	reference back()
	{
		assert(!empty());
		return node_->value;
	}

	const_reference back() const
	{
		assert(!empty());
		return node_->value;
	}

public:
	//复杂函数
	void assign(size_type n, const value_type& value);
	template <class Iter, typename std::enable_if<
		JStl::is_input_iterator<Iter>::value, int>::type = 0>
	void assign(Iter first, Iter last);
	void assign(std::initializer_list<value_type> il);

	template<class ...Args>
	void emplace_front(Args&& ...args);
	template<class ...Args>
	void emplace_back(Args&& ...args);
	template<class ...Args>
	iterator emplace(iterator pos,Args&& ...args);

	void push_front(const value_type& value);
	void push_front(value_type &&value);
	void push_back(const value_type& value);
	void push_back(value_type &&value);

	void pop_front();
	void pop_back(); 

	iterator insert(const_iterator pos, const value_type& value);
	iterator insert(const_iterator pos, value_type&& value);
	iterator insert(const_iterator pos, size_type n, const value_type& value);
	template<typename Iter, typename std::enable_if<
		JStl::is_input_iterator<Iter>::value, int>::type = 0 >
	iterator insert(const_iterator pos, Iter begin, Iter end);
	iterator insert(const_iterator pos, std::initializer_list<value_type> l);

	iterator erase(const_iterator pos); 
	iterator erase(const_iterator begin, const_iterator end);
	
	void resize(size_type new_size);
	void resize(size_type new_size, const value_type& value);
	void clear();

public:
	//将x的元素移动到目的list的指定位置，高效的将他们插入到目的list并从x中删除
	void splice(const_iterator pos, list& other);
	void splice(const_iterator pos, list& other, const_iterator it);
	void splice(const_iterator pos, list& other, const_iterator first, const_iterator last);

	//从 list 中删除所有与 val 值相等的元素
	void remove(const value_type& value);
	//从 list 中删除所有与谓词判断true的元素
	template <class UnaryPredicate>
	void remove_if(UnaryPredicate pred);

	// 移除 list 中满足谓词为true重复元素(首先要有序)
	void unique();
	template <class BinaryPredicate>
	void unique(BinaryPredicate pred);

	//合并两个list，按照comp的比较(首先要有序)
	void merge(list& x);
	template <class Compared>
	void merge(list& x, Compared comp);

	void sort();
	template <class Compared>
	void sort(Compared comp);

	void reverse();

};

template<typename T, typename Alloc = JStl::allocator<T>>
template<class... Args>
typename list<T, Alloc>::node_ptr 
list<T, Alloc>::create_node(Args&& ...args)
{
	node_ptr p = node_allocator::allocate(1);
	try{
		data_allocator::construct(address_of(p->value),JStl::forward<Args>(args)...);
		p->prev = nullptr;
		p->next = nullptr;
	}
	catch (...)
	{
		node_allocator::deallocate(p);
		throw;
	}
	return p;
}

template<typename T,typename Alloc = allocator<T>>
void list<T, Alloc>::destroy_node(node_ptr ptr)
{
	data_allocator::destroy(address_of(ptr->value));
	node_allocator::deallocate(ptr);
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::unlink_nodes(node_ptr first, node_ptr last)
{
	first->prev->next = last->next;
	last->next->prev = first->prev;
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::link_node_at_front(node_ptr first, node_ptr last)
{
	last->next = node_->next;   
	first->prev = node_;		
	last->next->prev = last;
	node_->next = first;			//node->first->..->last->head
}

template<typename T,typename Alloc = allocator<T>>
void list<T, Alloc>::link_node_at_back(node_ptr first, node_ptr last)
{
	first->prev = node_->prev;
	last->next = node_;				
	node_->prev->next = first;
	node_->prev = last;			//first->...->last->node
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::link_nodes(node_ptr pos, node_ptr first, node_ptr last)
{
	first->prev = pos->prev;
	last->next = pos;
	pos->prev->next = first;
	pos->prev = last;			//first->...->last->pos
}

template<typename T, typename Alloc = allocator<T>>
typename list<T, Alloc>::iterator
list<T, Alloc>::link_iter_node(node_ptr pos, node_ptr link)
{
	link->prev = pos->prev;
	link->next = pos;
	pos->prev->next = link;
	pos->prev = link;
	return iterator(link);
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::fill_init(size_type n, const value_type& value)
{
	node_ = node_allocator::allocate(1);
	node_->unlink();
	size_ = n;
	try{
		for( ; n > 0; --n ){
			node_ptr node = create_node(value);
			link_node_at_back(node, node);
		}
	}
	catch (...){
		clear();
		node_allocator::deallocate(node_);
		node_ = nullptr;
		throw;
	}
}

template<typename T, typename Alloc = JStl::allocator<T>>
template<class Iter>
void list<T, Alloc>::copy_init(Iter first, Iter last)
{
	node_ = node_allocator::allocate(1);
	node_->unlink();
	size_ = JStl::distance(first, last);
	size_type n = size_;
	try{
		while (n--){
			node_ptr node = create_node(*first);
			link_node_at_back(node, node);
			++first;
		}
	}
	catch (...){
		clear();
		node_allocator::deallocate(node_);
		node_ = nullptr;
		throw;
	}	
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::fill_assign(size_type n, const value_type& value)
{
	auto i = begin();
	auto end = end();
	for (; n > 0 && i != end; --n, ++i){
		*i = value;
	}
	if (n > 0){
		insert(end, n, value); //迭代器先完成，还有未插入
	}
	else{
		erase(i, end);		//插入先完成，删除后面的原节点
	}
}

template<typename T, typename Alloc = JStl::allocator<T>>
template<class Iter>
void list<T, Alloc>::copy_assign(Iter first, Iter last)
{
	auto i = begin();
	auto end = end();
	for (; first != last && i != end; ++first, ++i){
		*i = *first;
	}
	if (first != last){
		insert(end, first, last);
	}
	else{
		erase(i, end);		
	}
}

template<typename T, typename Alloc = allocator<T>>
typename list<T, Alloc>::iterator 
typename list<T, Alloc>::fill_insert(const_iterator pos, size_type n, const value_type& value)
{
 	iterator be,en(be);
	if (n != 0){
		size_ += n;
		auto link = create_node(value);
		be = iterator(link);
		try{
			for (--n; n > 0; --n){
				auto next = create_node(value);
				next->prev = link;
				link->next = next;
				link = next;	
			}
			en = iterator(link);
		}
		catch (...)
		{
			auto enode = en.node_;
			while (true)
			{
				auto prev = enode->prev;
				destroy_node(enode);
				if (prev == nullptr)
					break;
				enode = prev;
			}
			throw;
		}
		link_nodes(pos.node_, be.node_, en.node_);
	}
	return be;
}

template<typename T, typename Alloc = allocator<T>>
template<typename Iter>
typename list<T, Alloc>::iterator  
list<T, Alloc>::copy_insert(const_iterator pos, size_type n, Iter begin)
{
	iterator be, en(be);
	if (n != 0){
		size_ += n;
		auto link = create_node(*begin);
		be = iterator(link);
		try{
			for (--n, ++begin; n > 0; --n, ++begin){
				auto next = create_node(*begin);
				next->prev = link;
				link->next = next;
				link = next;			
			}
			en = iterator(link);
		}
		catch (...)
		{
			auto enode = en.node_;
			while (true)
			{
				auto prev = enode->prev;
				destroy_node(enode);
				if (prev == nullptr)
					break;
				enode = prev;
			}
			throw;
		}
		link_nodes(pos.node_, be.node_, en.node_);
	}
	return be;
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>::list()
{
	fill_init(0, value_type());
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>::list(size_type n)
{
	fill_init(n, value_type());
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>::list(size_type n,const value_type& v)
{
	fill_init(n, v);
}

template<typename T, typename Alloc = allocator<T>>
template<typename Iter, typename std::enable_if<
	JStl::is_input_iterator<Iter>::value, int>::type = 0>
list<T, Alloc>::list(Iter first, Iter last)
{
	copy_init(first, last);
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>::list(const std::initializer_list<T> &l)
{
	copy_init(l.begin(), l.end());
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>::list(const list& rhs)
{
	copy_init(rhs.begin(), rhs.end());
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>::list(list&& rhs) :node_(rhs.node_), size_(rhs.size_)
{
	rhs.node_ = nullptr;
	rhs.size_ = 0;
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>& list<T, Alloc>::operator=(const list& rhs)
{
	if (this != &rhs){
		assign(rhs.begin(), rhs.end());
	}
	return *this;
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>& list<T, Alloc>::operator=(list&& rhs)
{
	if (this != &rhs){
		list tmp(JStl::move(rhs));
		swap(tmp);
	}
	return *this;
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>& list<T, Alloc>::operator=(std::initializer_list<T> l)
{
	list tmp(l.begin(),l.end());
	swap(tmp);
	return *this;
}

template<typename T, typename Alloc = allocator<T>>
list<T, Alloc>::~list()
{
	if (node_){
		clear();
		node_allocator::deallocate(node_);
		node_ = nullptr;
	}
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::assign(size_type n, const value_type& value)
{
	fill_assign(n, value);
}

template<typename T, typename Alloc = allocator<T>>
template <class Iter, typename std::enable_if<
	JStl::is_input_iterator<Iter>::value, int>::type = 0>
void list<T, Alloc>::assign(Iter first, Iter last)
{
	copy_assign(first, last);
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::assign(std::initializer_list<value_type> il)
{
	copy_assign(il.begin(), il.end());
}

template<typename T, typename Alloc = allocator<T>>
template<class ...Args>
void list<T, Alloc>::emplace_front(Args&& ...args)
{
	auto tmp = create_node(JStl::forward<Args>(args)...);
	link_node_at_front(tmp, tmp);
	++size_;
}

template<typename T, typename Alloc = allocator<T>>
template<class ...Args>
void list<T, Alloc>::emplace_back(Args&& ...args)
{
	auto tmp = create_node(JStl::forward<Args>(args)...);
	link_node_at_back(tmp, tmp);
	++size_;
}

template<typename T, typename Alloc = allocator<T>>
template<class ...Args>
typename list<T, Alloc>::iterator
list<T, Alloc>::emplace(iterator pos, Args&& ...args)
{
	auto tmp = create_node(JStl::forward<Args>(args)...);
	link_nodes(pos.node_, tmp, tmp);
	++size_;
	return iterator(tmp);
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::push_front(const value_type& value)
{
	auto tmp = create_node(value);
	++size_;
	link_node_at_front(tmp, tmp);
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::push_front(value_type &&value)
{
	emplace_front(JStl::move(value));
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::push_back(const value_type& value)
{
	auto tmp = create_node(value);
	++size_;
	link_node_at_back(tmp, tmp);
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::push_back(value_type &&value)
{
	emplace_back(JStl::move(value));
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::pop_front()
{
	assert(size_ > 0);
	auto tmp = node_->next;
	unlink_nodes(tmp, tmp);
	destroy_node(tmp);
	--size_;
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::pop_back()
{
	assert(size_ > 0);
	auto tmp = node_->prev;
	unlink_nodes(tmp, tmp);
	destroy_node(tmp);
	--size_;
}

template<typename T, typename Alloc = allocator<T>>
typename list<T, Alloc>::iterator
list<T, Alloc>::insert(const_iterator pos, const value_type& value)
{
	auto tmp = create_node(value);
	++size_;
	return link_iter_node(pos.node_, tmp);
}

template<typename T, typename Alloc = allocator<T>>
typename list<T, Alloc>::iterator
list<T, Alloc>::insert(const_iterator pos, value_type&& value)
{
	auto tmp = create_node(JStl::move(value));
	++size_;
	return link_iter_node(pos.node_, tmp);
}

template<typename T, typename Alloc = allocator<T>>
typename list<T, Alloc>::iterator  
list<T, Alloc>::insert(const_iterator pos, size_type n, const value_type& value)
{	
	return fill_insert(pos, n, value);
}

template<typename T, typename Alloc = allocator<T>>
template<typename Iter, typename std::enable_if<
	JStl::is_input_iterator<Iter>::value, int>::type = 0 >
typename list<T, Alloc>::iterator
list<T, Alloc>::insert(const_iterator pos, Iter begin, Iter end)
{
	size_type n = JStl::distance(begin, end);
	return copy_insert(pos, n, begin);
}

template<typename T, typename Alloc = allocator<T>>
typename list<T, Alloc>::iterator
list<T, Alloc>::insert(const_iterator pos, std::initializer_list<value_type> l)
{
	size_type n = l.size();
	return copy_insert(pos, n, l.begin());
}

template<typename T, typename Alloc = allocator<T>>
typename list<T, Alloc>::iterator
list<T, Alloc>::erase(const_iterator pos)
{
	assert(pos != cend());
	auto tmp = pos.node_;
	auto next = tmp->next;
	unlink_nodes(tmp, tmp);
	destroy_node(tmp);
	--size_;
	return iterator(next);
}

template<typename T, typename Alloc = allocator<T>>
typename list<T, Alloc>::iterator
list<T, Alloc>::erase(const_iterator begin, const_iterator end)
{
	if (begin != end){
		unlink_nodes(begin.node_, end.node_->prev);	
		for (; begin != end; ){	
			auto tmp = begin.node_;
			++begin;
			destroy_node(tmp);
			--size_;
		}
	}
	return iterator(end.node_);
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::resize(size_type new_size)
{
	resize(new_size, value_type());
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::resize(size_type new_size, const value_type& value)
{
	if (new_size > size_){
		insert(end(), new_size - size_, value);
	}
	else if(new_size < size_){
		size_type n = size_ - new_size;
		auto it = begin();
		while (n > 0){
			n--;
			it++;
		}
		erase(it, end());
	}
	size_ = new_size;
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::clear()
{
	if (size_ != 0){
		node_ptr cur = node_->next;
		for (auto next = cur->next; cur != node_; cur = next, next = cur->next){
			destroy_node(cur->self());
		}
		node_->unlink();
		size_ = 0;
	}
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::splice(const_iterator pos, list& other)
{
	assert(&other != this);
	if (!other.empty()){
		auto be = other.node_->next;
		auto en = other.node_->prev;
		other.unlink_nodes(be, en);
		link_nodes(pos.node_, be, en);
		size_ += other.size_;
		other.size_ = 0;
	}
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::splice(const_iterator pos, list& other, const_iterator it)
{
	//在pos的前面插节点
	if (pos.node_ != it.node_ && pos.node_ != it.node_->next){
		auto tmp = it.node_;
		other.unlink_nodes(tmp, tmp);
		link_nodes(pos.node_, tmp, tmp);
		++size_;
		--other.size_;
	}
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::splice(const_iterator pos, list& other, const_iterator first, const_iterator last)
{
	if (first != last){
		auto n = distance(first, last);
		auto be = first.node_;
		auto en = last.node_->prev;
		other.unlink_nodes(be, en);
		link_nodes(pos.node_, be, en);
		size_ += n;
		other.size_ -= n;
	}
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::remove(const value_type& value)
{
	remove_if([&](const value_type& v) {return v == value; });
}

template<typename T, typename Alloc = allocator<T>>
template <class UnaryPredicate>
void list<T, Alloc>::remove_if(UnaryPredicate pred)
{
	auto be = begin();
	auto en = end();
	while (be != en){
		auto next = be;
		++next;	
		if (pred(*be)){
			erase(be);
		}	
		be = next;
	}
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::unique()
{
	unique([](const value_type& a, const value_type& b){return a == b; });
}

template<typename T, typename Alloc = allocator<T>>
template <class BinaryPredicate>
void list<T, Alloc>::unique(BinaryPredicate pred)
{
	auto be = begin();
	auto en = end();
	auto i = be;
	++i;
	while (i != en){
		if (pred(*i, *be)){
			erase(be);
		}
		be = i;
		++i;
	}
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::merge(list& x)
{
	merge(x, [](const value_type &a, const value_type &b){return a < b; });
}

template<typename T, typename Alloc = allocator<T>>
template <class Compared>
void list<T, Alloc>::merge(list& x, Compared comp)
{
	if (this != &x){
		auto be1 = begin();
		auto en1 = end();
		auto be2 = x.begin();
		auto en2 = x.end();
		while (be1 != en1&&be2 != en2){
			if (comp(*be2, *be1)){
				auto next = be2;
				++next;
				x.unlink_nodes(be2.node_, be2.node_);
				link_nodes(be1.node_, be2.node_, be2.node_);			
				be2 = next;
			}
			else{
				++be1;
			}
		}
		if (be2 != en2){
			x.unlink_nodes(be2.node_, en2.node_);
			link_nodes(be1.node_, be2.node_, en2.node_->prev);
			
		}
		size_ += x.size_;
		x.size_ = 0;
		x.node_->unlink();
	}
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::sort()
{
	sort([](const value_type &a, const value_type &b){return a < b; });
}

// 非递归归并排序，返回一个迭代器指向区间最小元素的位置
//https://www.linuxidc.com/Linux/2016-04/130199.htm
template<typename T, typename Alloc = allocator<T>>
template <class Compared>
void list<T, Alloc>::sort(Compared comp)
{
	if (size_ < 2){
		return;
	}
	size_type n = size_;
	auto be = begin();
	list<T, Alloc> carry;
	list<T, Alloc> counter[64];	//每个数组放 1，2，4，8。。。
	int fill = 0;
	while (size_ > 0){
		auto be2(++be);
		carry.splice(carry.begin(), *this, begin());
		int i = 0;
		while (i < fill && !counter[i].empty())	{
			counter[i].merge(carry);
			carry.swap(counter[i++]);
		}
		carry.swap(counter[i]);
		if (i == fill)
			++fill;
		be = be2;
	}
	for (int i = 1; i < fill; ++i)
		counter[i].merge(counter[i - 1]);
	auto tmp1 = counter[fill - 1].begin().node_;
	auto tmp2 = counter[fill - 1].end().node_->prev;
	counter[fill - 1].unlink_nodes(tmp1, tmp2);
	link_nodes(node_, tmp1, tmp2);
	size_ = n;
}

template<typename T, typename Alloc = allocator<T>>
void list<T, Alloc>::reverse()
{
	if (size_ <= 1){
		return;
	}
	auto be = node_;
	while (be->next != end().node_)
	{
		auto tmp = end().node_->prev;
		unlink_nodes(tmp, tmp);
		link_nodes(be->next,tmp, tmp);
		be = tmp;
	}
}

template <typename T>
bool operator==(const list<T>& lhs, const list<T>& rhs)
{
	auto be1 = lhs.cbegin();
	auto be2 = rhs.cbegin();
	auto en1 = lhs.cend();
	auto en2 = rhs.cend();
	for (; be1 != en1 && be2 != en2 && *be1 == *be2; ++be1, ++be2)
		;
	return be1 == en1 && be2 == en2;//两个都到终点则==
}

template <class T>
bool operator<(const list<T>& lhs, const list<T>& rhs)
{
	return JStl::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <class T>
bool operator!=(const list<T>& lhs, const list<T>& rhs)
{
	return !(lhs == rhs);
}

template <class T>
bool operator>(const list<T>& lhs, const list<T>& rhs)
{
	return rhs < lhs;
}

template <class T>
bool operator<=(const list<T>& lhs, const list<T>& rhs)
{
	return !(rhs < lhs);
}

template <class T>
bool operator>=(const list<T>& lhs, const list<T>& rhs)
{
	return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class T>
void swap(list<T>& lhs, list<T>& rhs)
{
	lhs.swap(rhs);
}


}//namespaec JStl;
#endif