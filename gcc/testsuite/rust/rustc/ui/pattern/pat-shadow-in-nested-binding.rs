#[allow(non_camel_case_types)]
struct foo(usize);

fn main() {
    let (foo, _) = (2, 3); // { dg-error ".E0530." "" { target *-*-* } }
}

