// ignore-tidy-linelength
#![allow(unused)]
#![allow(const_err)] // make sure we cannot allow away the errors tested here

use std::mem;

// normalize-stderr-test "offset \d+" -> "offset N"
// normalize-stderr-test "alloc\d+" -> "allocN"
// normalize-stderr-test "size \d+" -> "size N"

#[repr(C)]
union MaybeUninit<T: Copy> {
    uninit: (),
    init: T,
}

trait Trait {}
impl Trait for bool {}

// custom unsized type
struct MyStr(str);

// custom unsized type with sized fields
struct MySlice<T: ?Sized>(bool, T);
type MySliceBool = MySlice<[bool]>;

// # str
// OK
const STR_VALID: &str = unsafe { mem::transmute((&42u8, 1usize)) };
// bad str
const STR_TOO_LONG: &str = unsafe { mem::transmute((&42u8, 999usize)) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
const NESTED_STR_MUCH_TOO_LONG: (&str,) = (unsafe { mem::transmute((&42, usize::MAX)) },);
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// bad str
const STR_LENGTH_PTR: &str = unsafe { mem::transmute((&42u8, &3)) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// bad str in user-defined unsized type
const MY_STR_LENGTH_PTR: &MyStr = unsafe { mem::transmute((&42u8, &3)) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
const MY_STR_MUCH_TOO_LONG: &MyStr = unsafe { mem::transmute((&42u8, usize::MAX)) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }

// uninitialized byte
const STR_NO_INIT: &str = unsafe { mem::transmute::<&[_], _>(&[MaybeUninit::<u8> { uninit: () }]) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// uninitialized byte in user-defined str-like
const MYSTR_NO_INIT: &MyStr = unsafe { mem::transmute::<&[_], _>(&[MaybeUninit::<u8> { uninit: () }]) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }

// # slice
// OK
const SLICE_VALID: &[u8] = unsafe { mem::transmute((&42u8, 1usize)) };
// bad slice: length uninit
const SLICE_LENGTH_UNINIT: &[u8] = unsafe {
// { dg-error ".E0080." "" { target *-*-* } .-1 }
    let uninit_len = MaybeUninit::<usize> { uninit: () };
    mem::transmute((42, uninit_len))
};
// bad slice: length too big
const SLICE_TOO_LONG: &[u8] = unsafe { mem::transmute((&42u8, 999usize)) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// bad slice: length not an int
const SLICE_LENGTH_PTR: &[u8] = unsafe { mem::transmute((&42u8, &3)) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// bad slice box: length too big
const SLICE_TOO_LONG_BOX: Box<[u8]> = unsafe { mem::transmute((&42u8, 999usize)) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// bad slice box: length not an int
const SLICE_LENGTH_PTR_BOX: Box<[u8]> = unsafe { mem::transmute((&42u8, &3)) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }

// bad data *inside* the slice
const SLICE_CONTENT_INVALID: &[bool] = &[unsafe { mem::transmute(3u8) }];
// { dg-error ".E0080." "" { target *-*-* } .-1 }

// good MySliceBool
const MYSLICE_GOOD: &MySliceBool = &MySlice(true, [false]);
// bad: sized field is not okay
const MYSLICE_PREFIX_BAD: &MySliceBool = &MySlice(unsafe { mem::transmute(3u8) }, [false]);
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// bad: unsized part is not okay
const MYSLICE_SUFFIX_BAD: &MySliceBool = &MySlice(true, [unsafe { mem::transmute(3u8) }]);
// { dg-error ".E0080." "" { target *-*-* } .-1 }

// # raw slice
const RAW_SLICE_VALID: *const [u8] = unsafe { mem::transmute((&42u8, 1usize)) }; // ok
const RAW_SLICE_TOO_LONG: *const [u8] = unsafe { mem::transmute((&42u8, 999usize)) }; // ok because raw
const RAW_SLICE_MUCH_TOO_LONG: *const [u8] = unsafe { mem::transmute((&42u8, usize::MAX)) }; // ok because raw
const RAW_SLICE_LENGTH_UNINIT: *const [u8] = unsafe {
// { dg-error ".E0080." "" { target *-*-* } .-1 }
    let uninit_len = MaybeUninit::<usize> { uninit: () };
    mem::transmute((42, uninit_len))
};

// # trait object
// bad trait object
const TRAIT_OBJ_SHORT_VTABLE_1: &dyn Trait = unsafe { mem::transmute((&92u8, &3u8)) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// bad trait object
const TRAIT_OBJ_SHORT_VTABLE_2: &dyn Trait = unsafe { mem::transmute((&92u8, &3u64)) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
// bad trait object
const TRAIT_OBJ_INT_VTABLE: &dyn Trait = unsafe { mem::transmute((&92u8, 4usize)) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
const TRAIT_OBJ_UNALIGNED_VTABLE: &dyn Trait = unsafe { mem::transmute((&92u8, &[0u8; 128])) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
const TRAIT_OBJ_BAD_DROP_FN_NULL: &dyn Trait = unsafe { mem::transmute((&92u8, &[0usize; 8])) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
const TRAIT_OBJ_BAD_DROP_FN_INT: &dyn Trait = unsafe { mem::transmute((&92u8, &[1usize; 8])) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
const TRAIT_OBJ_BAD_DROP_FN_NOT_FN_PTR: &dyn Trait = unsafe { mem::transmute((&92u8, &[&42u8; 8])) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }

// bad data *inside* the trait object
const TRAIT_OBJ_CONTENT_INVALID: &dyn Trait = unsafe { mem::transmute::<_, &bool>(&3u8) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }

// # raw trait object
const RAW_TRAIT_OBJ_VTABLE_NULL: *const dyn Trait = unsafe { mem::transmute((&92u8, 0usize)) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
const RAW_TRAIT_OBJ_VTABLE_INVALID: *const dyn Trait = unsafe { mem::transmute((&92u8, &3u64)) };
// { dg-error ".E0080." "" { target *-*-* } .-1 }
const RAW_TRAIT_OBJ_CONTENT_INVALID: *const dyn Trait = unsafe { mem::transmute::<_, &bool>(&3u8) } as *const dyn Trait; // ok because raw

// Const eval fails for these, so they need to be statics to error.
static mut RAW_TRAIT_OBJ_VTABLE_NULL_THROUGH_REF: *const dyn Trait = unsafe {
    mem::transmute::<_, &dyn Trait>((&92u8, 0usize))
// { dg-error ".E0080." "" { target *-*-* } .-1 }
};
static mut RAW_TRAIT_OBJ_VTABLE_INVALID_THROUGH_REF: *const dyn Trait = unsafe {
    mem::transmute::<_, &dyn Trait>((&92u8, &3u64))
// { dg-error ".E0080." "" { target *-*-* } .-1 }
};

fn main() {}

