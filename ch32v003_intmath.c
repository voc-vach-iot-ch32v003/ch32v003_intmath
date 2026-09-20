/**
 * @file ch32v003_intmath.c
 * @author Vọc Vạch IoT
 * @brief Thực thi các hàm toán số nguyên cho CH32V003.
 * @version 1.0.0
 * @date 2026-09-19
 */

#include "ch32v003_intmath.h"

#define INTMATH_CORDIC_STEPS 12

/**
 * @brief Bảng lookup giá trị Arctan(2^-i) tính bằng centi-độ (0.01 deg).
 */
static const int32_t cordicAnglesCenti[INTMATH_CORDIC_STEPS] = {
    4500, // atan(2^-0) = 45.00°
    2657, // atan(2^-1) = 26.57°
    1404, // atan(2^-2) = 14.04°
    713, // atan(2^-3) =  7.13°
    358, // atan(2^-4) =  3.58°
    179, // atan(2^-5) =  1.79°
    90, // atan(2^-6) =  0.90°
    45, // atan(2^-7) =  0.45°
    22, // atan(2^-8) =  0.22°
    11, // atan(2^-9) =  0.11°
    6, // atan(2^-10) = 0.06°
    3 // atan(2^-11) = 0.03°
};

/**
 * @brief Vòng lặp xoay vector CORDIC cốt lõi trong góc phần tư thứ 1 và 4.
 */
static int32_t intMathCordicCore(const int32_t y, const int32_t x)
{
    int32_t currentX = x;
    int32_t currentY = y;
    int32_t angle = 0;

    for (uint8_t i = 0; i < INTMATH_CORDIC_STEPS; i++)
    {
        int32_t nextX;
        int32_t nextY;

        if (currentY > 0)
        {
            nextX = currentX + (currentY >> i);
            nextY = currentY - (currentX >> i);
            angle += cordicAnglesCenti[i];
        }
        else
        {
            nextX = currentX - (currentY >> i);
            nextY = currentY + (currentX >> i);
            angle -= cordicAnglesCenti[i];
        }

        currentX = nextX;
        currentY = nextY;
    }

    return angle;
}

uint16_t intMathMagnitude2D(const int16_t a, const int16_t b)
{
    const uint16_t absA = (a < 0) ? -a : a;
    const uint16_t absB = (b < 0) ? -b : b;
    const uint16_t hi = (absA > absB) ? absA : absB;
    const uint16_t lo = (absA > absB) ? absB : absA;

    return (uint16_t)(hi - (hi >> 3) + (lo >> 1));
}

int32_t intMathClamp(const int32_t value, const int32_t min, const int32_t max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int32_t intMathAtan2Centi(const int32_t y, const int32_t x)
{
    if (x == 0 && y == 0)
    {
        return 0;
    }

    // Đưa vector về góc phần tư thích hợp trước khi gọi CORDIC
    if (x >= 0)
    {
        return intMathCordicCore(y, x);
    }
    else
    {
        if (y >= 0)
        {
            return 18000 + intMathCordicCore(y, x);
        }
        else
        {
            return -18000 + intMathCordicCore(y, x);
        }
    }
}

uint16_t intMathSqrt32(uint32_t value)
{
    if (value == 0)
        return 0;

    uint32_t res = 0;
    uint32_t bit =
        (uint32_t)1
        << 30; // Bit lớn nhất của uint32_t (phải là bình phương của 2^15)

    // Tìm bit bắt đầu phù hợp
    while (bit > value)
    {
        bit >>= 2;
    }

    while (bit != 0)
    {
        if (value >= res + bit)
        {
            value -= res + bit;
            res = (res >> 1) + bit;
        }
        else
        {
            res >>= 1;
        }
        bit >>= 2;
    }

    return (uint16_t)res;
}
