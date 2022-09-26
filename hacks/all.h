#pragma once

#include "b1.reg.cpp"
#include "b2.reg.cpp"
#include "b3.reg.cpp"
#include "b4.reg.cpp"
#include "b5.reg.cpp"

struct rhe_sz_type
{
	const void* data;
	size_t size;
};

constexpr rhe_sz_type rhe_map[6]
{
	{nullptr, 0},
	{b1_reg, sizeof(b1_reg)},
	{b2_reg, sizeof(b2_reg)},
	{b3_reg, sizeof(b3_reg)},
	{b4_reg, sizeof(b4_reg)},
	{b5_reg, sizeof(b5_reg)}
};