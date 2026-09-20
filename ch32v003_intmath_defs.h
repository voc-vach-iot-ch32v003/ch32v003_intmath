/**
 * @file ch32v003_intmath_defs.h
 * @author Vọc Vạch IoT
 * @brief Định nghĩa các macro và hàm hỗ trợ cho thư viện toán học số nguyên.
 * @version 1.0.0
 * @date 2026-09-19
 */

#ifndef CH32V003_INTMATH_DEFS_H
#define CH32V003_INTMATH_DEFS_H

#include <stdint.h>

// =========================================================================
// [MỤC 1] BẢNG MASTER KIỂU DỮ LIỆU (Nhận Macro xử lý làm tham số 'M')
// =========================================================================
#define MATH_SIGNED_TYPES(M) \
    M(int8_t,   uint8_t,  I8)  \
    M(int16_t,  uint16_t, I16) \
    M(int32_t,  uint32_t, I32)

#define MATH_UNSIGNED_TYPES(M) \
    M(uint8_t,  8)   \
    M(uint16_t, 16)  \
    M(uint32_t, 32)


// =========================================================================
// [MỤC 2] KHỐI HÀM TRỊ TUYỆT ĐỐI (ABS) - Chống tràn số bằng Unsigned
// =========================================================================

// 1. Tạo macro phụ có tên riêng biệt để sinh hàm inline
#define GENERATE_ABS_FUNC(signed_t, unsigned_t, suffix) \
    static inline unsigned_t intMathAbs##suffix(signed_t v) { \
        return (unsigned_t)((v < 0) ? -(int32_t)v : (int32_t)v); \
    }
MATH_SIGNED_TYPES(GENERATE_ABS_FUNC) // Gọi bảng master sinh hàm
#undef GENERATE_ABS_FUNC

// 2. Tạo macro phụ có tên riêng biệt để sinh dòng map trong _Generic
#define GENERATE_ABS_GENERIC(signed_t, unsigned_t, suffix) signed_t: intMathAbs##suffix,

// 3. Macro tổng gọi bảng master kèm tham số xử lý cụ thể
#define intMathAbs(X_PARAM) _Generic((X_PARAM), \
    int: intMathAbsI32, \
    MATH_SIGNED_TYPES(GENERATE_ABS_GENERIC)   \
    default: intMathAbsI32 \
)(X_PARAM)


// =========================================================================
// [MỤC 3] KHỐI HÀM GIẢ NGẪU NHIÊN (XORSHIFT)
// =========================================================================
static inline uint8_t intMathXorshift8(uint8_t* state)
{
    uint8_t s = (*state != 0u) ? *state : 0x9Du;
    s ^= (uint8_t)(s << 7);
    s ^= (uint8_t)(s >> 5);
    s ^= (uint8_t)(s << 3);
    return *state = s;
}

static inline uint16_t intMathXorshift16(uint16_t* state)
{
    uint16_t s = (*state != 0u) ? *state : 0xACE1u;
    s ^= (uint16_t)(s << 7);
    s ^= (uint16_t)(s >> 9);
    s ^= (uint16_t)(s << 8);
    return *state = s;
}

static inline uint32_t intMathXorshift32(uint32_t* state)
{
    uint32_t s = (*state != 0u) ? *state : 2463534242u;
    s ^= s << 13;
    s ^= s >> 17;
    s ^= s << 5;
    return *state = s;
}

// Macro phụ sinh dòng map trong _Generic cho Xorshift
#define GENERATE_XOR_GENERIC(type, suffix) type*: intMathXorshift##suffix,

#define intMathXorshift(X_PARAM) _Generic((X_PARAM), \
    MATH_UNSIGNED_TYPES(GENERATE_XOR_GENERIC) \
    default: intMathXorshift32 \
)(X_PARAM)

#endif /* CH32V003_INTMATH_DEFS_H */
