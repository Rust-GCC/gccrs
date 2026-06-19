static c_x: &'blk isize = &22; // { dg-error ".E0261." "" { target *-*-* } }

enum EnumDecl {
    Foo(&'a isize), // { dg-error ".E0261." "" { target *-*-* } }
    Bar(&'a isize), // { dg-error ".E0261." "" { target *-*-* } }
}

fn fnDecl(x: &'a isize, // { dg-error ".E0261." "" { target *-*-* } }
          y: &'a isize) // { dg-error ".E0261." "" { target *-*-* } }
{}

fn main() {
}

