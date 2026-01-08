struct Generic<const N: usize>;

fn main() {
    let _a = Generic::<1>;
    let _b = Generic::<{ 1 + 1 }>;
    // { dg-error "cannot evaluate constant expression" "" { target *-*-* } .-1 }
}
