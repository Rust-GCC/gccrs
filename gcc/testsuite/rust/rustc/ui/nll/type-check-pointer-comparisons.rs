#![feature(nll)]

// Check that we assert that pointers have a common subtype for comparisons

fn compare_const<'a, 'b>(x: *const &mut &'a i32, y: *const &mut &'b i32) {
    x == y;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn compare_mut<'a, 'b>(x: *mut &'a i32, y: *mut &'b i32) {
    x == y;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn compare_fn_ptr<'a, 'b, 'c>(f: fn(&'c mut &'a i32), g: fn(&'c mut &'b i32)) {
    f == g;
// { dg-error "" "" { target *-*-* } .-1 }
// { dg-error "" "" { target *-*-* } .-2 }
}

fn compare_hr_fn_ptr<'a>(f: fn(&'a i32), g: fn(&i32)) {
    // Ideally this should compile with the operands swapped as well, but HIR
    // type checking prevents it (and stops compilation) for now.
    f == g; // OK
}

fn compare_const_fn_ptr<'a>(f: *const fn(&'a i32), g: *const fn(&i32)) {
    f == g; // OK
}

fn main() {}

