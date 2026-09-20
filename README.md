# 🧮 ch32v003_intmath

Thư viện toán số nguyên tốc độ cao (Fast Integer Math Library) được tối ưu hóa đặc biệt cho dòng vi điều khiển RISC-V **CH32V003** không có bộ xử lý số thực (FPU). Thư viện hoạt động trên nền framework **ch32v003fun**, cung cấp các thuật toán CORDIC, xấp xỉ Vector 2D, căn bậc hai nguyên và hàm giả ngẫu nhiên giúp thay thế thư viện `<math.h>` tiêu chuẩn nhằm tiết kiệm tối đa dung lượng Flash và RAM.

## 🚀 Tính năng nổi bật

- **Thuật toán CORDIC Atan2 (`intMathAtan2Centi`):** Tính toán góc $\arctan(y, x)$ chính xác bằng thuật toán CORDIC thuần số nguyên, trả về kết quả theo đơn vị **centi-độ (0.01°)** trong khoảng $[-18000, +18000]$ (tương ứng $-180.00^\circ \rightarrow +180.00^\circ$).

- **Xấp xỉ độ lớn Vector 2D (`intMathMagnitude2D`):** Áp dụng thuật toán Alpha-Max-Beta-Min giúp tính $\sqrt{a^2 + b^2}$ chỉ bằng phép dịch bit và phép cộng/trừ. Không sử dụng phép nhân/chia hay căn bậc hai, với sai số tối đa $\approx 3.9\%$.

- **Căn bậc hai số nguyên 32-bit (`intMathSqrt32`):** Tính toán $\lfloor\sqrt{X}\rfloor$ hiệu năng cao bằng thuật toán dịch bit Binary Search / Babylonian.

- **Trị tuyệt đối an toàn với C11 `_Generic` (`intMathAbs`):** Tự động chuyển đổi hàm phù hợp theo kiểu dữ liệu (`int8_t`, `int16_t`, `int32_t`), hỗ trợ chuyển đổi sang Unsigned để tránh tràn số (Integer Overflow).

- **Sinh số giả ngẫu nhiên siêu nhẹ (`intMathXorshift`):** Sử dụng thuật toán Xorshift PRNG tối ưu cực kỳ ít tốn tài nguyên CPU, tự động nhận diện biến trạng thái 8-bit, 16-bit hoặc 32-bit qua C11 `_Generic`.

- **Hàm ép khoảng tiện lợi (`intMathClamp`):** Giới hạn nhanh giá trị số nguyên trong khoảng $[min, max]$.

## 📑 Cấu trúc hàm (API Reference)

### 1. API Toán học cơ bản & CORDIC

|

| **Hàm xử lý** | **Tham số** | **Giá trị trả về** | **Mô tả** |
| **`intMathAtan2Centi(y, x)`** | `int32_t y`, `int32_t x` | `int32_t` | Tính góc $\arctan(y/x)$ bằng CORDIC thuần số nguyên. Trả về centi-độ ($-18000 \rightarrow +18000$). |
| **`intMathMagnitude2D(a, b)`** | `int16_t a`, `int16_t b` | `uint16_t` | Tính xấp xỉ độ lớn vector $\sqrt{a^2 + b^2}$ bằng thuật toán Alpha-Max-Beta-Min (Sai số $\le 3.9\%$). |
| **`intMathSqrt32(value)`** | `uint32_t value` | `uint16_t` | Tính căn bậc hai số nguyên không âm 32-bit $\lfloor\sqrt{V}\rfloor$ làm tròn xuống. |
| **`intMathClamp(value, min, max)`** | `int32_t value`, `int32_t min`, `int32_t max` | `int32_t` | Giới hạn giá trị `value` nằm trong khoảng $[min, max]$. |

### 2. API Generic Macros (C11 `_Generic`)

| **Generic Macro** | **Kiểu dữ liệu hỗ trợ** | **Giá trị trả về** | **Mô tả** |
| **`intMathAbs(X)`** | `int8_t`, `int16_t`, `int32_t` | `uint8_t`, `uint16_t`, `uint32_t` | Trả về trị tuyệt đối an toàn kiểu Unsigned tương ứng để chống tràn số. |
| **`intMathXorshift(state_ptr)`** | `uint8_t*`, `uint16_t*`, `uint32_t*` | `uint8_t`, `uint16_t`, `uint32_t` | Sinh số giả ngẫu nhiên tiếp theo và tự động cập nhật giá trị biến trạng thái qua con trỏ. |

## ⚙️ Cấu hình môi trường (`platformio.ini`)

Thêm thư viện vào dự án PlatformIO bằng cách khai báo trong file `platformio.ini` hoặc đặt thư mục mã nguồn vào `lib/`:

```
[env:ch32v003]
platform = ch32v
board = ch32v003f4p6
framework = ch32v003fun
lib_deps =
    ch32v003_intmath
build_flags =
    -I .

```

## 📝 Code mẫu sử dụng

```
#include "ch32fun.h"
#include <ch32v003_intmath.h>
#include <stdio.h>

void setup() {
    // 1. Tính góc Atan2 theo đơn vị centi-độ (0.01 deg)
    int32_t y = 100;
    int32_t x = 100;
    int32_t angle = intMathAtan2Centi(y, x);
    // Angle = 4500 (Tương ứng 45.00°)
    printf("Atan2(%ld, %ld) = %ld.%02ld deg\n", y, x, angle / 100, intMathAbs(angle) % 100);

    // 2. Tính xấp xỉ độ lớn vector 2D (Alpha-Max-Beta-Min)
    int16_t vx = -300;
    int16_t vy = 400;
    uint16_t mag = intMathMagnitude2D(vx, vy);
    // Magnitude ~ 500
    printf("Magnitude 2D (-300, 400) = %u\n", mag);

    // 3. Tính căn bậc hai số nguyên
    uint32_t val = 65536;
    uint16_t root = intMathSqrt32(val);
    // Root = 256
    printf("Sqrt32(%lu) = %u\n", val, root);

    // 4. Giới hạn giá trị Clamp
    int32_t pwm = intMathClamp(300, 0, 255);
    // PWM = 255
    printf("Clamp PWM = %ld\n", pwm);

    // 5. Trị tuyệt đối an toàn chống tràn số với C11 _Generic
    int8_t val8 = -128;
    uint8_t abs8 = intMathAbs(val8); // Tự động gọi intMathAbsI8
    printf("Abs(-128) = %u\n", abs8);

    // 6. Sinh số giả ngẫu nhiên Xorshift
    uint16_t seed16 = 0xACE1;
    uint16_t random_val = intMathXorshift(&seed16); // Tự động gọi intMathXorshift16
    printf("Random 16-bit: %u\n", random_val);
}

int main() {
    SystemInit();
    setup();
    while (1) {
        // Main loop
    }
}

```
