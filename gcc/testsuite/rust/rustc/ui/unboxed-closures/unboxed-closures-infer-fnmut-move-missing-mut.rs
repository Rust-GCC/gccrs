// Test that we are able to infer a suitable kind for this closure
// that is just called (`FnMut`).

fn main() {
    let mut counter = 0;
    let tick = move || counter += 1;
    tick(); // { dg-error ".E0596." "" { target *-*-* } }
}

