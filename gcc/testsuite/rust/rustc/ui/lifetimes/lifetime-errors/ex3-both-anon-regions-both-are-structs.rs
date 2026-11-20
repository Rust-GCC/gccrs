struct Ref<'a> {
    x: &'a u32,
}

fn foo(mut x: Vec<Ref>, y: Ref) {
    x.push(y); // { dg-error ".E0623." "" { target *-*-* } }
}

fn main() {}

