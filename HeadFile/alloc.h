#ifndef  JIUGESTL_HEADFILE_ALLOC
#define  JIUGESTL_HEADFILE_ALLOC
//内存池实现(跟着stl源码剖析写的)
#include<iostream>
#include<exception>

namespace JStl{
class Alloc_Poor
{
private:
	static const size_t k_align_ = 8;
	static const size_t k_max_bytes_ = 128;
	static const size_t k_list_number_ = k_max_bytes_ / k_align_;
	static size_t round_Up(size_t bytes)
	{//8的倍数
		return (bytes + k_align_ - 1) & ~(k_align_ - 1);
	}
	union Free_Node
	{
		char* c;
		Free_Node *next;
	};
	static Free_Node *free_list_[k_list_number_];
	static size_t free_List_Index(size_t n)
	{// 返回索引,n>0
		return (n + k_align_ - 1) / k_align_ - 1 ;
	}
	static void* refill(size_t n);
	static char* chunk_Alloc(size_t size, int *nobjs);
	static char* start_free_;
	static char* end_free_;
	static size_t heap_size_;
public:
	Alloc_Poor() = default;

	~Alloc_Poor() = default;

	Alloc_Poor(const  Alloc_Poor&) = delete;

	Alloc_Poor& operator=(const  Alloc_Poor&) = delete;

	static void* allocate(size_t n);
	static void deallocate(void* p, size_t n);
	static void* reallocate(void *p, size_t old_n, size_t new_n);
};

char* Alloc_Poor::start_free_ = nullptr;
char* Alloc_Poor::end_free_ = nullptr;
size_t Alloc_Poor::heap_size_ = 0;

Alloc_Poor::Free_Node*
Alloc_Poor::free_list_[Alloc_Poor::k_list_number_] =
{
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr
};
void* Alloc_Poor::allocate(size_t n)
{
	if (n > k_max_bytes_){
        return ::operator new(n);
	}
	Free_Node *my_free_node = *(free_list_ + free_List_Index(n));
	Free_Node *result = my_free_node;
	if(result == nullptr){
        void *r = refill(round_Up(n));
        return r;
	}
    *my_free_node = *my_free_node->next;
    return result;
}

void Alloc_Poor::deallocate(void* p,size_t n)
{
    if(n > k_max_bytes_){
        ::operator delete(p);
        return;
    }
    Free_Node *my_free_node = *(free_list_ + free_List_Index(n));
    Free_Node *q = reinterpret_cast<Free_Node*>(p);
    q->next = my_free_node;
    my_free_node = q;
}

void* Alloc_Poor::reallocate(void *p,size_t old_n,size_t new_n)
{
    deallocate(p,old_n);
    return allocate(new_n);
}

void* Alloc_Poor::refill(size_t n)
{
    int nodjs = 20;
    char *chunk = chunk_Alloc(n,&nodjs);
    if(nodjs == 1)
        return chunk;
    Free_Node *my_free_node = *(free_list_ + free_List_Index(n));
    Free_Node *result = (Free_Node*)chunk;
    Free_Node *next_node = nullptr,*current_node = nullptr;
    my_free_node = next_node =  (Free_Node*)(chunk + n);
    for(int i = 1;;i++){
        current_node = next_node;
        next_node = (Free_Node*)((char*)next_node + n);
        if(i + 1 == nodjs){
            current_node->next = nullptr;
            break;
        }else{
            current_node->next = next_node;
        }
    }
    return result;
}

char* Alloc_Poor::chunk_Alloc(size_t n,int *nodjs)
{
    size_t total_bytes = n * (*nodjs);//要求分配的内存
    size_t left_bytes = end_free_ - start_free_;//内存池所剩
    char *result;
    if(left_bytes > total_bytes){//够20个了就直接分配
        result = start_free_;
        start_free_ += total_bytes;
        return result;
    } else if(left_bytes >= n){
        *nodjs = left_bytes / n;
        total_bytes = n * (*nodjs);
        result = start_free_;
        start_free_ += total_bytes;
        return result;
    }else{
		size_t  byte_to_get = 2 * total_bytes + round_Up(heap_size_ >> 4);
		if (left_bytes > 0){
			Free_Node *my_free_list = *(free_list_ + free_List_Index(left_bytes));
			((Free_Node *)start_free_)->next = my_free_list;
			my_free_list = (Free_Node*)start_free_;
		}	
		//配置堆空间
		start_free_ = (char*)::operator new(byte_to_get);
		if (start_free_ == 0){//如果堆空间不足
			Free_Node *my_free_list, *p;
			for (int i = n; i <= k_max_bytes_; i += k_align_){
				my_free_list = *(free_list_ + free_List_Index(i));
				p = my_free_list;
				if (p){
					my_free_list = p->next;
					start_free_ = (char*)p;
					end_free_ = start_free_ + i;
					return chunk_Alloc(n, nodjs);
				}
			}
		end_free_ = nullptr;
		std::cout << "out of memory" << std::endl;
		throw std::bad_alloc();
		}
		end_free_ = start_free_ + byte_to_get;
		heap_size_ += byte_to_get;
		return chunk_Alloc(n, nodjs);
	}
}

}//namespace JStl
#endif
