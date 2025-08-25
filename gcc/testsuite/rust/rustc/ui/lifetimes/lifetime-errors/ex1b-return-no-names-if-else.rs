fn foo(x: &i32, y: &i32) -> &i32 { // { dg-error ".E0106." "" { target *-*-* } }
    if x > y { x } else { y }
}

fn main() { }

