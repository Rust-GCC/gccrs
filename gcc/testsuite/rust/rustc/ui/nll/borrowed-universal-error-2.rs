fn foo<'a>(x: &'a (u32,)) -> &'a u32 {
    let v = 22;
    &v
// { dg-error ".E0515." "" { target *-*-* } .-1 }
}

fn main() {}

