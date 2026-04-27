// Check that non constant exprs fail for array repeat syntax

fn main() {
    fn bar(n: usize) {
        let _x = [0; n];
// { dg-error ".E0435." "" { target *-*-* } .-1 }
    }
}

