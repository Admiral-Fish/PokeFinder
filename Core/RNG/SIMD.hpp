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

#ifndef __CORE_SIMD_HPP
#define __CORE_SIMD_HPP

#if defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_AMD64)
#include <smmintrin.h>
#define __USE_SSE
#ifdef USE_AVX2
#include <immintrin.h>
#define __USE_AVX
#define SIMD_256BIT
#endif
#elif defined(__arm__) || defined(_M_ARM) || defined(__aarch64__)
#include <arm_neon.h>
#define __USE_NEON
#else
#error "No supported SIMD Instruction set found!"
#endif

#ifdef __USE_SSE
typedef __m128i vuint32x4;
#define v32x4_splat(x) _mm_set1_epi32((x))
#define v32x4_load(x) _mm_loadu_si128((const vuint32x4 *)(x))
#define v32x4_or(x, y) _mm_or_si128((x), (y))
#define v32x4_and(x, y) _mm_and_si128((x), (y))
#define v32x4_xor(x, y) _mm_xor_si128((x), (y))
#define v32x4_shr(x, y) _mm_srli_epi32((x), (y))
#define v32x4_shl(x, y) _mm_slli_epi32((x), (y))
#define v8x16_shr(x, y) _mm_srli_si128((x), (y))
#define v8x16_shl(x, y) _mm_slli_si128((x), (y))
#define v32x4_cmpeq(x, y) _mm_cmpeq_epi32((x), (y))
#define v32x4_store(x, y) _mm_storeu_si128((vuint32x4 *)(x), (y))
#define v32x4_insert(x, y, z) _mm_insert_epi32((x), (y), (z))
#endif
#ifdef __USE_AVX
typedef __m256i vuint32x8;
#define v32x8_splat(x) _mm256_set1_epi32((x))
#define v32x8_load(x) _mm256_loadu_si256((const vuint32x8 *)(x))
#define v32x8_or(x, y) _mm256_or_si256((x), (y))
#define v32x8_and(x, y) _mm256_and_si256((x), (y))
#define v32x8_xor(x, y) _mm256_xor_si256((x), (y))
#define v32x8_shr(x, y) _mm256_srli_epi32((x), (y))
#define v32x8_cmpeq(x, y) _mm256_cmpeq_epi32((x), (y))
#define v32x8_store(x, y) _mm256_storeu_si256((vuint32x8 *)(x), (y))
#endif
#ifdef __USE_NEON
typedef uint32x4_t vuint32x4;
#define v32x4_splat(x) vdupq_n_u32((x))
#define v32x4_load(x) vld1q_u32((const uint32_t *)(x))
#define v32x4_or(x, y) vorrq_u32((x), (y))
#define v32x4_and(x, y) vandq_u32((x), (y))
#define v32x4_xor(x, y) veorq_u32((x), (y))
#define v32x4_shr(x, y) vshrq_n_u32((x), (y))
#define v32x4_shl(x, y) vshlq_n_u32((x), (y))
#define v8x16_shr(x, y) ((uint32x4_t)vextq_u8((uint8x16_t)(x), vdupq_n_u8(0), (y)))
#define v8x16_shl(x, y) ((uint32x4_t)vextq_u8(vdupq_n_u8(0), (uint8x16_t)(x), (y)))
#define v32x4_cmpeq(x, y) vceqq_u32((x), (y))
#define v32x4_store(x, y) vst1q_u32((uint32_t *)(x), (y))
#define v32x4_insert(x, y, z) vsetq_lane_u32((y), (x), (z))
#endif

#endif
