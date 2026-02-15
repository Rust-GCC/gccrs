// Test that we report an error if the trait ref in a qualified type
// uses invalid type arguments.

trait Foo<T> {
    type Bar;
    fn get_bar(&self) -> Self::Bar;
}

fn f<T:Foo<isize>>(t: &T) {
    let u: <T as Foo<usize>>::Bar = t.get_bar();
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

fn main() { }

