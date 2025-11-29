// Check that we report an error if an upcast box is moved twice.

fn consume(_: Box<[i32]>) {
}

fn foo(b: Box<[i32;5]>) {
    consume(b);
    consume(b); // { dg-error ".E0382." "" { target *-*-* } }
}

fn main() {
}

