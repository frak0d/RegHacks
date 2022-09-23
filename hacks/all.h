#pragma once

#include "b1.reg.cpp"
#include "b2.reg.cpp"
#include "b3.reg.cpp"
//#include "b4.reg.cpp"
//#include "b5.reg.cpp"

struct rhe_sz_type
{
	const void* data;
	size_t size;
};

constexpr rhe_sz_type rhe_map[7] =
{
	{nullptr, 0},
	{b1_reg, b1_reg_sz},
	{b2_reg, b2_reg_sz},
	{b3_reg, b3_reg_sz},
//	{b4_reg, b4_reg_sz},
//	{b5_reg, b5_reg_sz}
};