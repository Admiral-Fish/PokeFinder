/*
 * This file is part of PokéFinder
 * Copyright (C) 2020 by Admiral_Fish, bumba, Dark Kirb, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef SIMD_HPP
#define SIMD_HPP

#include <Core/Util/Global.hpp>

#if defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64)
#include <smmintrin.h>
using vuint32x4 = __m128i;

inline vuint32x4 v32x4_load(u32 *address)
{
    return _mm_loadu_si128((const vuint32x4 *)address);
}

inline void v32x4_store(u32 *address, vuint32x4 value)
{
    _mm_storeu_si128((vuint32x4 *)address, value);
}

inline vuint32x4 v32x4_set(u32 x)
{
    return _mm_set1_epi32(x);
}

inline vuint32x4 v32x4_set(u32 x0, u32 x1, u32 x2, u32 x3)
{
    return _mm_set_epi32(x3, x2, x1, x0);
}

inline vuint32x4 v32x4_shr(vuint32x4 value, int num)
{
    return _mm_srli_epi32(value, num);
}

inline vuint32x4 v32x4_shl(vuint32x4 value, int num)
{
    return _mm_slli_epi32(value, num);
}

inline vuint32x4 v32x4_and(vuint32x4 x, vuint32x4 y)
{
    return _mm_and_si128(x, y);
}

inline vuint32x4 v32x4_xor(vuint32x4 x, vuint32x4 y)
{
    return _mm_xor_si128(x, y);
}

inline vuint32x4 v32x4_or(vuint32x4 x, vuint32x4 y)
{
    return _mm_or_si128(x, y);
}

inline vuint32x4 v32x4_cmpeq(vuint32x4 x, vuint32x4 y)
{
    return _mm_cmpeq_epi32(x, y);
}

template <int shift>
inline vuint32x4 v128_shr(vuint32x4 x)
{
    static_assert(shift == 1, "Only usage has a value of 1");
    return _mm_srli_si128(x, shift);
}

template <int shift>
inline vuint32x4 v128_shl(vuint32x4 x)
{
    static_assert(shift == 1, "Only usage has a value of 1");
    return _mm_slli_si128(x, shift);
}

template <int lane>
inline vuint32x4 v32x4_insert(vuint32x4 value, u32 insert)
{
    static_assert(lane == 3, "Only usage has a value of 3");
    return _mm_insert_epi32(value, insert, lane);
}
#elif defined(__arm__) || defined(_M_ARM) || defined(__aarch64__)
#include <arm_neon.h>
using vuint32x4 = uint32x4_t;

inline vuint32x4 v32x4_load(u32 *address)
{
    return vld1q_u32(address);
}

inline void v32x4_store(u32 *address, vuint32x4 value)
{
    vst1q_u32(address, value);
}

inline vuint32x4 v32x4_set(u32 x)
{
    return vdupq_n_u32(x);
}

inline vuint32x4 v32x4_set(u32 x0, u32 x1, u32 x2, u32 x3)
{
    u32 data[4] = { x0, x1, x2, x3 };
    return vld1q_u32(data);
}

inline vuint32x4 v32x4_shr(vuint32x4 value, int num)
{
    return vshrq_n_u32(value, num);
}

inline vuint32x4 v32x4_shl(vuint32x4 value, int num)
{
    return vshlq_n_u32(value, num);
}

inline vuint32x4 v32x4_and(vuint32x4 x, vuint32x4 y)
{
    return vandq_u32(x, y);
}

inline vuint32x4 v32x4_xor(vuint32x4 x, vuint32x4 y)
{
    return veorq_u32(x, y);
}

inline vuint32x4 v32x4_or(vuint32x4 x, vuint32x4 y)
{
    return vorrq_u32(x, y);
}

inline vuint32x4 v32x4_cmpeq(vuint32x4 x, vuint32x4 y)
{
    return vceqq_u32(x, y);
}

template <int shift>
inline vuint32x4 v128_shr(vuint32x4 x)
{
    static_assert(shift == 1, "Only usage has a value of 1");
    return (uint32x4_t)vextq_u8((uint8x16_t)x, vdupq_n_u8(0), shift);
}

template <int shift>
inline vuint32x4 v128_shl(vuint32x4 x)
{
    static_assert(shift == 1, "Only usage has a value of 1");
    return (uint32x4_t)vextq_u8(vdupq_n_u8(0), (uint8x16_t)x, 16 - shift);
}

template <int lane>
inline vuint32x4 v32x4_insert(vuint32x4 value, u32 insert)
{
    static_assert(lane == 3, "Only usage has a value of 3");
    return vsetq_lane_u32(insert, value, lane);
}
#else
#error "No supported SIMD Instruction set found!"
#endif

#endif // SIMD_HPP
