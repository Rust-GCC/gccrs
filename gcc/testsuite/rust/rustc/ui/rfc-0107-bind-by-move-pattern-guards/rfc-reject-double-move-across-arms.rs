enum VecWrapper { A(Vec<i32>) }

fn foo(x: VecWrapper) -> usize {
    match x {
        VecWrapper::A(v) if { drop(v); false } => 1,
// { dg-error ".E0507." "" { target *-*-* } .-1 }
        VecWrapper::A(v) => v.len()
    }
}

fn main() {
    foo(VecWrapper::A(vec![107]));
}

