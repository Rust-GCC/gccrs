fn foo(Option<i32>, String) {} // { dg-error "" "" { target *-*-* } }
// { dg-error "" "" { target *-*-* } .-1 }
fn bar(x, y: usize) {} // { dg-error "" "" { target *-*-* } }

fn main() {
    foo(Some(42), 2);
    foo(Some(42), 2, ""); // { dg-error ".E0061." "" { target *-*-* } }
    bar("", ""); // { dg-error ".E0308." "" { target *-*-* } }
    bar(1, 2);
    bar(1, 2, 3); // { dg-error ".E0061." "" { target *-*-* } }
}

