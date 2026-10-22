struct Foo { bar: f64, baz: i64, bat: i64 }

fn main() {
    let _ = Foo { bar: .5, baz: 42 };
// { dg-error ".E0063." "" { target *-*-* } .-1 }
// { dg-error ".E0063." "" { target *-*-* } .-2 }
    let bar = 1.5f32;
    let _ = Foo { bar.into(), bat: -1, . };
// { dg-error ".E0063." "" { target *-*-* } .-1 }
// { dg-error ".E0063." "" { target *-*-* } .-2 }
// { dg-error ".E0063." "" { target *-*-* } .-3 }
}

