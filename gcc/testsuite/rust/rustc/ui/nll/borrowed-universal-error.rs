fn gimme(x: &(u32,)) -> &u32 {
    &x.0
}

fn foo<'a>(x: &'a (u32,)) -> &'a u32 {
    let v = 22;
    gimme(&(v,))
// { dg-error ".E0515." "" { target *-*-* } .-1 }
}

fn main() {}

