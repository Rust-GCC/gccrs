fn foo1<T:Copy<U>, U>(x: T) {}
// { dg-error ".E0107." "" { target *-*-* } .-1 }

trait Trait: Copy<dyn Send> {}
// { dg-error ".E0107." "" { target *-*-* } .-1 }

struct MyStruct1<T: Copy<T>>;
// { dg-error ".E0107." "" { target *-*-* } .-1 }

struct MyStruct2<'a, T: Copy<'a>>;
// { dg-error ".E0107." "" { target *-*-* } .-1 }


fn foo2<'a, T:Copy<'a, U>, U>(x: T) {}
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }

fn main() {
}

