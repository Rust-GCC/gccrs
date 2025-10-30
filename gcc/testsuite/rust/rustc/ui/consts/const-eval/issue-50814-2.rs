// build-fail

trait C {
    const BOO: usize;
}

trait Foo<T> {
    const BAR: usize;
}

struct A<T>(T);

impl<T: C> Foo<T> for A<T> {
    const BAR: usize = [5, 6, 7][T::BOO]; // { dg-error "" "" { target *-*-* } }
}

fn foo<T: C>() -> &'static usize {
    &<A<T> as Foo<T>>::BAR // { dg-error ".E0080." "" { target *-*-* } }
}

impl C for () {
    const BOO: usize = 42;
}

impl C for u32 {
    const BOO: usize = 1;
}

fn main() {
    println!("{:x}", foo::<()>() as *const usize as usize);
    println!("{:x}", foo::<u32>() as *const usize as usize);
    println!("{:x}", foo::<()>());
    println!("{:x}", foo::<u32>());
}

