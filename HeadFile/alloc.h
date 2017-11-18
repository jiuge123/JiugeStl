#ifndef  JIUGESTL_HEADFILE_ALLOC
#define  JIUGESTL_HEADFILE_ALLOC
//内存池实现
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
	struct Free_Node
	{
		Free_Node *next;
	};
	static Free_Node *free_list_[k_list_number_];
	static size_t free_List_Index(size_t n)
	{// 返回索引
		return (n + k_align_ - 1) / k_align_ - 1 ;
	}
	static void* refill(size_t n);
	static char* chunk_Alloc(size_t size, int &nobjs);
	static char *start_;
	static char *end_free_;
	static char *heap_size_;
public:
	Alloc_Poor() = default;

	~Alloc_Poor() = default;

	Alloc_Poor(const  Alloc_Poor&) = delete;

	Alloc_Poor& operator=(const  Alloc_Poor&) = delete;

};

char* Alloc_Poor::start_ = nullptr;
char* Alloc_Poor::end_free_ = nullptr;
char* Alloc_Poor::heap_size_ = nullptr;

Alloc_Poor::Free_Node*
Alloc_Poor::free_list_[Alloc_Poor::k_list_number_] =
{
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr
};



}//namespace JStl
#endif