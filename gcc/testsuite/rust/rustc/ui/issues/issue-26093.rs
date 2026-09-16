macro_rules! not_a_place {
    ($thing:expr) => {
        $thing = 42;
// { dg-error ".E0067." "" { target *-*-* } .-1 }
        $thing += 42;
// { dg-error ".E0067." "" { target *-*-* } .-1 }
    }
}

fn main() {
    not_a_place!(99);
}

