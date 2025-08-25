fn bar<'a>() -> &'a mut u32 {
    &mut 4
// { dg-error ".E0515." "" { target *-*-* } .-1 }
}

fn main() { }

