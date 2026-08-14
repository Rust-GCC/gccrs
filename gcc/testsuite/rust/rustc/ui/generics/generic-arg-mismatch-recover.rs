struct Foo<'a, T: 'a>(&'a T);

struct Bar<'a>(&'a ());

fn main() {
    Foo::<'static, 'static, ()>(&0); // { dg-error ".E0107." "" { target *-*-* } }

    Bar::<'static, 'static, ()>(&()); // { dg-error ".E0107." "" { target *-*-* } }
// { dg-error ".E0107." "" { target *-*-* } .-1 }
}

