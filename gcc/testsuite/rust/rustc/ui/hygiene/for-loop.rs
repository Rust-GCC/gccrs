// for-loops are expanded in the front end, and use an `iter` ident in their expansion. Check that
// `iter` is not accessible inside the for loop.

fn main() {
    for _ in 0..10 {
        iter.next();  // { dg-error ".E0425." "" { target *-*-* } }
    }
}

