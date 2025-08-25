struct Reactor {
    input_cells: Vec<usize>,
}

impl Reactor {
    pub fn new() -> Self {
        input_cells: Vec::new()
// { dg-error ".E0107." "" { target *-*-* } .-1 }
// { dg-error ".E0107." "" { target *-*-* } .-2 }
// { dg-error ".E0107." "" { target *-*-* } .-3 }
    }
}

// This case isn't currently being handled gracefully, including for completeness.
fn main() {}

