fn foo<'a>((x, y): (&'a i32, &i32)) -> &'a i32 {
    if x > y { x } else { y } // { dg-error ".E0621." "" { target *-*-* } }
}

fn main () { }

