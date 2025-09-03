use core::convert::TryFrom;
use core::num::{IntErrorKind, NonZeroI32, NonZeroI8, NonZeroU32, NonZeroU8};
use core::option::Option::{self, None, Some};
use std::mem::size_of;

#[test]
fn test_create_nonzero_instance() {
    let _a = unsafe { NonZeroU32::new_unchecked(21) };
}

#[test]
fn test_size_nonzero_in_option() {
    assert_eq!(size_of::<NonZeroU32>(), size_of::<Option<NonZeroU32>>());
    assert_eq!(size_of::<NonZeroI32>(), size_of::<Option<NonZeroI32>>());
}

#[test]
fn test_match_on_nonzero_option() {
    let a = Some(unsafe { NonZeroU32::new_unchecked(42) });
    match a {
        Some(val) => assert_eq!(val.get(), 42),
        None => panic!("unexpected None while matching on Some(NonZeroU32(_))"),
    }

    match unsafe { Some(NonZeroU32::new_unchecked(43)) } {
        Some(val) => assert_eq!(val.get(), 43),
        None => panic!("unexpected None while matching on Some(NonZeroU32(_))"),
    }
}

#[test]
fn test_match_option_empty_vec() {
    let a: Option<Vec<isize>> = Some(vec![]);
    match a {
        None => panic!("unexpected None while matching on Some(vec![])"),
        _ => {}
    }
}

#[test]
fn test_match_option_vec() {
    let a = Some(vec![1, 2, 3, 4]);
    match a {
        Some(v) => assert_eq!(v, [1, 2, 3, 4]),
        None => panic!("unexpected None while matching on Some(vec![1, 2, 3, 4])"),
    }
}

#[test]
fn test_match_option_rc() {
    use std::rc::Rc;

    let five = Rc::new(5);
    match Some(five) {
        Some(r) => assert_eq!(*r, 5),
        None => panic!("unexpected None while matching on Some(Rc::new(5))"),
    }
}

#[test]
fn test_match_option_arc() {
    use std::sync::Arc;

    let five = Arc::new(5);
    match Some(five) {
        Some(a) => assert_eq!(*a, 5),
        None => panic!("unexpected None while matching on Some(Arc::new(5))"),
    }
}

#[test]
fn test_match_option_empty_string() {
    let a = Some(String::new());
    match a {
        None => panic!("unexpected None while matching on Some(String::new())"),
        _ => {}
    }
}

#[test]
fn test_match_option_string() {
    let five = "Five".to_string();
    match Some(five) {
        Some(s) => assert_eq!(s, "Five"),
        None => panic!("unexpected None while matching on Some(String { ... })"),
    }
}

mod atom {
    use core::num::NonZeroU32;

    #[derive(PartialEq, Eq)]
    pub struct Atom {
        index: NonZeroU32, // private
    }
    pub const FOO_ATOM: Atom = Atom { index: unsafe { NonZeroU32::new_unchecked(7) } };
}

macro_rules! atom {
    ("foo") => {
        atom::FOO_ATOM
    };
}

#[test]
fn test_match_nonzero_const_pattern() {
    match atom!("foo") {
        // Using as a pattern is supported by the compiler:
        atom!("foo") => {}
        _ => panic!("Expected the const item as a pattern to match."),
    }
}

#[test]
fn test_from_nonzero() {
    let nz = NonZeroU32::new(1).unwrap();
    let num: u32 = nz.into();
    assert_eq!(num, 1u32);
}

#[test]
fn test_from_signed_nonzero() {
    let nz = NonZeroI32::new(1).unwrap();
    let num: i32 = nz.into();
    assert_eq!(num, 1i32);
}

#[test]
fn test_from_str() {
    assert_eq!("123".parse::<NonZeroU8>(), Ok(NonZeroU8::new(123).unwrap()));
    assert_eq!("0".parse::<NonZeroU8>().err().map(|e| e.kind().clone()), Some(IntErrorKind::Zero));
    assert_eq!(
        "-1".parse::<NonZeroU8>().err().map(|e| e.kind().clone()),
        Some(IntErrorKind::InvalidDigit)
    );
    assert_eq!(
        "-129".parse::<NonZeroI8>().err().map(|e| e.kind().clone()),
        Some(IntErrorKind::NegOverflow)
    );
    assert_eq!(
        "257".parse::<NonZeroU8>().err().map(|e| e.kind().clone()),
        Some(IntErrorKind::PosOverflow)
    );
}

#[test]
fn test_nonzero_bitor() {
    let nz_alt = NonZeroU8::new(0b1010_1010).unwrap();
    let nz_low = NonZeroU8::new(0b0000_1111).unwrap();

    let both_nz: NonZeroU8 = nz_alt | nz_low;
    assert_eq!(both_nz.get(), 0b1010_1111);

    let rhs_int: NonZeroU8 = nz_low | 0b1100_0000u8;
    assert_eq!(rhs_int.get(), 0b1100_1111);

    let rhs_zero: NonZeroU8 = nz_alt | 0u8;
    assert_eq!(rhs_zero.get(), 0b1010_1010);

    let lhs_int: NonZeroU8 = 0b0110_0110u8 | nz_alt;
    assert_eq!(lhs_int.get(), 0b1110_1110);

    let lhs_zero: NonZeroU8 = 0u8 | nz_low;
    assert_eq!(lhs_zero.get(), 0b0000_1111);
}

#[test]
fn test_nonzero_bitor_assign() {
    let mut target = NonZeroU8::new(0b1010_1010).unwrap();

    target |= NonZeroU8::new(0b0000_1111).unwrap();
    assert_eq!(target.get(), 0b1010_1111);

    target |= 0b0001_0000;
    assert_eq!(target.get(), 0b1011_1111);

    target |= 0;
    assert_eq!(target.get(), 0b1011_1111);
}

#[test]
fn test_nonzero_from_int_on_success() {
    assert_eq!(NonZeroU8::try_from(5), Ok(NonZeroU8::new(5).unwrap()));
    assert_eq!(NonZeroU32::try_from(5), Ok(NonZeroU32::new(5).unwrap()));

    assert_eq!(NonZeroI8::try_from(-5), Ok(NonZeroI8::new(-5).unwrap()));
    assert_eq!(NonZeroI32::try_from(-5), Ok(NonZeroI32::new(-5).unwrap()));
}

#[test]
fn test_nonzero_from_int_on_err() {
    assert!(NonZeroU8::try_from(0).is_err());
    assert!(NonZeroU32::try_from(0).is_err());

    assert!(NonZeroI8::try_from(0).is_err());
    assert!(NonZeroI32::try_from(0).is_err());
}

#[test]
fn nonzero_const() {
    // test that the methods of `NonZeroX>` are usable in a const context
    // Note: only tests NonZero8

    const NONZERO: NonZeroU8 = unsafe { NonZeroU8::new_unchecked(5) };

    const GET: u8 = NONZERO.get();
    assert_eq!(GET, 5);

    const ZERO: Option<NonZeroU8> = NonZeroU8::new(0);
    assert!(ZERO.is_none());

    const ONE: Option<NonZeroU8> = NonZeroU8::new(1);
    assert!(ONE.is_some());
}
