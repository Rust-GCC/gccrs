// Attempt to change the type as well as unsizing.

#![feature(unsized_tuple_coercion)]

struct Fat<T: ?Sized> {
    ptr: T
}

struct Foo;
trait Bar { fn bar(&self) {} }

pub fn main() {
    // With a vec of isize.
    let f1 = Fat { ptr: [1, 2, 3] };
    let f2: &Fat<[isize; 3]> = &f1;
    let f3: &Fat<[usize]> = f2;
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    // With a trait.
    let f1 = Fat { ptr: Foo };
    let f2: &Fat<Foo> = &f1;
    let f3: &Fat<dyn Bar> = f2;
// { dg-error ".E0277." "" { target *-*-* } .-1 }

    // Tuple with a vec of isize.
    let f1 = ([1, 2, 3],);
    let f2: &([isize; 3],) = &f1;
    let f3: &([usize],) = f2;
// { dg-error ".E0308." "" { target *-*-* } .-1 }

    // Tuple with a trait.
    let f1 = (Foo,);
    let f2: &(Foo,) = &f1;
    let f3: &(dyn Bar,) = f2;
// { dg-error ".E0277." "" { target *-*-* } .-1 }
}

