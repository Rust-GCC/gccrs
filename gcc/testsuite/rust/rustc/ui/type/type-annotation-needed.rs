fn foo<T: Into<String>>(x: i32) {}
// { dg-note "" "" { target *-*-* } .-1 }

fn main() {
    foo(42);
// { dg-error ".E0283." "" { target *-*-* } .-1 }
// { dg-note ".E0283." "" { target *-*-* } .-2 }
// { dg-note ".E0283." "" { target *-*-* } .-3 }
}

