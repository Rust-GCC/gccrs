pub fn bar<F: Fn()>(_f: F) {} // { dg-note "" "" { target *-*-* } }

pub fn foo() {
    let mut x = 0;
    bar(move || x = 1);
// { dg-error ".E0594." "" { target *-*-* } .-1 }
// { dg-note ".E0594." "" { target *-*-* } .-2 }
// { dg-note ".E0594." "" { target *-*-* } .-3 }
}

fn main() {}

