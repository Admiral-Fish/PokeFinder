/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#if SIMD && (defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64))
#define SIMD_X86
#include <smmintrin.h>
using vuint32x4 = __m128i;
#elif SIMD && (defined(__arm__) || defined(_M_ARM) || defined(__aarch64__))
#define SIMD_ARM
#include <arm_neon.h>
using vuint32x4 = uint32x4_t;
#else
#include <array>
using vuint32x4 = std::array<u32, 4>;
#endif

union alignas(16) vuint128 {
    vuint32x4 uint128;
    u64 uint64[2];
    u32 uint32[4];

    /**
     * @brief Construct a new vuint128 object
     */
    vuint128() = default;

    /**
     * @brief Construct a new vuint128 object
     *
     * @param x Initalization number
     */
    vuint128(u32 x)
    {
#if defined(SIMD_X86)
        uint128 = _mm_set1_epi32(x);
#elif defined(SIMD_ARM)
        uint128 = vdupq_n_u32(x);
#else
        uint128 = { x, x, x, x };
#endif
    }

    /**
     * @brief Construct a new vuint128 object
     *
     * @param x0 Initalization number0
     * @param x1 Initalization number1
     * @param x2 Initalization number2
     * @param x3 Initalization number3
     */
    vuint128(u32 x0, u32 x1, u32 x2, u32 x3)
    {
#if defined(SIMD_X86)
        uint128 = _mm_set_epi32(x3, x2, x1, x0);
#elif defined(SIMD_ARM)
        u32 data[4] = { x0, x1, x2, x3 };
        uint128 = vld1q_u32(data);
#else
        uint128 = { x0, x1, x2, x3 };
#endif
    }

    /**
     * @brief Returns reference to u32 data element
     *
     * @param i Index
     */
    inline u32 &operator[](int i)
    {
        return uint32[i];
    }

    /**
     * @brief Computes the bitwise NOT of each 32bit number pair in the vector
     *
     * @return Computed bitwise NOT vector
     */
    inline vuint128 operator~() const
    {
        vuint128 ret;
#if defined(SIMD_X86)
        ret.uint128 = _mm_xor_si128(uint128, _mm_cmpeq_epi32(uint128, uint128));
#elif defined(SIMD_ARM)
        ret.uint128 = veorq_u32(vceqq_u32(uint128, uint128));
#else
        for (int i = 0; i < 4; i++)
        {
            ret.uint32[i] = ~uint32[i];
        }
#endif
        return ret;
    }

    /**
     * @brief Computes the bitwise ADD of each 32bit number pair in the vector
     *
     * @param y Second operand
     *
     * @return Computed bitwise ADD vector
     */
    inline vuint128 operator+(vuint128 y) const
    {
        vuint128 ret;
#if defined(SIMD_X86)
        ret.uint128 = _mm_add_epi32(uint128, y.uint128);
#elif defined(SIMD_ARM)
        ret.uint128 = vaddq_u32(uint128, y.uint128);
#else
        for (int i = 0; i < 4; i++)
        {
            ret.uint32[i] = uint32[i] + y.uint32[i];
        }
#endif
        return ret;
    }

    /**
     * @brief Computes the bitwise left shift of each 32bit number pair in the vector
     *
     * @param shift Amount to shift by
     *
     * @return Computed bitwise left shift vector
     */
    inline vuint128 operator<<(int shift) const
    {
        vuint128 ret;
#if defined(SIMD_X86)
        ret.uint128 = _mm_slli_epi32(uint128, shift);
#elif defined(SIMD_ARM)
        ret.uint128 = vshlq_n_u32(uint128, shift);
#else
        for (int i = 0; i < 4; i++)
        {
            ret.uint32[i] = uint32[i] << shift;
        }
#endif
        return ret;
    }

    /**
     * @brief Computes the bitwise right shift of each 32bit number pair in the vector
     *
     * @param shift Amount to shift by
     *
     * @return Computed bitwise right shift vector
     */
    inline vuint128 operator>>(int shift) const
    {
        vuint128 ret;
#if defined(SIMD_X86)
        ret.uint128 = _mm_srli_epi32(uint128, shift);
#elif defined(SIMD_ARM)
        ret.uint128 = vshrq_n_u32(uint128, shift);
#else
        for (int i = 0; i < 4; i++)
        {
            ret.uint32[i] = uint32[i] >> shift;
        }
#endif
        return ret;
    }

    /**
     * @brief Compares each 32bit number pair in the vector for equality. Assigns 0xFFFFFFFF on equal and 0 when not
     *
     * @param y Second operand
     *
     * @return Computed equality vector
     */
    inline vuint128 operator==(vuint128 y) const
    {
        vuint128 ret;
#if defined(SIMD_X86)
        ret.uint128 = _mm_cmpeq_epi32(uint128, y.uint128);
#elif defined(SIMD_ARM)
        ret.uint128 = vceqq_u32(uint128, y.uint128);
#else
        for (int i = 0; i < 4; i++)
        {
            ret.uint32[i] = (uint32[i] == y.uint32[i]) ? 0xffffffff : 0;
        }
#endif
        return ret;
    }

    /**
     * @brief Computes the bitwise AND of each 32bit number pair in the vector
     *
     * @param y Second operand
     *
     * @return Computed bitwise AND vector
     */
    inline vuint128 operator&(vuint128 y) const
    {
        vuint128 ret;
#if defined(SIMD_X86)
        ret.uint128 = _mm_and_si128(uint128, y.uint128);
#elif defined(SIMD_ARM)
        ret.uint128 = vandq_u32(uint128, y.uint128);
#else
        for (int i = 0; i < 4; i++)
        {
            ret.uint32[i] = uint32[i] & y.uint32[i];
        }
#endif
        return ret;
    }

    /**
     * @brief Computes the bitwise XOR of each 32bit number pair in the vector
     *
     * @param y Second operand
     *
     * @return Computed bitwise XOR vector
     */
    inline vuint128 operator^(vuint128 y) const
    {
        vuint128 ret;
#if defined(SIMD_X86)
        ret.uint128 = _mm_xor_si128(uint128, y.uint128);
#elif defined(SIMD_ARM)
        ret.uint128 = veorq_u32(uint128, y.uint128);
#else
        for (int i = 0; i < 4; i++)
        {
            ret.uint32[i] = uint32[i] ^ y.uint32[i];
        }
#endif
        return ret;
    }

    /**
     * @brief Computes the bitwise OR of each 32bit number pair in the vector
     *
     * @param y Second operand
     *
     * @return Computed bitwise OR vector
     */
    inline vuint128 operator|(vuint128 y) const
    {
        vuint128 ret;
#if defined(SIMD_X86)
        ret.uint128 = _mm_or_si128(uint128, y.uint128);
#elif defined(SIMD_ARM)
        ret.uint128 = vorrq_u32(uint128, y.uint128);
#else
        for (int i = 0; i < 4; i++)
        {
            ret.uint32[i] = uint32[i] | y.uint32[i];
        }
#endif
        return ret;
    }
};

/**
 * @brief Loads vector from memory
 *
 * @param address Memory address
 *
 * @return Loaded vector
 */
inline vuint128 v32x4_load(const u32 *address)
{
    vuint128 ret;
#if defined(SIMD_X86)
    ret.uint128 = _mm_loadu_si128((const vuint32x4 *)address);
#elif defined(SIMD_ARM)
    ret.uint128 = vld1q_u32(address);
#else
    ret.uint128 = { address[0], address[1], address[2], address[3] };
#endif
    return ret;
}

/**
 * @brief Computes the bitwise rotate left of each 32bit number pair in the vector
 *
 * @tparam rotate Amount to rotateby
 * @param x Input vector
 *
 * @return Computed bitwise rotate left vector
 */
template <int rotate>
inline vuint128 v32x4_rotl(vuint128 x)
{
    return (x << rotate) | (x >> (32 - rotate));
}

/**
 * @brief Computes the bitwise rotate right of each 32bit number pair in the vector
 *
 * @tparam rotate Amount to rotateby
 * @param x Input vector
 *
 * @return Computed bitwise rotate right vector
 */
template <int rotate>
inline vuint128 v32x4_rotr(vuint128 x)
{
    return (x >> rotate) | (x << (32 - rotate));
}

/**
 * @brief Store vector into memory
 *
 * @param address Memory address
 *
 * @param value Vector to store
 */
inline void v32x4_store(u32 *address, vuint128 value)
{
#if defined(SIMD_X86)
    _mm_storeu_si128((vuint32x4 *)address, value.uint128);
#elif defined(SIMD_ARM)
    vst1q_u32(address, value.uint128);
#else
    for (int i = 0; i < 4; i++)
    {
        address[i] = value.uint32[i];
    }
#endif
}

/**
 * @brief Computes the bitwise left shift of a 128bit number
 *
 * @tparam shift Amount to shift by
 * @param x Input vector
 *
 * @return Computed bitwise left shift vector
 */
template <int shift>
inline vuint128 v128_shl(vuint128 x)
{
    vuint128 ret;
#if defined(SIMD_X86)
    ret.uint128 = _mm_slli_si128(x.uint128, shift);
#elif defined(SIMD_ARM)
    ret.uint128 = (uint32x4_t)vextq_u8(vdupq_n_u8(0), (uint8x16_t)x.uint128, 16 - shift);
#else
    u64 th = ((u64)x.uint32[3] << 32) | ((u64)x.uint32[2]);
    u64 tl = ((u64)x.uint32[1] << 32) | ((u64)x.uint32[0]);

    u64 oh = (th << (shift * 8)) | (tl >> (64 - shift * 8));
    u64 ol = tl << (shift * 8);

    ret.uint32[1] = ol >> 32;
    ret.uint32[0] = ol & 0xffffffff;
    ret.uint32[3] = oh >> 32;
    ret.uint32[2] = oh & 0xffffffff;
#endif
    return ret;
}

/**
 * @brief Computes the bitwise right shift of a 128bit number
 *
 * @tparam shift Amount to shift by
 * @param x Input vector
 *
 * @return Computed bitwise right shift vector
 */
template <int shift>
inline vuint128 v128_shr(vuint128 x)
{
    vuint128 ret;
#if defined(SIMD_X86)
    ret.uint128 = _mm_srli_si128(x.uint128, shift);
#elif defined(SIMD_ARM)
    ret.uint128 = (uint32x4_t)vextq_u8((uint8x16_t)x.uint128, vdupq_n_u8(0), shift);
#else
    u64 th = ((u64)x.uint32[3] << 32) | ((u64)x.uint32[2]);
    u64 tl = ((u64)x.uint32[1] << 32) | ((u64)x.uint32[0]);

    u64 oh = th >> (shift * 8);
    u64 ol = (tl >> (shift * 8)) | (th << (64 - shift * 8));

    ret.uint32[1] = ol >> 32;
    ret.uint32[0] = ol & 0xffffffff;
    ret.uint32[3] = oh >> 32;
    ret.uint32[2] = oh & 0xffffffff;
#endif
    return ret;
}

#endif // SIMD_HPP
