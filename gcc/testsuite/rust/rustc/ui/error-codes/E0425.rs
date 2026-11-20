trait Foo {
    fn bar() {
        elf; // { dg-error ".E0425." "" { target *-*-* } }
    }
}

fn main () {
}

