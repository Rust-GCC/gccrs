struct Ref<'a, T: 'a> {
    data: &'a T
}

fn foo<'a>(x: Ref<i32>, y: &mut Vec<Ref<'a, i32>>) {
    y.push(x); // { dg-error ".E0621." "" { target *-*-* } }
}

fn main() { }

