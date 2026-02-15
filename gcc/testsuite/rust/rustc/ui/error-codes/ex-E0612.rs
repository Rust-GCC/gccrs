struct Foo(u32);

fn main() {
   let y = Foo(0);
   y.1; // { dg-error ".E0609." "" { target *-*-* } }
}

