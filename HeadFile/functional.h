#ifndef  JIUGESTL_HEADFILE_FUNCTIONAL
#define  JIUGESTL_HEADFILE_FUNCTIONAL

//������������

namespace JStl{

// ����һԪ�����Ĳ����ͱ�ͷ���ֵ�ͱ�
// ���������������
template <class Arg, class Result>
struct unarg_function
{
	typedef Arg       argument_type;
	typedef Result    result_type;
};

// �����Ԫ�����Ĳ����ͱ�ķ���ֵ�ͱ�
template <class Arg1, class Arg2, class Result>
struct binary_function
{
	typedef Arg1      first_argument_type;
	typedef Arg2      second_argument_type;
	typedef Result    result_type;
};

//�ӷ�
template<typename T>
struct plus : public binary_function<T,T,T>  
{
	T operator()(const T& a, const T& b)
	{
		return a + b;
	}
};

//����
template<typename T>
struct minus : public binary_function<T, T, T>
{
	T operator()(const T& a, const T& b)
	{
		return a - b;
	}
};

//�˷�
template<typename T>
struct multiplies : public binary_function<T, T, T>
{
	T operator()(const T& a, const T& b)
	{
		return a * b;
	}
};

//����
template<typename T>
struct divides : public binary_function<T, T, T>
{
	T operator()(const T& a, const T& b)
	{
		return a / b;
	}
};

//ȡģ
template<typename T>
struct modulus : public binary_function<T, T, T>
{
	T operator()(const T& a, const T& b)
	{
		return a % b;
	}
};

//��
template<typename T>
struct negate : public unarg_function<T, T>
{
	T operator()(const T& a)
	{
		return -a;
	}
};

// �ӷ���֤ͬԪ��
template <typename T>
T identity_element(plus<T>) 
{ 
	return T(0); 
}

// �˷���֤ͬԪ��
template <typename T>
T identity_element(multiplies<T>) 
{ 
	return T(1); 
}

// ����
template <typename T>
struct equal_to :public binary_function<T, T, bool>
{
	bool operator()(const T& a, const T& b) const 
	{ 
		return a == b; 
	}
};

// ������
template <typename T>
struct not_equal_to :public binary_function<T, T, bool>
{
	bool operator()(const T& a, const T& b) const 
	{ 
		return a != b; 
	}
};

// ����
template <typename T>
struct greater :public binary_function<T, T, bool>
{
	bool operator()(const T& a, const T& b) const 
	{ 
		return a > b; 
	}
};

// С��
template <typename T>
struct less :public binary_function<T, T, bool>
{
	bool operator()(const T& a, const T& b) const 
	{ 
		return a < b; 
	}
};

// ���ڵ���
template <typename T>
struct greater_equal :public binary_function<T, T, bool>
{
	bool operator()(const T& a, const T& b) const 
	{ 
		return a >= b; 
	}
};

// С�ڵ���
template <typename T>
struct less_equal :public binary_function<T, T, bool>
{
	bool operator()(const T& a, const T& b) const 
	{ 
		return a <= b; 
	}
};

// �߼���
template <typename T>
struct logical_and :public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const 
	{ 
		return x && y; 
	}
};

// �߼���
template <typename T>
struct logical_or :public binary_function<T, T, bool>
{
	bool operator()(const T& x, const T& y) const 
	{ 
		return x || y; 
	}
};

// �߼���
template <typename T>
struct logical_not :public unarg_function<T, bool>
{
	bool operator()(const T& x) const 
	{ 
		return !x; 
	}
};

// ֤ͬ����������ı�Ԫ�أ����ر���
template <typename T>
struct identity :public unarg_function<T, bool>
{
	const T& operator()(const T& x) const 
	{ 
		return x; 
	}
};

// ѡ����������һ�� pair�����ص�һ��Ԫ��
template <class Pair>
struct selectfirst :public unarg_function<Pair, typename Pair::first_type>
{
	const typename Pair::first_type& operator()(const Pair& x) const
	{
		return x.first;
	}
};

// ѡ����������һ�� pair�����صڶ���Ԫ��
template <class Pair>
struct selectsecond :public unarg_function<Pair, typename Pair::second_type>
{
	const typename Pair::second_type& operator()(const Pair& x) const
	{
		return x.second;
	}
};

// Ͷ�亯�������ص�һ����
template <class Arg1, class Arg2>
struct projectfirst :public binary_function<Arg1, Arg2, Arg1>
{
	Arg1 operator()(const Arg1& x, const Arg2&) const 
	{ 
		return x; 
	}
};

// Ͷ�亯�������صڶ�����
template <class Arg1, class Arg2>
struct projectsecond :public binary_function<Arg1, Arg2, Arg1>
{
	Arg2 operator()(const Arg1&, const Arg2& y) const 
	{ 
		return y; 
	}
};

/****************************************************************************/
// ��ϣ��������
//�󲿷ֶ��󣬹�ϣ������
template <class Key>
struct hash {};

// ���ָ���ƫ�ػ��汾
template <class T>
struct hash<T*>
{
	size_t operator()(T* p) const 
	{ 
		//������ڽ�ָ��ת��Ϊ�������߽���ת��Ϊָ��
		return reinterpret_cast<size_t>(p); 
	}
};

//��������������ֵ
//���¹���ģ��ƫ�ػ���ȫ�ػ�
//https://www.cnblogs.com/staring-hxs/p/3659479.html  
template <> 
struct hash<bool>
{
	size_t operator()(bool val) const 
	{
		return static_cast<size_t>(val);
	}
};

template <> 
struct hash<char>
{
	size_t operator()(char val) const
	{
		return static_cast<size_t>(val);
	}
};

template <>
struct hash<signed char>
{
	size_t operator()(signed char val) const
	{
		return static_cast<size_t>(val);
	}
};

template <>
struct hash<unsigned char>
{
	size_t operator()(unsigned char val) const
	{
		return static_cast<size_t>(val);
	}
};

template <>
struct hash<wchar_t>
{
	size_t operator()(wchar_t val) const
	{
		return static_cast<size_t>(val);
	}
};

template <>
struct hash<short>
{
	size_t operator()(short val) const
	{
		return static_cast<size_t>(val);
	}
};

template <>
struct hash<unsigned short>
{
	size_t operator()(unsigned short val) const
	{
		return static_cast<size_t>(val);
	}
};

template <>
struct hash<int>
{
	size_t operator()(int val) const
	{
		return static_cast<size_t>(val);
	}
};

template <>
struct hash<unsigned int>
{
	size_t operator()(unsigned int val) const
	{
		return static_cast<size_t>(val);
	}
};

template <>
struct hash<long>
{
	size_t operator()(long val) const
	{
		return static_cast<size_t>(val);
	}
};

template <>
struct hash<unsigned long>
{
	size_t operator()(unsigned long val) const
	{
		return static_cast<size_t>(val);
	}
};

template <>
struct hash<long long>
{
	size_t operator()(long long val) const
	{
		return static_cast<size_t>(val);
	}
};

template <>
struct hash<unsigned long long>
{
	size_t operator()(unsigned long long val) const
	{
		return static_cast<size_t>(val);
	}
};

//���ڸ����ͣ�����λ����
inline size_t bitwise_hash(const unsigned char* first, size_t count)
{
	const size_t offset = 5721542u;
	const size_t prime = 99266161u;
	size_t result = offset;
	for (size_t i = 0; i < count; ++i)
	{
		result ^= (size_t)first[i];
		result *= prime;
	}
	return result;
}

template <>
struct hash<float>
{
	size_t operator()(const float& val)
	{
		return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(float));
	}
};

template <>
struct hash<double>
{
	size_t operator()(const double& val)
	{
		return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(double));
	}
};

template <>
struct hash<long double>
{
	size_t operator()(const long double& val)
	{
		return val == 0.0f ? 0 : bitwise_hash((const unsigned char*)&val, sizeof(long double));
	}
};

}//namespaec JStl;
#endif