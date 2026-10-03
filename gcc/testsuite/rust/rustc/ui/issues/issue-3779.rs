struct S {
// { dg-error ".E0072." "" { target *-*-* } .-1 }
    element: Option<S>
}

fn main() {
    let x = S { element: None };
}

