fn main() {
    // Assert `Iterator` methods are unstable
    assert!([1, 2, 2, 9].iter().is_sorted());
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    assert!(![-2i32, -1, 0, 3].iter().is_sorted_by_key(|n| n.abs()));
// { dg-error ".E0658." "" { target *-*-* } .-1 }

    // Assert `[T]` methods are unstable
    assert!([1, 2, 2, 9].is_sorted());
// { dg-error ".E0658." "" { target *-*-* } .-1 }
    assert!(![-2i32, -1, 0, 3].is_sorted_by_key(|n| n.abs()));
// { dg-error ".E0658." "" { target *-*-* } .-1 }
}

