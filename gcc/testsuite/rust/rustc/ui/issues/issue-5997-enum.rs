fn f<Z>() -> bool {
    enum E { V(Z) }
// { dg-error ".E0401." "" { target *-*-* } .-1 }
    true
}

fn main() {
    let b = f::<isize>();
    assert!(b);
}

