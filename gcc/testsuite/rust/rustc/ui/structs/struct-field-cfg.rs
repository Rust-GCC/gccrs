struct Foo {
    present: (),
}

fn main() {
    let foo = Foo { #[cfg(all())] present: () };
    let _ = Foo { #[cfg(any())] present: () };
// { dg-error ".E0063." "" { target *-*-* } .-1 }
    let _ = Foo { present: (), #[cfg(any())] absent: () };
    let _ = Foo { present: (), #[cfg(all())] absent: () };
// { dg-error ".E0560." "" { target *-*-* } .-1 }
    let Foo { #[cfg(all())] present: () } = foo;
    let Foo { #[cfg(any())] present: () } = foo;
// { dg-error ".E0027." "" { target *-*-* } .-1 }
    let Foo { present: (), #[cfg(any())] absent: () } = foo;
    let Foo { present: (), #[cfg(all())] absent: () } = foo;
// { dg-error ".E0026." "" { target *-*-* } .-1 }
}

