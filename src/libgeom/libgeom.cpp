// libgeom.cpp : スタティック ライブラリ用の関数を定義します。
//
#include "pch.h"
#include <intrin.h>
#include "../include/libgeom.h"

// 4つのfloat が num個の配列をsrcからdstにコピー
void copy_vector4_array(float* dst, const float* src, int num)
{
	const int byte = 4;
	__m128 ps;
	__m128 pd;
	__m128 ps0 = { 0.0f };

	for (int i = 0; i < num; i++)
	{
		int byte_times_i = byte * i;

		ps = _mm_load_ps(src + byte_times_i);

		pd = _mm_add_ps(ps, ps0);

		_mm_store_ps(dst + byte_times_i, pd);
	}
}
// 4つのfloat が num個の配列 src0, src1を足してdstに出力
void add_vector4_array(float* dst, const float* src0, const float* src1, int num)
{
	const int byte = 4;

	for (int i = 0; i < num; i++)
	{
		int byte_times_i = byte * i;

		__m128 ps0 = _mm_load_ps(src0 + byte_times_i);

		__m128 ps1 = _mm_load_ps(src1 + byte_times_i);

		__m128 pd = _mm_add_ps(ps0, ps1);

		_mm_store_ps(dst + byte_times_i, pd);
	}
}

// 4つのfloat に num個の配列 src にmatrix[4][4] の行列を掛けてdstに出力
void apply_matrix_vector4_array(float* dst, const float* src, const float* matrix, int num)
{
#if 1
	float* pd = dst;
	const float* ps = src;
	for (int i = 0; i < num; i++) 
	{
		pd[0] = matrix[4 * 0 + 0] * ps[0] + matrix[4 * 0 + 1] * ps[1] + matrix[4 * 0 + 2] * ps[2] + matrix[4 * 0 + 3] * ps[3];
		pd[1] = matrix[4 * 1 + 0] * ps[0] + matrix[4 * 1 + 1] * ps[1] + matrix[4 * 1 + 2] * ps[2] + matrix[4 * 1 + 3] * ps[3];
		pd[2] = matrix[4 * 2 + 0] * ps[0] + matrix[4 * 2 + 1] * ps[1] + matrix[4 * 2 + 2] * ps[2] + matrix[4 * 2 + 3] * ps[3];
		pd[3] = matrix[4 * 3 + 0] * ps[0] + matrix[4 * 3 + 1] * ps[1] + matrix[4 * 3 + 2] * ps[2] + matrix[4 * 3 + 3] * ps[3];
		
		pd += 4;
		ps += 4;
	}

//↓自分で考えたSIMD計算(うまくいかず。)↓
#else

	const int byte = 4;

	__m128 ps;
	__m128 a;
	__m128 b;
	__m128 pd;
	__m128 a1;

	for (int i = 0; i < num; i++)
	{
		int byte_times_i = byte * i;

		for (int j = 0; j < byte; j++)
		{
			a = _mm_load_ps(matrix + byte_times_i);

			for (int k = 0; k < byte; k++)
			{	
				ps = _mm_load_ps(src + k);
			}
			
			a1 = _mm_mul_ps(ps, a);
		}

		pd = _mm_add_ps(a1, a1);

		_mm_store_ps(dst + byte_times_i, pd);
	}
#endif
}
