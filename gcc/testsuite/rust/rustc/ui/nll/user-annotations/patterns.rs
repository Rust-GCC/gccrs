// Test that various patterns also enforce types.

#![feature(nll)]

fn variable_no_initializer() {
    let x = 22;
    let y: &'static u32;
    y = &x; // { dg-error ".E0597." "" { target *-*-* } }
}

fn tuple_no_initializer() {


    let x = 22;
    let (y, z): (&'static u32, &'static u32);
    y = &x; // { dg-error ".E0597." "" { target *-*-* } }
}

fn ref_with_ascribed_static_type() -> u32 {
    // Check the behavior in some wacky cases.
    let x = 22;
    let y = &x; // { dg-error ".E0597." "" { target *-*-* } }
    let ref z: &'static u32 = y;
    **z
}

fn ref_with_ascribed_any_type() -> u32 {
    let x = 22;
    let y = &x;
    let ref z: &u32 = y;
    **z
}

struct Single<T> { value: T }

fn struct_no_initializer() {


    let x = 22;
    let Single { value: y }: Single<&'static u32>;
    y = &x; // { dg-error ".E0597." "" { target *-*-* } }
}


fn struct_no_initializer_must_normalize() {
    trait Indirect { type Assoc; }
    struct StaticU32;
    impl Indirect for StaticU32 { type Assoc = &'static u32; }
    struct Single2<T: Indirect> { value: <T as Indirect>::Assoc }

    let x = 22;
    let Single2 { value: mut _y }: Single2<StaticU32>;
    _y = &x; // { dg-error ".E0597." "" { target *-*-* } }
}

fn variable_with_initializer() {
    let x = 22;
    let y: &'static u32 = &x; // { dg-error ".E0597." "" { target *-*-* } }
}

fn underscore_with_initializer() {
    let x = 22;
    let _: &'static u32 = &x; // { dg-error ".E0597." "" { target *-*-* } }

    let _: Vec<&'static String> = vec![&String::new()];
// { dg-error ".E0716." "" { target *-*-* } .-1 }

    let (_, a): (Vec<&'static String>, _) = (vec![&String::new()], 44);
// { dg-error ".E0716." "" { target *-*-* } .-1 }

    let (_a, b): (Vec<&'static String>, _) = (vec![&String::new()], 44);
// { dg-error ".E0716." "" { target *-*-* } .-1 }
}

fn pair_underscores_with_initializer() {
    let x = 22;
    let (_, _): (&'static u32, u32) = (&x, 44); // { dg-error ".E0597." "" { target *-*-* } }
}

fn pair_variable_with_initializer() {
    let x = 22;
    let (y, _): (&'static u32, u32) = (&x, 44); // { dg-error ".E0597." "" { target *-*-* } }
}

fn struct_single_field_variable_with_initializer() {
    let x = 22;
    let Single { value: y }: Single<&'static u32> = Single { value: &x }; // { dg-error ".E0597." "" { target *-*-* } }
}

fn struct_single_field_underscore_with_initializer() {
    let x = 22;
    let Single { value: _ }: Single<&'static u32> = Single { value: &x }; // { dg-error ".E0597." "" { target *-*-* } }
}

struct Double<T> { value1: T, value2: T }

fn struct_double_field_underscore_with_initializer() {
    let x = 22;
    let Double { value1: _, value2: _ }: Double<&'static u32> = Double {
        value1: &x, // { dg-error ".E0597." "" { target *-*-* } }
        value2: &44,
    };
}

fn static_to_a_to_static_through_variable<'a>(x: &'a u32) -> &'static u32 {






    let y: &'a u32 = &22;
    y // { dg-error "" "" { target *-*-* } }
}

fn static_to_a_to_static_through_tuple<'a>(x: &'a u32) -> &'static u32 {







    let (y, _z): (&'a u32, u32) = (&22, 44);
    y // { dg-error "" "" { target *-*-* } }
}

fn static_to_a_to_static_through_struct<'a>(_x: &'a u32) -> &'static u32 {
    let Single { value: y }: Single<&'a u32> = Single { value: &22 };
    y // { dg-error "" "" { target *-*-* } }
}

fn a_to_static_then_static<'a>(x: &'a u32) -> &'static u32 {
    let (y, _z): (&'static u32, u32) = (x, 44); // { dg-error "" "" { target *-*-* } }
    y
}

fn main() { }

