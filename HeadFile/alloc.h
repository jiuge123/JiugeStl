#ifndef  JIUGESTL_HEADFILE_ALLOC
#define  JIUGESTL_HEADFILE_ALLOC
//内存池实现
namespace JStl{
class Alloc_Poor
{
private:
	static const int k_align_ = 8;
	static const int k_max_bytes_ = 128;
	static const int k_list_number_ = k_align_ / k_max_bytes_;
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
	{
		return (n + k_align_ - 1) / k_align_ - 1 ;
	}
public:
	Alloc_Poor() = default;

	~Alloc_Poor() = default;

	Alloc_Poor(const  Alloc_Poor&) = delete;

	Alloc_Poor& operator=(const  Alloc_Poor&) = delete;

};

}//namespace JStl
#endif