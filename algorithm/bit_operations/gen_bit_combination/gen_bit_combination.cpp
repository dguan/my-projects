#include <iostream>
#include <bitset>


template<class IntType>
inline IntType first_n_1_int(int n)
{
	return static_cast<IntType>((IntType)(-1) >> (sizeof(IntType)*8-n));
}

template<class IntType>
inline IntType last_n_1_int(int n)
{
	return static_cast<IntType>((signed)((IntType)(-1)) & ~(((IntType)1<<(sizeof(IntType)*8-n))-1));
}

template<class IntType>
bool next_n_1_int(IntType& cur_x, int total_bits = sizeof(IntType)* 8)
{
	IntType bit_mask = 0x1;
	int one_cnt = 0;
	int cnt = 0;

	while (cnt++ < total_bits)
	{
		if (cur_x & bit_mask)
		{
			if (!(cur_x & (bit_mask << 1)))
				break;
			else
				++one_cnt;
		}
		bit_mask <<= 1;
	}
	bit_mask <<= 1;
	if (cnt >= total_bits)
		return false;
	cur_x |= bit_mask;
	cur_x &= ~(bit_mask - 1);
	cur_x |= ((1 << one_cnt) - 1);

	return true;
}

template<class IntType>
bool prev_n_1_int(IntType& cur_x, int total_bits = sizeof(IntType)* 8)
{
	IntType bit_mask = 0x1;
	int one_cnt = 0;
	int cnt = 0;
	while (cnt++ < total_bits)
	{
		if (cur_x & bit_mask)
			++one_cnt;
		else if(cur_x & (bit_mask << 1))
			break;
		bit_mask <<= 1;
	}
	if (cnt >= total_bits)
		return false;
	cur_x ^= (bit_mask << 1);
	cur_x |= bit_mask;
	cur_x &= ~(bit_mask - 1);
	cur_x |= (bit_mask - (bit_mask >> one_cnt));

	return true;
}

int main()
{
	std::cout << std::bitset<64>(first_n_1_int<long long>(64)) << std::endl;
	std::cout << std::bitset<64>(last_n_1_int<long long>(64)) << std::endl;
	std::cout << std::bitset<64>(first_n_1_int<unsigned long long>(63)) << std::endl;
	std::cout << std::bitset<64>(last_n_1_int<unsigned long long>(63)) << std::endl;
	std::cout << std::bitset<32>(first_n_1_int<unsigned int>(32)) << std::endl;
	std::cout << std::bitset<32>(last_n_1_int<unsigned int>(32)) << std::endl;
	std::cout << std::bitset<32>(first_n_1_int<unsigned int>(31)) << std::endl;
	std::cout << std::bitset<32>(last_n_1_int<unsigned int>(31)) << std::endl;
	std::cout << std::bitset<16>(first_n_1_int<unsigned short>(16)) << std::endl;
	std::cout << std::bitset<16>(last_n_1_int<unsigned short>(16)) << std::endl;
	std::cout << std::bitset<16>(first_n_1_int<unsigned short>(15)) << std::endl;
	std::cout << std::bitset<16>(last_n_1_int<unsigned short>(15)) << std::endl;

	unsigned long long init_llint = 0x1;
	int cnt = 0;
	do {
		std::cout << std::bitset<sizeof(init_llint)*8>(init_llint) << ", ";
		++cnt;
	} while (next_n_1_int(init_llint));
	std::cout << "Total " << cnt << " integers" << std::endl;

	short init_short = 0x3;
	cnt = 0;
	do {
		std::cout << std::bitset<sizeof(init_short)*8>(init_short) << ", ";
		++cnt;
	} while (next_n_1_int(init_short));
	std::cout << "Total " << cnt << " integers" << std::endl;

	init_short = 0x7;
	cnt = 0;
	do {
		std::cout << std::bitset<8>(init_short) << ", ";
		++cnt;
	} while (next_n_1_int(init_short, 8));
	std::cout << "Total " << cnt << " integers" << std::endl;

	init_short = 0x3;
	cnt = 0;
	do {
		std::cout << std::bitset<4>(init_short) << ", ";
		++cnt;
	} while (next_n_1_int(init_short, 4));
	std::cout << "Total " << cnt << " integers" << std::endl;

	init_short = 0x6;
	cnt = 0;
	do {
		std::cout << std::bitset<4>(init_short) << ", ";
		++cnt;
	} while (next_n_1_int(init_short, 4));
	std::cout << "Total " << cnt << " integers" << std::endl;

	std::cout << std::endl << "******** Test prev integer ********" << std::endl << std::endl;

	int init_int = last_n_1_int<int>(1);
	cnt = 0;
	do {
		std::cout << std::bitset<sizeof(init_int)* 8>(init_int) << ", ";
		++cnt;
	} while (prev_n_1_int(init_int));
	std::cout << "Total " << cnt << " integers" << std::endl;

	init_int = last_n_1_int<short>(2);
	cnt = 0;
	do {
		std::cout << std::bitset<sizeof(short)* 8>(init_int) << ", ";
		++cnt;
	} while (prev_n_1_int(init_int, sizeof(short)*8));
	std::cout << "Total " << cnt << " integers" << std::endl;

	init_int = 0xe0;
	cnt = 0;
	do {
		std::cout << std::bitset<8>(init_int) << ", ";
		++cnt;
	} while (prev_n_1_int(init_int, 8));
	std::cout << "Total " << cnt << " integers" << std::endl;

	init_int = 0xc;
	cnt = 0;
	do {
		std::cout << std::bitset<4>(init_int) << ", ";
		++cnt;
	} while (prev_n_1_int(init_int, 4));
	std::cout << "Total " << cnt << " integers" << std::endl;

	init_int = 0x6;
	cnt = 0;
	do {
		std::cout << std::bitset<4>(init_int) << ", ";
		++cnt;
	} while (prev_n_1_int(init_int, 4));
	std::cout << "Total " << cnt << " integers" << std::endl;

	system("pause");
	return 0;
}

