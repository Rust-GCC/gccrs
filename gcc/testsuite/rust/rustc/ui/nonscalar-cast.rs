// run-rustfix

#[derive(Debug)]
struct Foo {
    x: isize
}

impl From<Foo> for isize {
    fn from(val: Foo) -> isize {
        val.x
    }
}

fn main() {
    println!("{}", Foo { x: 1 } as isize); // { dg-error ".E0605." "" { target *-*-* } }
}

