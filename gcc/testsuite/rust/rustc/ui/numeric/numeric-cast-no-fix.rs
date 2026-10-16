#[allow(unused_must_use)]
fn main() {
    let x_usize: usize = 1;
    let x_u128: u128 = 2;
    let x_u64: u64 = 3;
    let x_u32: u32 = 4;
    let x_u16: u16 = 5;
    let x_u8: u8 = 6;

    x_usize > -1_isize;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u128 > -1_isize;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u64 > -1_isize;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u32 > -1_isize;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u16 > -1_isize;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u8 > -1_isize;
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    x_usize > -1_i128;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u128 > -1_i128;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u64 > -1_i128;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u32 > -1_i128;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u16 > -1_i128;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u8 > -1_i128;
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    x_usize > -1_i64;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u128 > -1_i64;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u64 > -1_i64;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u32 > -1_i64;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u16 > -1_i64;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u8 > -1_i64;
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    x_usize > -1_i32;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u128 > -1_i32;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u64 > -1_i32;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u32 > -1_i32;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u16 > -1_i32;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u8 > -1_i32;
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    x_usize > -1_i16;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u128 > -1_i16;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u64 > -1_i16;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u32 > -1_i16;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u16 > -1_i16;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u8 > -1_i16;
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    x_usize > -1_i8;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u128 > -1_i8;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u64 > -1_i8;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u32 > -1_i8;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u16 > -1_i8;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
    x_u8 > -1_i8;
// { dg-error ".E0308." "" { target *-*-* } .-1 }
}

