type Foo<T> = u32; // { dg-error ".E0091." "" { target *-*-* } }
type Foo2<A, B> = Box<A>; // { dg-error ".E0091." "" { target *-*-* } }

fn main() {
}

