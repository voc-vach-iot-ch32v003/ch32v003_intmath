/**
 * @file ch32v003_intmath.h
 * @author Vọc Vạch IoT
 * @brief Thư viện các hàm tính toán số nguyên tốc độ cao tối ưu cho CH32V003.
 * @details Cung cấp các thuật toán CORDIC, xấp xỉ độ lớn vector 2D, căn bậc hai số nguyên
 *          giúp thay thế các thư viện math.h tiêu chuẩn để tiết kiệm Flash và RAM.
 */

#ifndef CH32V003_INTMATH_H
#define CH32V003_INTMATH_H

#include <stdint.h>
#include "ch32v003_intmath_defs.h"

/**
 * @brief Tính độ lớn Vector 2D |(a, b)| xấp xỉ theo thuật toán Alpha-Max-Beta-Min.
 * @details Công thức: max(hi, 0.875 * hi + 0.5 * lo) với hi = max(|a|, |b|), lo = min(|a|, |b|).
 *          Thực hiện hoàn toàn bằng phép dịch bit và cộng, không dùng phép nhân/chia hay căn bậc hai.
 * @param a Giá trị thành phần thứ nhất (int16_t).
 * @param b Giá trị thành phần thứ hai (int16_t).
 * @return uint16_t Độ lớn vector xấp xỉ (Sai số trong khoảng -3% .. +1.2%).
 */
uint16_t intMathMagnitude2D(int16_t a, int16_t b);

/**
 * @brief Tính góc Atan2(y, x) bằng thuật toán CORDIC thuần số nguyên.
 * @details Trả về kết quả theo đơn vị centi-độ (0.01 degree) để tránh dùng số thực float.
 *          Hợp lệ trên cả 4 góc phần tư, sai số tối đa ~0.02° (đã kiểm thử trên host).
 * @param y Giá trị trục tung Y (int32_t).
 * @param x Giá trị trục hoành X (int32_t).
 * @return int32_t Góc tính được trong khoảng -18000 đến +18000 (tương ứng -180.00° đến +180.00°).
 */
int32_t intMathAtan2Centi(int32_t y, int32_t x);

/**
 * @brief Tính căn bậc hai của một số nguyên không âm (Integer Square Root).
 * @details Sử dụng thuật toán căn bậc hai từng bit (digit-by-digit), không dùng thư viện float.
 * @param value Số nguyên cần tính căn bậc hai (uint32_t).
 * @return uint16_t Giá trị căn bậc hai đã làm tròn xuống.
 */
uint16_t intMathSqrt32(uint32_t value);

/**
 * @brief Giới hạn một giá trị số nguyên trong khoảng [min, max] (Clamp).
 * @param value Giá trị đầu vào.
 * @param min Giá trị ngưỡng dưới.
 * @param max Giá trị ngưỡng trên.
 * @return int32_t Giá trị sau khi đã ép vào khoảng [min, max].
 */
int32_t intMathClamp(int32_t value, int32_t min, int32_t max);

#endif // CH32V003_INTMATH_H
