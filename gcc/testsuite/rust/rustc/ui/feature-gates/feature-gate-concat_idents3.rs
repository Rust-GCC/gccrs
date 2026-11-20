const XY_1: i32 = 10;

fn main() {
    const XY_2: i32 = 20;
    assert_eq!(10, concat_idents!(X, Y_1)); // { dg-error ".E0658." "" { target *-*-* } }
    assert_eq!(20, concat_idents!(X, Y_2)); // { dg-error ".E0658." "" { target *-*-* } }
}

