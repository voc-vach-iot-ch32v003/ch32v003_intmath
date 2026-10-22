#include "ch32v003_intmath.h"

#define INTMATH_CORDIC_STEPS 14

/**
 * @brief Bảng lookup giá trị Arctan(2^-i) tính bằng milli-độ (0.001 deg) để giảm sai số làm tròn.
 */
static const int32_t cordicAnglesMilli[INTMATH_CORDIC_STEPS] = {
    45000, // atan(2^-0)  = 45.000°
    26565, // atan(2^-1)  = 26.565°
    14036, // atan(2^-2)  = 14.036°
    7125, // atan(2^-3)  =  7.125°
    3576, // atan(2^-4)  =  3.576°
    1790, // atan(2^-5)  =  1.790°
    895, // atan(2^-6)  =  0.895°
    448, // atan(2^-7)  =  0.448°
    224, // atan(2^-8)  =  0.224°
    112, // atan(2^-9)  =  0.112°
    56, // atan(2^-10) =  0.056°
    28, // atan(2^-11) =  0.028°
    14, // atan(2^-12) =  0.014°
    7 // atan(2^-13) =  0.007°
};

/**
 * @brief Vòng lặp xoay vector CORDIC cốt lõi, chỉ hợp lệ khi x > 0 (góc phần tư 1 và 4).
 * @return Góc tính bằng milli-độ trong khoảng (-90000, 90000).
 */
static int32_t intMathCordicCore(int32_t y, int32_t x)
{
    int32_t angle = 0;

    for (uint8_t i = 0; i < INTMATH_CORDIC_STEPS; i++)
    {
        const int32_t nextX = (y > 0) ? x + (y >> i) : x - (y >> i);

        if (y > 0)
        {
            y -= x >> i;
            angle += cordicAnglesMilli[i];
        }
        else
        {
            y += x >> i;
            angle -= cordicAnglesMilli[i];
        }
        x = nextX;
    }

    return angle;
}

uint16_t intMathMagnitude2D(const int16_t a, const int16_t b)
{
    const uint16_t absA = intMathAbs(a);
    const uint16_t absB = intMathAbs(b);
    const uint16_t hi = (absA > absB) ? absA : absB;
    const uint16_t lo = (absA > absB) ? absB : absA;

    // max(hi, 7/8*hi + 1/2*lo): chặn sai số âm khi vector gần trục, sai số -3% .. +1.2%
    const uint16_t approx = (uint16_t)(hi - (hi >> 3) + (lo >> 1));
    return (approx > hi) ? approx : hi;
}

int32_t intMathClamp(const int32_t value, const int32_t min, const int32_t max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int32_t intMathAtan2Centi(int32_t y, int32_t x)
{
    if (x == 0 && y == 0)
    {
        return 0;
    }

    // 1. Lật vector sang nửa phải (x > 0), bù lại ±180° ở cuối
    int32_t offsetCenti = 0;
    if (x < 0)
    {
        offsetCenti = (y >= 0) ? 18000 : -18000;
        x = -x;
        y = -y;
    }

    // 2. Chuẩn hóa độ lớn về [2^20, 2^28): đủ bit cho phép dịch, không tràn khi nhân hệ số CORDIC ~1.647
    uint32_t magnitude = (uint32_t)x | intMathAbs(y);
    while (magnitude < (1UL << 20))
    {
        x <<= 1;
        y <<= 1;
        magnitude <<= 1;
    }
    while (magnitude >= (1UL << 28))
    {
        x >>= 1;
        y >>= 1;
        magnitude >>= 1;
    }

    // 3. Quy đổi milli-độ -> centi-độ có làm tròn
    const int32_t angleMilli = intMathCordicCore(y, x);
    const int32_t angleCenti = (angleMilli >= 0) ? (angleMilli + 5) / 10 : -((-angleMilli + 5) / 10);

    return offsetCenti + angleCenti;
}

uint16_t intMathSqrt32(uint32_t value)
{
    uint32_t result = 0;
    uint32_t bit = 1UL << 30; // Lũy thừa 4 lớn nhất của uint32_t

    // Tìm bit bắt đầu phù hợp
    while (bit > value)
    {
        bit >>= 2;
    }

    while (bit != 0)
    {
        if (value >= result + bit)
        {
            value -= result + bit;
            result = (result >> 1) + bit;
        }
        else
        {
            result >>= 1;
        }
        bit >>= 2;
    }

    return (uint16_t)result;
}
