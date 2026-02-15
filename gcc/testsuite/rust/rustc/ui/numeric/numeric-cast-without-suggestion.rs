fn foo<N>(_x: N) {}

fn main() {
    let x_usize: usize = 1;
    let x_u64: u64 = 2;
    let x_u32: u32 = 3;
    let x_u16: u16 = 4;
    let x_u8: u8 = 5;
    let x_isize: isize = 6;
    let x_i64: i64 = 7;
    let x_i32: i32 = 8;
    let x_i16: i16 = 9;
    let x_i8: i8 = 10;
    let x_f64: f64 = 11.0;
    let x_f32: f32 = 12.0;

    foo::<usize>(x_f64); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<usize>(x_f32); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<isize>(x_f64); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<isize>(x_f32); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<u64>(x_f64); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<u64>(x_f32); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<i64>(x_f64); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<i64>(x_f32); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<u32>(x_f64); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<u32>(x_f32); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<i32>(x_f64); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<i32>(x_f32); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<u16>(x_f64); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<u16>(x_f32); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<i16>(x_f64); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<i16>(x_f32); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<u8>(x_f64); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<u8>(x_f32); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<i8>(x_f64); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<i8>(x_f32); // { dg-error ".E0308." "" { target *-*-* } }
    foo::<f32>(x_f64); // { dg-error ".E0308." "" { target *-*-* } }
}

