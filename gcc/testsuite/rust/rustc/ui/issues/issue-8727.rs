// Verify the compiler fails with an error on infinite function
// recursions.

// build-fail
// normalize-stderr-test: ".nll/" -> "/"

fn generic<T>() { // { dg-warning "" "" { target *-*-* } }
    generic::<Option<T>>();
}
// { dg-error "" "" { target *-*-* } .-2 }


fn main () {
    // Use generic<T> at least once to trigger instantiation.
    generic::<i32>();
}

