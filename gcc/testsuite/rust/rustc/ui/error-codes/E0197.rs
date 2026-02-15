struct Foo;

unsafe impl Foo { } // { dg-error ".E0197." "" { target *-*-* } }

fn main() {
}

