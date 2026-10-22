fn f<T>() -> bool {
    struct S(T); // { dg-error ".E0401." "" { target *-*-* } }

    true
}

fn main() {
    let b = f::<isize>();
    assert!(b);
}

