fn inside_closure(x: &mut i32) {
}

fn outside_closure_1(x: &mut i32) {
}

fn outside_closure_2(x: &i32) {
}

fn foo(a: &mut i32) {
    let bar = || {
        inside_closure(a)
    };
    outside_closure_1(a);
// { dg-error ".E0501." "" { target *-*-* } .-1 }

    outside_closure_2(a);
// { dg-error ".E0501." "" { target *-*-* } .-1 }

    drop(bar);
}

fn main() {
}

