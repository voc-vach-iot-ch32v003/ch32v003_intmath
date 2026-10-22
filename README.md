# 🧮 ch32v003_intmath

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![PlatformIO](https://img.shields.io/badge/PlatformIO-Supported-orange.svg)
![Framework](https://img.shields.io/badge/Framework-ch32v003fun-green.svg)
![MCU](https://img.shields.io/badge/MCU-CH32V003-red.svg)

Thư viện toán số nguyên cho CH32V003 (RISC-V RV32EC, không có FPU và không có lệnh nhân/chia phần cứng): `atan2` bằng CORDIC, độ lớn vector 2D, căn bậc hai, trị tuyệt đối và số giả ngẫu nhiên — thay cho `<math.h>` để tiết kiệm Flash, RAM và thời gian chạy.

---

## 🚀 Tính năng nổi bật

- **`intMathAtan2Centi(y, x)`** — CORDIC thuần số nguyên, đủ 4 góc phần tư, kết quả centi-độ (0.01°) trong `[-18000, 18000]`, **sai số ≤ 0.02°** kể cả với đầu vào rất nhỏ.
- **`intMathMagnitude2D(a, b)`** — xấp xỉ √(a² + b²) chỉ bằng dịch bit và cộng, **sai số -3% … +1.2%**.
- **`intMathSqrt32(v)`** — ⌊√v⌋ chính xác tuyệt đối cho mọi `uint32_t`.
- **`intMathAbs(x)`** — trị tuyệt đối trả về kiểu không dấu (an toàn với `-32768`, `-128`...), tự chọn hàm theo kiểu bằng C11 `_Generic`.
- **`intMathXorshift(&state)`** — số giả ngẫu nhiên Xorshift 8/16/32-bit, tự chọn theo kiểu biến trạng thái.
- **`intMathClamp(v, min, max)`** — ép giá trị vào khoảng.

Hiệu năng đo trên chip thật (48MHz, bàn test HIL):

| Hàm | Chu kỳ máy / lần gọi | Thời gian |
| :-- | --: | --: |
| `intMathAtan2Centi` | 817 | ~17 µs |
| `intMathMagnitude2D` | 81 | ~1.7 µs |
| `intMathSqrt32` | 308 | ~6.4 µs |

---

## 📦 Cài đặt (Installation)

```ini
lib_deps =
    https://github.com/voc-vach-iot-ch32v003/ch32v003_intmath.git
```

---

## ⚙️ Cấu hình môi trường

Không cần build flag hay file cấu hình. Thư viện không phụ thuộc thư viện nào khác và cũng biên dịch được trên máy tính (gcc) để kiểm thử.

---

## 📝 Code mẫu sử dụng (Quick Start)

```c
#include "ch32fun.h"
#include "ch32v003_debug.h"
#include "ch32v003_delay.h"
#include "ch32v003_intmath.h"

static void setup(void)
{
    // Góc của vector (y = 100, x = -100) -> 13500 centi-độ = 135.00°
    const int32_t angle = intMathAtan2Centi(100, -100);
    printf("atan2 = %ld.%02lu deg\n", angle / 100, intMathAbs(angle) % 100);

    // Độ lớn vector (-300, 400) ~ 500
    printf("magnitude = %u\n", intMathMagnitude2D(-300, 400));

    // Căn bậc hai, ép khoảng
    printf("sqrt(65536) = %u, clamp = %ld\n", intMathSqrt32(65536), intMathClamp(300, 0, 255));

    // Trị tuyệt đối an toàn, số ngẫu nhiên 16-bit
    const int8_t value8 = -128;
    uint16_t seed = 0xACE1;
    printf("abs = %u, random = %u\n", intMathAbs(value8), intMathXorshift(&seed));
}

int main(void)
{
    SystemInit();
    delayMs(1000); // Khoảng chờ an toàn tránh brick chip

    setup();
    while (1)
    {
    }
}
```

---

## 📁 Danh sách Ví dụ mẫu (Examples)

Thư viện tiện ích: không có thư mục `examples/`, cách dùng được trình bày ở Quick Start.

---

## 📑 Tra cứu API (API Reference)

### Hàm toán học

| Hàm xử lý | Tham số | Giá trị trả về | Mô tả |
| :-- | :-- | :-- | :-- |
| `intMathAtan2Centi(y, x)` | `int32_t y`, `int32_t x` | `int32_t` | Góc atan2(y, x) theo centi-độ, `[-18000, 18000]`; trả 0 khi `x = y = 0`. |
| `intMathMagnitude2D(a, b)` | `int16_t a`, `int16_t b` | `uint16_t` | Xấp xỉ √(a² + b²), sai số -3% … +1.2%. |
| `intMathSqrt32(value)` | `uint32_t value` | `uint16_t` | ⌊√value⌋ chính xác. |
| `intMathClamp(value, min, max)` | `int32_t value`, `int32_t min`, `int32_t max` | `int32_t` | Ép `value` vào `[min, max]`. |

### Macro tự chọn kiểu (C11 `_Generic`)

| Macro | Tham số | Giá trị trả về | Mô tả |
| :-- | :-- | :-- | :-- |
| `intMathAbs(x)` | `int8_t` / `int16_t` / `int32_t` | `uint8_t` / `uint16_t` / `uint32_t` | Trị tuyệt đối kiểu không dấu, không tràn số với giá trị âm nhỏ nhất. |
| `intMathXorshift(statePtr)` | `uint8_t*` / `uint16_t*` / `uint32_t*` | cùng kiểu với trạng thái | Sinh số giả ngẫu nhiên kế tiếp và cập nhật trạng thái (trạng thái 0 tự thay bằng hạt giống mặc định). |

---

## 📄 Giấy phép (License) & Tác giả

- **Tác giả:** [Vọc Vạch IoT](https://github.com/voc-vach-iot)
- **Giấy phép:** Phát hành theo giấy phép [MIT License](LICENSE).
