/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include <Core/Global.hpp>

#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
#include <smmintrin.h>
using vuint32x4 = __m128i;
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
#include <arm_neon.h>
using vuint32x4 = uint32x4_t;
#else
#include <array>
using vuint32x4 = std::array<u32, 4>;
#endif

template <bool aligned = true>
inline vuint32x4 v32x4_load(const u32 *address)
{
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
    if constexpr (aligned)
    {
        return _mm_load_si128((const vuint32x4 *)address);
    }
    else
    {
        return _mm_loadu_si128((const vuint32x4 *)address);
    }
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
    return vld1q_u32(address);
#else
    return { address[0], address[1], address[2], address[3] };
#endif
}

template <bool aligned = true>
inline void v32x4_store(u32 *address, vuint32x4 value)
{
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
    if constexpr (aligned)
    {
        _mm_store_si128((vuint32x4 *)address, value);
    }
    else
    {
        _mm_storeu_si128((vuint32x4 *)address, value);
    }
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
    vst1q_u32(address, value);
#else
    for (int i = 0; i < 4; i++)
    {
        address[i] = value[i];
    }
#endif
}

inline vuint32x4 v32x4_set(u32 x)
{
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
    return _mm_set1_epi32(x);
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
    return vdupq_n_u32(x);
#else
    return { x, x, x, x };
#endif
}

inline vuint32x4 v32x4_set(u32 x0, u32 x1, u32 x2, u32 x3)
{
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
    return _mm_set_epi32(x3, x2, x1, x0);
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
    u32 data[4] = { x0, x1, x2, x3 };
    return vld1q_u32(data);
#else
    return { x0, x1, x2, x3 };
#endif
}

template <int shift>
inline vuint32x4 v32x4_shr(vuint32x4 value)
{
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
    return _mm_srli_epi32(value, shift);
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
    return vshrq_n_u32(value, shift);
#else
    for (int i = 0; i < 4; i++)
    {
        value[i] >>= shift;
    }
    return value;
#endif
}

template <int shift>
inline vuint32x4 v32x4_shl(vuint32x4 value)
{
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
    return _mm_slli_epi32(value, shift);
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
    return vshlq_n_u32(value, shift);
#else
    for (int i = 0; i < 4; i++)
    {
        value[i] <<= shift;
    }
    return value;
#endif
}

inline vuint32x4 v32x4_and(vuint32x4 x, vuint32x4 y)
{
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
    return _mm_and_si128(x, y);
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
    return vandq_u32(x, y);
#else
    for (int i = 0; i < 4; i++)
    {
        x[i] &= y[i];
    }
    return x;
#endif
}

inline vuint32x4 v32x4_xor(vuint32x4 x, vuint32x4 y)
{
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
    return _mm_xor_si128(x, y);
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
    return veorq_u32(x, y);
#else
    for (int i = 0; i < 4; i++)
    {
        x[i] ^= y[i];
    }
    return x;
#endif
}

inline vuint32x4 v32x4_or(vuint32x4 x, vuint32x4 y)
{
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
    return _mm_or_si128(x, y);
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
    return vorrq_u32(x, y);
#else
    for (int i = 0; i < 4; i++)
    {
        x[i] |= y[i];
    }
    return x;
#endif
}

inline vuint32x4 v32x4_cmpeq(vuint32x4 x, vuint32x4 y)
{
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
    return _mm_cmpeq_epi32(x, y);
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
    return vceqq_u32(x, y);
#else
    for (int i = 0; i < 4; i++)
    {
        x[i] = (x[i] == y[i]) ? 0xffffffff : 0;
    }
    return x;
#endif
}

template <int shift>
inline vuint32x4 v128_shr(vuint32x4 x)
{
    static_assert(shift == 1, "Only usage has a value of 1");
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
    return _mm_srli_si128(x, shift);
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
    return (uint32x4_t)vextq_u8((uint8x16_t)x, vdupq_n_u8(0), shift);
#else
    u64 th = ((u64)x[3] << 32) | ((u64)x[2]);
    u64 tl = ((u64)x[1] << 32) | ((u64)x[0]);

    u64 oh = th >> (shift * 8);
    u64 ol = (tl >> (shift * 8)) | (th << (64 - shift * 8));

    x[1] = ol >> 32;
    x[0] = ol & 0xffffffff;
    x[3] = oh >> 32;
    x[2] = oh & 0xffffffff;

    return x;
#endif
}

template <int shift>
inline vuint32x4 v128_shl(vuint32x4 x)
{
    static_assert(shift == 1, "Only usage has a value of 1");
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
    return _mm_slli_si128(x, shift);
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
    return (uint32x4_t)vextq_u8(vdupq_n_u8(0), (uint8x16_t)x, 16 - shift);
#else
    u64 th = ((u64)x[3] << 32) | ((u64)x[2]);
    u64 tl = ((u64)x[1] << 32) | ((u64)x[0]);

    u64 oh = (th << (shift * 8)) | (tl >> (64 - shift * 8));
    u64 ol = tl << (shift * 8);

    x[1] = ol >> 32;
    x[0] = ol & 0xffffffff;
    x[3] = oh >> 32;
    x[2] = oh & 0xffffffff;

    return x;
#endif
}

template <int lane>
inline vuint32x4 v32x4_insert(vuint32x4 value, u32 insert)
{
    static_assert(lane == 3, "Only usage has a value of 3");
#if (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
    return _mm_insert_epi32(value, insert, lane);
#elif (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
    return vsetq_lane_u32(insert, value, lane);
#else
    value[lane] = insert;
    return value;
#endif
}

#endif // SIMD_HPP
