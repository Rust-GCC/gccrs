// run-rustfix

// Point at the captured immutable outer variable

fn foo(mut f: Box<dyn FnMut()>) {
    f();
}

fn main() {
    let y = true;
    foo(Box::new(move || y = !y) as Box<_>);
// { dg-error ".E0594." "" { target *-*-* } .-1 }
}

