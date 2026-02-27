// { dg-do run }
// { dg-options "-O2" }

/*
 * Note on test design:
 * This test uses a union-based type punning approach rather than standard
 * integer literals (e.g., 0x..._u128) or bitwise shifts for 64-bit and 128-bit types.
 * We do this for a few practical reasons:
 *  gccrs doesn't support 128-bit integer literals yet.
 *  Doing manual bitwise shifts on 32-bit targets currently triggers ICEs in the frontend.
 *  Casting via raw pointers gets optimized away by GCC under -O2 due to strict aliasing rules.
 * Using a #[repr(C)] union gives us a safe workaround.
 *  As a bonus, by asserting against the physical memory layout ([u8; N]) instead
 * of the logical integer value, this test is inherently endian-agnostic.
 */

#![feature(intrinsics, lang_items, no_core)]
#![no_core]

#[lang = "sized"]
pub trait Sized {}

#[lang = "copy"]
pub trait Copy {}

extern "rust-intrinsic" {
    pub fn bswap<T>(x: T) -> T;
    pub fn abort() -> !;
}

#[repr(C)]
union U64Pun {
    bytes: [u8; 8],
    val: u64,
}
#[repr(C)]
union I64Pun {
    bytes: [u8; 8],
    val: i64,
}
#[repr(C)]
union U128Pun {
    bytes: [u8; 16],
    val: u128,
}
#[repr(C)]
union I128Pun {
    bytes: [u8; 16],
    val: i128,
}

fn test_bswap_16_and_32() {
    if bswap(0x0102_u16) != 0x0201_u16 {
        abort();
    }
    if bswap(0x0102_i16) != 0x0201_i16 {
        abort();
    }

    if bswap(0x01020304_u32) != 0x04030201_u32 {
        abort();
    }
    if bswap(0x01020304_i32) != 0x04030201_i32 {
        abort();
    }
}

fn test_bswap_u64() {
    let in_pun = U64Pun {
        bytes: [1, 2, 3, 4, 5, 6, 7, 8],
    };
    unsafe {
        let out_pun = U64Pun {
            val: bswap(in_pun.val),
        };
        if out_pun.bytes[0] != 8 {
            abort();
        }
        if out_pun.bytes[1] != 7 {
            abort();
        }
        if out_pun.bytes[2] != 6 {
            abort();
        }
        if out_pun.bytes[3] != 5 {
            abort();
        }
        if out_pun.bytes[4] != 4 {
            abort();
        }
        if out_pun.bytes[5] != 3 {
            abort();
        }
        if out_pun.bytes[6] != 2 {
            abort();
        }
        if out_pun.bytes[7] != 1 {
            abort();
        }
    }
}

fn test_bswap_i64() {
    let in_pun = I64Pun {
        bytes: [1, 2, 3, 4, 5, 6, 7, 8],
    };
    unsafe {
        let out_pun = I64Pun {
            val: bswap(in_pun.val),
        };
        if out_pun.bytes[0] != 8 {
            abort();
        }
        if out_pun.bytes[1] != 7 {
            abort();
        }
        if out_pun.bytes[2] != 6 {
            abort();
        }
        if out_pun.bytes[3] != 5 {
            abort();
        }
        if out_pun.bytes[4] != 4 {
            abort();
        }
        if out_pun.bytes[5] != 3 {
            abort();
        }
        if out_pun.bytes[6] != 2 {
            abort();
        }
        if out_pun.bytes[7] != 1 {
            abort();
        }
    }
}

fn test_bswap_u128() {
    let in_pun = U128Pun {
        bytes: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16],
    };
    unsafe {
        let out_pun = U128Pun {
            val: bswap(in_pun.val),
        };
        if out_pun.bytes[0] != 16 {
            abort();
        }
        if out_pun.bytes[1] != 15 {
            abort();
        }
        if out_pun.bytes[2] != 14 {
            abort();
        }
        if out_pun.bytes[3] != 13 {
            abort();
        }
        if out_pun.bytes[4] != 12 {
            abort();
        }
        if out_pun.bytes[5] != 11 {
            abort();
        }
        if out_pun.bytes[6] != 10 {
            abort();
        }
        if out_pun.bytes[7] != 9 {
            abort();
        }
        if out_pun.bytes[8] != 8 {
            abort();
        }
        if out_pun.bytes[9] != 7 {
            abort();
        }
        if out_pun.bytes[10] != 6 {
            abort();
        }
        if out_pun.bytes[11] != 5 {
            abort();
        }
        if out_pun.bytes[12] != 4 {
            abort();
        }
        if out_pun.bytes[13] != 3 {
            abort();
        }
        if out_pun.bytes[14] != 2 {
            abort();
        }
        if out_pun.bytes[15] != 1 {
            abort();
        }
    }
}

fn test_bswap_i128() {
    let in_pun = I128Pun {
        bytes: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16],
    };
    unsafe {
        let out_pun = I128Pun {
            val: bswap(in_pun.val),
        };
        if out_pun.bytes[0] != 16 {
            abort();
        }
        if out_pun.bytes[1] != 15 {
            abort();
        }
        if out_pun.bytes[2] != 14 {
            abort();
        }
        if out_pun.bytes[3] != 13 {
            abort();
        }
        if out_pun.bytes[4] != 12 {
            abort();
        }
        if out_pun.bytes[5] != 11 {
            abort();
        }
        if out_pun.bytes[6] != 10 {
            abort();
        }
        if out_pun.bytes[7] != 9 {
            abort();
        }
        if out_pun.bytes[8] != 8 {
            abort();
        }
        if out_pun.bytes[9] != 7 {
            abort();
        }
        if out_pun.bytes[10] != 6 {
            abort();
        }
        if out_pun.bytes[11] != 5 {
            abort();
        }
        if out_pun.bytes[12] != 4 {
            abort();
        }
        if out_pun.bytes[13] != 3 {
            abort();
        }
        if out_pun.bytes[14] != 2 {
            abort();
        }
        if out_pun.bytes[15] != 1 {
            abort();
        }
    }
}

fn main() -> i32 {
    test_bswap_16_and_32();
    test_bswap_u64();
    test_bswap_i64();
    test_bswap_u128();
    test_bswap_i128();
    0
}
