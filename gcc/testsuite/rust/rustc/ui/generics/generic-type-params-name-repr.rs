use std::marker;

struct A;
struct B;
struct C;
struct Foo<T = A, U = B, V = C>(marker::PhantomData<(T,U,V)>);

struct Hash<T>(marker::PhantomData<T>);
struct HashMap<K, V, H = Hash<K>>(marker::PhantomData<(K,V,H)>);

fn main() {
    // Ensure that the printed type doesn't include the default type params...
    let _: Foo<isize> = ();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }

    // ...even when they're present, but the same types as the defaults.
    let _: Foo<isize, B, C> = ();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }

    // Including cases where the default is using previous type params.
    let _: HashMap<String, isize> = ();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
    let _: HashMap<String, isize, Hash<String>> = ();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }

    // But not when there's a different type in between.
    let _: Foo<A, isize, C> = ();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }

    // And don't print <> at all when there's just defaults.
    let _: Foo<A, B, C> = ();
// { dg-error ".E0308." "" { target *-*-* } .-1 }
// { dg-error ".E0308." "" { target *-*-* } .-2 }
// { dg-error ".E0308." "" { target *-*-* } .-3 }
// { dg-error ".E0308." "" { target *-*-* } .-4 }
}

