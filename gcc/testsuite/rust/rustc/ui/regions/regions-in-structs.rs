struct Yes1<'a> {
  x: &'a usize,
}

struct Yes2<'a> {
  x: &'a usize,
}

struct StructDecl {
    a: &'a isize, // { dg-error ".E0261." "" { target *-*-* } }
    b: &'a isize, // { dg-error ".E0261." "" { target *-*-* } }
}


fn main() {}

