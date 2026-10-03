enum Foo { Alpha, Beta(isize) }

fn main() {
    match Foo::Alpha {
      Foo::Alpha | Foo::Beta(i) => {}
// { dg-error ".E0408." "" { target *-*-* } .-1 }
    }
}

