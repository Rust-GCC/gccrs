trait Foo {

fn foo<'a>(x: &i32, y: &'a i32) -> &'a i32 {
   if x > y { x } else { y } // { dg-error ".E0621." "" { target *-*-* } }
   }
}

fn main() { }

