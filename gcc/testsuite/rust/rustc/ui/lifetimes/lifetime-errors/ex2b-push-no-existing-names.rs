struct Ref<'a, T: 'a> {
    data: &'a T
}

fn foo(x: &mut Vec<Ref<i32>>, y: Ref<i32>) {
    x.push(y); // { dg-error ".E0623." "" { target *-*-* } }
}

fn main() { }

