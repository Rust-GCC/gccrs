// Check that an enum with recursion in the discriminant throws
// the appropriate error (rather than, say, blowing the stack).
enum X {
    A = X::A as isize, // { dg-error ".E0391." "" { target *-*-* } }
}

fn main() { }

