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

#include "UndergroundGenerator.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen8/States/UndergroundState.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xorshift.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>

struct EggMoveList
{
    constexpr EggMoveList(u8 count, u16 specie, const std::array<u16, 16> &moves) : moves(moves), specie(specie), count(count)
    {
    }

    std::array<u16, 16> moves;
    u16 specie;
    u8 count;
};

struct LevelInfo
{
    constexpr LevelInfo(u8 min, u8 max) : max(max), min(min)
    {
    }

    u8 max;
    u8 min;
};

// clang-format off
constexpr std::array<EggMoveList, 141> eggMoveList = {
    EggMoveList(12, 1, { 130, 80, 174, 275, 267, 204, 345, 133, 437, 438, 124, 580, 0, 0, 0, 0 }),
    EggMoveList(13, 4, { 187, 246, 44, 407, 232, 68, 17, 525, 200, 251, 349, 242, 314, 0, 0, 0 }),
    EggMoveList(12, 7, { 243, 114, 54, 175, 281, 252, 392, 453, 323, 791, 330, 396, 0, 0, 0, 0 }),
    EggMoveList(9, 19, { 103, 172, 154, 68, 179, 253, 387, 279, 515, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(9, 23, { 21, 180, 251, 305, 184, 342, 50, 415, 389, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 27, { 68, 175, 189, 400, 232, 468, 306, 341, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(11, 29, { 68, 50, 342, 130, 48, 36, 305, 116, 204, 251, 599, 0, 0, 0, 0, 0 }),
    EggMoveList(13, 32, { 93, 68, 50, 457, 342, 389, 48, 36, 32, 37, 133, 251, 599, 0, 0, 0 }),
    EggMoveList(10, 37, { 95, 175, 336, 262, 608, 488, 257, 394, 384, 506, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(9, 41, { 174, 16, 95, 98, 18, 17, 428, 413, 599, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(11, 43, { 75, 175, 235, 275, 321, 298, 267, 495, 668, 73, 204, 0, 0, 0, 0, 0 }),
    EggMoveList(13, 46, { 103, 68, 60, 175, 230, 232, 450, 440, 97, 73, 469, 565, 580, 0, 0, 0 }),
    EggMoveList(7, 48, { 226, 103, 97, 234, 390, 450, 476, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(9, 54, { 499, 109, 238, 95, 60, 493, 281, 248, 227, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(6, 56, { 68, 179, 251, 279, 227, 400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(10, 58, { 37, 38, 234, 343, 24, 34, 83, 257, 370, 682, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(6, 60, { 283, 114, 54, 170, 150, 227, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(12, 63, { 93, 470, 277, 227, 282, 7, 9, 8, 379, 385, 375, 502, 0, 0, 0, 0 }),
    EggMoveList(12, 66, { 68, 418, 321, 501, 66, 227, 370, 7, 9, 8, 379, 484, 0, 0, 0, 0 }),
    EggMoveList(13, 69, { 227, 235, 141, 275, 345, 388, 321, 311, 499, 438, 491, 562, 668, 0, 0, 0 }),
    EggMoveList(10, 72, { 367, 392, 62, 109, 114, 282, 243, 229, 321, 330, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(7, 74, { 5, 335, 359, 175, 174, 475, 469, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(10, 77, { 37, 24, 95, 38, 32, 234, 204, 67, 502, 667, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(7, 79, { 562, 187, 335, 23, 248, 173, 472, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(13, 83, { 16, 98, 175, 297, 174, 343, 400, 493, 515, 364, 143, 189, 279, 0, 0, 0 }),
    EggMoveList(5, 84, { 48, 114, 175, 413, 372, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(12, 86, { 122, 195, 50, 32, 21, 252, 333, 254, 256, 255, 562, 494, 0, 0, 0, 0 }),
    EggMoveList(13, 88, { 114, 212, 122, 286, 174, 325, 425, 254, 256, 255, 184, 491, 612, 0, 0, 0 }),
    EggMoveList(9, 90, { 61, 392, 791, 36, 229, 103, 333, 341, 350, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(12, 92, { 195, 114, 310, 288, 50, 499, 123, 513, 7, 8, 9, 184, 0, 0, 0, 0 }),
    EggMoveList(9, 95, { 175, 335, 111, 469, 205, 525, 457, 350, 484, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 96, { 272, 7, 9, 8, 260, 427, 385, 471, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(10, 98, { 282, 246, 359, 163, 400, 114, 133, 321, 97, 502, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(10, 104, { 246, 187, 197, 24, 195, 130, 174, 43, 103, 442, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(9, 108, { 562, 174, 359, 37, 34, 173, 133, 330, 428, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(11, 109, { 60, 220, 288, 180, 174, 254, 256, 255, 103, 390, 599, 0, 0, 0, 0, 0 }),
    EggMoveList(12, 111, { 68, 174, 407, 130, 368, 470, 242, 179, 306, 423, 424, 422, 0, 0, 0, 0 }),
    EggMoveList(9, 114, { 93, 283, 175, 73, 267, 476, 133, 437, 384, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 116, { 62, 499, 50, 175, 190, 150, 330, 200, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(9, 122, { 95, 252, 109, 471, 321, 248, 271, 478, 196, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(6, 123, { 68, 400, 364, 501, 226, 179, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(6, 127, { 364, 175, 37, 31, 98, 370, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(6, 147, { 453, 225, 245, 54, 48, 114, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(9, 152, { 22, 73, 68, 246, 175, 275, 437, 505, 580, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(10, 155, { 154, 179, 37, 343, 336, 306, 24, 394, 326, 267, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 158, { 246, 8, 232, 349, 453, 313, 335, 260, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 161, { 38, 163, 116, 271, 387, 204, 343, 608, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(9, 163, { 48, 17, 18, 297, 101, 143, 97, 212, 542, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(7, 165, { 60, 103, 227, 282, 450, 366, 68, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 167, { 60, 50, 226, 390, 476, 400, 224, 679, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(7, 170, { 60, 54, 487, 103, 133, 250, 97, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(7, 175, { 217, 64, 375, 326, 234, 248, 500, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 177, { 65, 98, 297, 389, 493, 114, 428, 502, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 179, { 34, 103, 260, 28, 495, 97, 598, 604, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(10, 187, { 93, 227, 38, 133, 270, 312, 538, 402, 580, 668, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(10, 190, { 68, 180, 21, 251, 252, 343, 340, 279, 415, 501, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(7, 191, { 227, 267, 174, 270, 230, 234, 580, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(15, 194, { 246, 174, 254, 256, 255, 68, 24, 105, 495, 491, 612, 34, 227, 385, 598, 0 }),
    EggMoveList(10, 198, { 18, 65, 143, 109, 297, 195, 375, 103, 413, 260, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 200, { 103, 194, 286, 262, 389, 425, 174, 472, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(9, 203, { 36, 133, 248, 251, 273, 277, 24, 243, 212, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 204, { 42, 175, 129, 68, 328, 279, 390, 379, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(12, 207, { 232, 17, 68, 328, 97, 226, 38, 364, 400, 440, 379, 342, 0, 0, 0, 0 }),
    EggMoveList(8, 209, { 118, 217, 215, 173, 370, 38, 102, 313, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(7, 211, { 175, 114, 61, 48, 310, 453, 491, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(12, 213, { 230, 282, 367, 51, 515, 111, 611, 343, 270, 328, 189, 350, 0, 0, 0, 0 }),
    EggMoveList(11, 215, { 68, 180, 44, 252, 458, 420, 364, 556, 306, 8, 675, 0, 0, 0, 0, 0 }),
    EggMoveList(12, 216, { 242, 36, 69, 68, 232, 281, 238, 38, 370, 400, 187, 583, 0, 0, 0, 0 }),
    EggMoveList(11, 218, { 151, 257, 174, 108, 262, 254, 255, 256, 205, 517, 385, 0, 0, 0, 0, 0 }),
    EggMoveList(11, 220, { 44, 246, 38, 90, 174, 556, 573, 34, 341, 333, 341, 0, 0, 0, 0, 0 }),
    EggMoveList(10, 222, { 54, 109, 267, 174, 457, 103, 133, 275, 333, 710, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(12, 223, { 190, 48, 114, 175, 323, 491, 103, 350, 173, 341, 129, 494, 0, 0, 0, 0 }),
    EggMoveList(15, 225, { 62, 98, 150, 229, 420, 252, 573, 194, 68, 694, 262, 248, 8, 196, 191, 0 }),
    EggMoveList(8, 228, { 83, 68, 180, 179, 422, 364, 389, 194, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(13, 231, { 116, 34, 246, 173, 68, 90, 283, 420, 457, 189, 484, 583, 667, 0, 0, 0 }),
    EggMoveList(8, 236, { 229, 136, 183, 170, 68, 410, 418, 364, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(3, 238, { 252, 273, 272, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(6, 239, { 238, 223, 359, 364, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(11, 240, { 562, 223, 183, 238, 5, 103, 9, 394, 187, 116, 384, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 241, { 562, 174, 38, 359, 217, 69, 179, 270, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(9, 246, { 23, 246, 174, 200, 116, 349, 334, 372, 442, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(12, 252, { 71, 163, 400, 24, 225, 73, 388, 235, 242, 306, 345, 580, 0, 0, 0, 0 }),
    EggMoveList(10, 255, { 64, 306, 174, 68, 364, 387, 400, 226, 97, 67, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(13, 258, { 189, 44, 246, 68, 174, 38, 243, 124, 23, 469, 281, 253, 250, 0, 0, 0 }),
    EggMoveList(7, 261, { 310, 305, 343, 43, 423, 424, 422, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(7, 263, { 321, 493, 245, 204, 271, 270, 189, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(7, 270, { 235, 75, 230, 73, 321, 68, 298, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(10, 273, { 73, 98, 36, 388, 400, 133, 384, 492, 251, 580, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(7, 278, { 239, 16, 392, 282, 487, 469, 314, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(11, 280, { 50, 212, 262, 194, 288, 425, 109, 282, 227, 581, 502, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 283, { 341, 60, 56, 170, 450, 565, 471, 679, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(5, 290, { 16, 400, 450, 515, 175, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(11, 300, { 270, 253, 313, 273, 226, 321, 387, 428, 389, 493, 322, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 302, { 105, 260, 364, 389, 368, 236, 271, 286, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(12, 303, { 246, 321, 21, 69, 612, 305, 423, 424, 422, 385, 368, 581, 0, 0, 0, 0 }),
    EggMoveList(10, 304, { 174, 407, 283, 457, 23, 189, 34, 103, 276, 179, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(11, 307, { 7, 9, 8, 252, 226, 223, 384, 385, 427, 418, 501, 0, 0, 0, 0, 0 }),
    EggMoveList(4, 313, { 226, 271, 69, 679, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(5, 314, { 226, 74, 313, 109, 312, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(11, 315, { 178, 79, 75, 326, 791, 191, 42, 170, 437, 402, 438, 0, 0, 0, 0, 0 }),
    EggMoveList(6, 318, { 246, 194, 38, 37, 56, 129, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(14, 322, { 336, 184, 34, 205, 111, 23, 246, 257, 254, 256, 255, 442, 74, 484, 0, 0 }),
    EggMoveList(6, 324, { 281, 90, 130, 175, 246, 276, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 328, { 98, 16, 175, 210, 450, 364, 116, 341, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(14, 331, { 51, 298, 223, 68, 67, 345, 402, 50, 335, 388, 415, 565, 562, 612, 0, 0 }),
    EggMoveList(9, 333, { 407, 297, 114, 366, 310, 97, 384, 304, 583, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(13, 335, { 175, 24, 68, 174, 154, 400, 232, 458, 50, 515, 364, 501, 187, 0, 0, 0 }),
    EggMoveList(8, 336, { 254, 256, 255, 34, 184, 372, 415, 515, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(9, 339, { 37, 209, 175, 36, 250, 56, 349, 414, 341, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(9, 341, { 246, 232, 38, 453, 415, 163, 34, 276, 349, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(5, 352, { 277, 271, 252, 105, 612, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(6, 353, { 50, 194, 310, 286, 109, 441, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(6, 355, { 220, 288, 262, 114, 286, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(6, 357, { 29, 21, 73, 267, 174, 348, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(12, 359, { 174, 38, 277, 212, 44, 364, 226, 428, 372, 224, 506, 583, 0, 0, 0, 0 }),
    EggMoveList(9, 361, { 335, 50, 415, 205, 556, 191, 311, 506, 313, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 363, { 174, 90, 254, 256, 255, 281, 392, 187, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(5, 366, { 34, 48, 109, 392, 330, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(7, 371, { 111, 407, 37, 239, 56, 349, 424, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(16, 387, { 388, 321, 34, 38, 328, 402, 37, 133, 276, 254, 256, 255, 414, 469, 580, 484 }),
    EggMoveList(12, 390, { 7, 9, 24, 227, 257, 116, 270, 252, 299, 68, 501, 66, 0, 0, 0, 0 }),
    EggMoveList(10, 393, { 458, 48, 281, 189, 173, 175, 97, 392, 297, 196, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(5, 399, { 98, 38, 154, 401, 130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(13, 403, { 608, 36, 24, 336, 400, 98, 423, 424, 422, 129, 270, 598, 313, 0, 0, 0 }),
    EggMoveList(11, 406, { 235, 178, 79, 75, 326, 791, 191, 42, 170, 437, 402, 0, 0, 0, 0, 0 }),
    EggMoveList(10, 417, { 343, 44, 313, 111, 205, 260, 175, 39, 266, 268, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(10, 418, { 189, 29, 154, 163, 210, 226, 392, 415, 487, 270, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(13, 420, { 75, 230, 321, 267, 312, 505, 361, 111, 579, 205, 311, 402, 580, 0, 0, 0 }),
    EggMoveList(13, 422, { 68, 243, 254, 256, 255, 281, 174, 124, 499, 54, 151, 133, 90, 0, 0, 0 }),
    EggMoveList(16, 427, { 509, 383, 458, 252, 175, 186, 415, 298, 313, 227, 67, 9, 8, 7, 322, 612 }),
    EggMoveList(8, 431, { 44, 39, 98, 28, 313, 372, 175, 387, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 433, { 50, 174, 95, 273, 248, 105, 500, 322, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(10, 434, { 43, 123, 38, 310, 114, 163, 242, 184, 492, 583, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(9, 443, { 37, 232, 38, 239, 200, 184, 34, 442, 341, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(10, 446, { 38, 174, 90, 68, 562, 204, 18, 428, 495, 120, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(7, 449, { 254, 256, 255, 174, 18, 34, 279, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(7, 451, { 28, 163, 109, 18, 103, 97, 342, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(10, 453, { 364, 223, 29, 410, 252, 238, 418, 68, 501, 367, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(8, 458, { 239, 114, 21, 243, 150, 366, 109, 133, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(7, 459, { 73, 74, 38, 23, 130, 345, 402, 0, 0, 0, 0, 0, 0, 0, 0, 0 }),
    EggMoveList(13, 422, { 68, 243, 254, 256, 255, 281, 174, 124, 499, 54, 151, 133, 90, 0, 0, 0 }),
    };

constexpr std::array<LevelInfo, 9> levelInfoList = {
    LevelInfo(16, 20),
    LevelInfo(25, 29),
    LevelInfo(29, 33),
    LevelInfo(33, 37),
    LevelInfo(36, 40),
    LevelInfo(39, 43),
    LevelInfo(42, 46),
    LevelInfo(50, 55),
    LevelInfo(58, 63)
    };

// clang-format on

static u32 rand(u32 prng)
{
    return (prng % 0xffffffff) + 0x80000000;
}

static u16 getItem(u8 rand, Lead lead, const PersonalInfo *info)
{
    constexpr u8 ItemTableRange[2] = { 50, 60 };

    u8 thresh1 = ItemTableRange[lead == Lead::CompoundEyes ? 1 : 0];
    constexpr u8 thresh2 = 20;

    if (rand >= thresh1)
    {
        if (rand >= (thresh1 + thresh2))
        {
            return info->getItem(2);
        }
        else
        {
            return info->getItem(1);
        }
    }
    else
    {
        return info->getItem(0);
    }
}

UndergroundGenerator::UndergroundGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, Lead lead, bool diglett, u8 levelFlag,
                                           const UndergroundArea &area, const Profile8 &profile, const UndergroundStateFilter &filter) :
    WildGenerator(initialAdvances, maxAdvances, offset, Method::None, lead, area, profile, filter), diglett(diglett), levelFlag(levelFlag)
{
}

std::vector<UndergroundState> UndergroundGenerator::generate(u64 seed0, u64 seed1) const
{
    RNGList<u32, Xorshift, 256> rngList(seed0, seed1, initialAdvances + offset);
    const PersonalInfo *base = PersonalLoader::getPersonal(profile.getVersion());
    const auto &levelInfo = levelInfoList[levelFlag];
    u8 pidRolls = diglett ? 2 : 1;

    auto createPokemon = [=, &rngList](u32 advances, u16 specie) {
        u8 level;
        if (lead == Lead::Pressure)
        {
            level = levelInfo.max;
        }
        else
        {
            u8 range = levelInfo.max - levelInfo.min + 1;
            level = levelInfo.min + (rngList.next() % range);
        }

        u32 ec = rngList.next(rand);
        u32 sidtid = rngList.next(rand);
        u32 pid;
        u8 shiny;
        for (u8 j = 0; j < pidRolls; j++)
        {
            pid = rngList.next(rand);

            shiny = Utilities::getShiny<false>(pid, (sidtid >> 16) ^ (sidtid & 0xffff));
            if (shiny) // Force shiny
            {
                if (Utilities::getShiny<false>(pid, tsv) != shiny)
                {
                    u16 high = (pid & 0xFFFF) ^ tsv ^ (2 - shiny);
                    pid = (high << 16) | (pid & 0xFFFF);
                }
                break;
            }
            else // Force non
            {
                if (Utilities::isShiny<false>(pid, tsv))
                {
                    pid ^= 0x10000000;
                }
            }
        }

        std::array<u8, 6> ivs;
        std::generate(ivs.begin(), ivs.end(), [&rngList] { return rngList.next(rand) % 32; });

        u8 ability = rngList.next(rand) % 2;

        const PersonalInfo *info = &base[specie];

        u8 gender;
        switch (info->getGender())
        {
        case 255:
            gender = 2;
            break;
        case 254:
            gender = 1;
            break;
        case 0:
            gender = 0;
            break;
        default:
            if ((lead == Lead::CuteCharmF || lead == Lead::CuteCharmM) && (rngList.next() % 100) < 67)
            {
                gender = lead == Lead::CuteCharmF ? 0 : 1;
            }
            else
            {
                gender = (rngList.next(rand) % 253) + 1 < info->getGender();
            }
        }

        u8 nature;
        if (lead <= Lead::SynchronizeEnd)
        {
            nature = toInt(lead);
        }
        else
        {
            nature = rngList.next(rand) % 25;
        }

        u8 height = rngList.next(rand) % 129;
        height += rngList.next(rand) % 128;

        u8 weight = rngList.next(rand) % 129;
        weight += rngList.next(rand) % 128;

        u16 item = getItem(rngList.next() % 100, lead, info);

        u16 eggMove = 0;
        auto eggMoves = std::lower_bound(eggMoveList.begin(), eggMoveList.end(), info->getHatchSpecie(),
                                         [](const EggMoveList &move, u16 specie) { return move.specie < specie; });
        if (eggMoves != eggMoveList.end() && eggMoves->specie == info->getHatchSpecie())
        {
            eggMove = eggMoves->moves[rngList.next() % eggMoves->count];
        }

        return UndergroundState(initialAdvances + advances, ec, pid, ivs, ability, gender, level, nature, shiny, height, weight, eggMove,
                                item, specie, info);
    };

    std::vector<UndergroundState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        u8 spawnCount = area.getMin();

        u16 specialPokemon = area.getSpecialPokemon(rngList);

        if ((rngList.next() % 100) >= 50)
        {
            spawnCount = area.getMax();
        }

        if (specialPokemon != 0)
        {
            spawnCount -= 1;
        }

        auto slots = area.getSlots(rngList, spawnCount);
        for (u8 i = 0; i < spawnCount; i++)
        {
            u16 pokemon = area.getPokemon(rngList, slots[i]);
            UndergroundState state = createPokemon(cnt, pokemon);
            if (filter.compareState(state))
            {
                states.emplace_back(state);
            }
        }

        if (specialPokemon != 0)
        {
            UndergroundState state = createPokemon(cnt, specialPokemon);
            if (filter.compareState(state))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
