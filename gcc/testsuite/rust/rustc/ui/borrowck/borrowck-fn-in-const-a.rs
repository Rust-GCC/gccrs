// Check that we check fns appearing in constant declarations.
// Issue #22382.

const MOVE: fn(&String) -> String = {
    fn broken(x: &String) -> String {
        return *x // { dg-error ".E0507." "" { target *-*-* } }
    }
    broken
};

fn main() {
}

