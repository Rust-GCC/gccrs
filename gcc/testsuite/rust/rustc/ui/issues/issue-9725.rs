struct A { foo: isize }

fn main() {
    let A { foo, foo } = A { foo: 3 };
// { dg-error ".E0025." "" { target *-*-* } .-1 }
// { dg-error ".E0025." "" { target *-*-* } .-2 }
}

