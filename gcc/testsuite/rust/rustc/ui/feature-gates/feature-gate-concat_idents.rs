const XY_1: i32 = 10;

fn main() {
    const XY_2: i32 = 20;
    let a = concat_idents!(X, Y_1); // { dg-error ".E0658." "" { target *-*-* } }
    let b = concat_idents!(X, Y_2); // { dg-error ".E0658." "" { target *-*-* } }
    assert_eq!(a, 10);
    assert_eq!(b, 20);
}

