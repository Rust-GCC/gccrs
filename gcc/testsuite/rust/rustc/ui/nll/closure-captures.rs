// Some cases with closures that might be problems

// Should have one error per assignment

fn one_closure(x: i32) {
    ||
    x = 1; // { dg-error ".E0594." "" { target *-*-* } }
    move ||
    x = 1; // { dg-error ".E0594." "" { target *-*-* } }
}

fn two_closures(x: i32) {
    || {
        ||
        x = 1; // { dg-error ".E0594." "" { target *-*-* } }
    };
    move || {
        ||
        x = 1; // { dg-error ".E0594." "" { target *-*-* } }
    };
}

fn fn_ref<F: Fn()>(f: F) -> F { f }

fn two_closures_ref_mut(mut x: i32) {
    fn_ref(|| {
        || // { dg-error ".E0596." "" { target *-*-* } }
         x = 1;}
    );
    fn_ref(move || {
        ||  // { dg-error ".E0596." "" { target *-*-* } }
    x = 1;});
}

// This still gives two messages, but it requires two things to be fixed.
fn two_closures_ref(x: i32) {
    fn_ref(|| {
        || // { dg-error ".E0596." "" { target *-*-* } }
         x = 1;} // { dg-error ".E0594." "" { target *-*-* } }
    );
    fn_ref(move || {
        ||  // { dg-error ".E0596." "" { target *-*-* } }
    x = 1;}); // { dg-error ".E0594." "" { target *-*-* } }
}

fn two_closures_two_refs(x: &mut i32) {
    fn_ref(|| {
        || // { dg-error ".E0596." "" { target *-*-* } }
        *x = 1;});
    fn_ref(move || {
        || // { dg-error ".E0596." "" { target *-*-* } }
        *x = 1;});
}

fn main() {}

