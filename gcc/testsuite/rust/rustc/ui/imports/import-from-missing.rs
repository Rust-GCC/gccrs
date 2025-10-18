use spam::{ham, eggs}; // { dg-error ".E0432." "" { target *-*-* } }
// { dg-error ".E0432." "" { target *-*-* } .-1 }

mod spam {
    pub fn ham() { }
}

fn main() {
    ham();
    // Expect eggs to pass because the compiler inserts a fake name for it
    eggs();
}

