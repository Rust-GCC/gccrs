// ignore-tidy-linelength

// regression test for #26376

trait Foo {
    fn foo(&self) -> [u8];
}

fn foo(f: Option<&dyn Foo>) {
    if let Some(f) = f {
        let _ = f.foo();
// { dg-error ".E0161." "" { target *-*-* } .-1 }
    }
}

fn main() { foo(None) }

