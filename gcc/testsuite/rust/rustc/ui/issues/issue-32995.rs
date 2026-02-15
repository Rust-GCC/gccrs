fn main() {
    let x: usize() = 1;
// { dg-error ".E0214." "" { target *-*-* } .-1 }

    let b: ::std::boxed()::Box<_> = Box::new(1);
// { dg-error ".E0214." "" { target *-*-* } .-1 }

    let p = ::std::str::()::from_utf8(b"foo").unwrap();
// { dg-error ".E0214." "" { target *-*-* } .-1 }

    let p = ::std::str::from_utf8::()(b"foo").unwrap();
// { dg-error ".E0214." "" { target *-*-* } .-1 }

    let o : Box<dyn (::std::marker()::Send)> = Box::new(1);
// { dg-error ".E0214." "" { target *-*-* } .-1 }

    let o : Box<dyn Send + ::std::marker()::Sync> = Box::new(1);
// { dg-error ".E0214." "" { target *-*-* } .-1 }
}

fn foo<X:Default>() {
    let d : X() = Default::default();
// { dg-error ".E0214." "" { target *-*-* } .-1 }
}

