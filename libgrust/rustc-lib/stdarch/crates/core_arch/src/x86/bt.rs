#[cfg(test)]
use stdarch_test::assert_instr;

/// Returns the bit in position `b` of the memory addressed by `p`.
#[inline]
#[cfg_attr(test, assert_instr(bt))]
#[unstable(feature = "simd_x86_bittest", issue = "59414")]
pub unsafe fn _bittest(p: *const i32, b: i32) -> u8 {
    let r: u8;
    llvm_asm!("btl $2, $1\n\tsetc ${0:b}"
              : "=r"(r)
              : "*m"(p), "r"(b)
              : "cc", "memory");
    r
}

/// Returns the bit in position `b` of the memory addressed by `p`, then sets the bit to `1`.
#[inline]
#[cfg_attr(test, assert_instr(bts))]
#[unstable(feature = "simd_x86_bittest", issue = "59414")]
pub unsafe fn _bittestandset(p: *mut i32, b: i32) -> u8 {
    let r: u8;
    llvm_asm!("btsl $2, $1\n\tsetc ${0:b}"
              : "=r"(r), "+*m"(p)
              : "r"(b)
              : "cc", "memory");
    r
}

/// Returns the bit in position `b` of the memory addressed by `p`, then resets that bit to `0`.
#[inline]
#[cfg_attr(test, assert_instr(btr))]
#[unstable(feature = "simd_x86_bittest", issue = "59414")]
pub unsafe fn _bittestandreset(p: *mut i32, b: i32) -> u8 {
    let r: u8;
    llvm_asm!("btrl $2, $1\n\tsetc ${0:b}"
              : "=r"(r), "+*m"(p)
              : "r"(b)
              : "cc", "memory");
    r
}

/// Returns the bit in position `b` of the memory addressed by `p`, then inverts that bit.
#[inline]
#[cfg_attr(test, assert_instr(btc))]
#[unstable(feature = "simd_x86_bittest", issue = "59414")]
pub unsafe fn _bittestandcomplement(p: *mut i32, b: i32) -> u8 {
    let r: u8;
    llvm_asm!("btcl $2, $1\n\tsetc ${0:b}"
              : "=r"(r), "+*m"(p)
              : "r"(b)
              : "cc", "memory");
    r
}

#[cfg(test)]
mod tests {
    use crate::core_arch::x86::*;

    #[test]
    fn test_bittest() {
        unsafe {
            let a = 0b0101_0000i32;
            assert_eq!(_bittest(&a as _, 4), 1);
            assert_eq!(_bittest(&a as _, 5), 0);
        }
    }

    #[test]
    fn test_bittestandset() {
        unsafe {
            let mut a = 0b0101_0000i32;
            assert_eq!(_bittestandset(&mut a as _, 4), 1);
            assert_eq!(_bittestandset(&mut a as _, 4), 1);
            assert_eq!(_bittestandset(&mut a as _, 5), 0);
            assert_eq!(_bittestandset(&mut a as _, 5), 1);
        }
    }

    #[test]
    fn test_bittestandreset() {
        unsafe {
            let mut a = 0b0101_0000i32;
            assert_eq!(_bittestandreset(&mut a as _, 4), 1);
            assert_eq!(_bittestandreset(&mut a as _, 4), 0);
            assert_eq!(_bittestandreset(&mut a as _, 5), 0);
            assert_eq!(_bittestandreset(&mut a as _, 5), 0);
        }
    }

    #[test]
    fn test_bittestandcomplement() {
        unsafe {
            let mut a = 0b0101_0000i32;
            assert_eq!(_bittestandcomplement(&mut a as _, 4), 1);
            assert_eq!(_bittestandcomplement(&mut a as _, 4), 0);
            assert_eq!(_bittestandcomplement(&mut a as _, 4), 1);
            assert_eq!(_bittestandcomplement(&mut a as _, 5), 0);
            assert_eq!(_bittestandcomplement(&mut a as _, 5), 1);
        }
    }
}
